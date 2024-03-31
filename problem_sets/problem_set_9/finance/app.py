import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd, check_password

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get username and cash
    username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]["username"]
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = cash[0]["cash"]

    # Calculate values of their stocks and grand_total
    values_of_stocks = db.execute("SELECT total_value FROM portfolios WHERE user_id = ?", session["user_id"])

    total_values_of_all_stocks = 0
    for stock in values_of_stocks:
        total_values_of_all_stocks += stock["total_value"]

    grand_total = cash + total_values_of_all_stocks

    # Get their portfolio and add values for one share
    rows = db.execute("SELECT stock, amount, total_value FROM portfolios WHERE user_id = ?", session["user_id"])

    number = 0
    for row in rows:
        number += 1
        stock_symbol = row["stock"]
        price = lookup(stock_symbol)["price"]
        row["price"] = price
        row["number"] = number

    return render_template("index.html", rows=rows, username=username, cash=cash, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        current_time = datetime.now()

        symbol = request.form.get("symbol")
        amount_of_shares = request.form.get("shares")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 400)

        # Ensure shares was submitted
        elif not amount_of_shares:
            return apology("must provide amount of shares", 400)
        else:
            try:
                amount_of_shares = int(amount_of_shares)
            except ValueError:
                return apology("must provide amount of shares", 400)

        # Ensure the amount of shares is positive
        if amount_of_shares < 1:
            return apology("the number of shares must be a positive integer", 400)

        # Ensure that the symbol exist
        info = lookup(request.form.get("symbol"))
        if not info:
            return apology("try again", 400)

        # Calculate the total price of the purchase
        price_of_one_share = info["price"]
        total_price = price_of_one_share * amount_of_shares

        # Ensure user has enough money
        users_balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        users_balance = users_balance[0]['cash']
        if users_balance < total_price:
            return apology("You don't have enough money", 403)

        # Update users balance
        users_new_balance = users_balance - total_price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", users_new_balance, session["user_id"])

        # Add the shares
        purchased_shares = db.execute(
            "SELECT * FROM portfolios JOIN users ON portfolios.user_id = users.id WHERE id = ? AND stock = ?", session["user_id"], symbol)

        if len(purchased_shares) != 0:
            previos_amount = db.execute("SELECT amount FROM portfolios WHERE stock = ? AND user_id = ?", symbol, session["user_id"])
            previos_amount = previos_amount[0]["amount"]
            new_amount = previos_amount + amount_of_shares
            total_value = new_amount * price_of_one_share
            db.execute("UPDATE portfolios SET amount = ?, time = ?, total_value = ? WHERE user_id = ? AND stock = ?",
                       new_amount, current_time, total_value, session["user_id"], symbol)
        else:
            total_value = amount_of_shares * price_of_one_share
            db.execute("INSERT INTO portfolios (stock, amount, time, total_value, user_id) VALUES (?, ?, ?, ?, ?)",
                       symbol, amount_of_shares, current_time, total_value, session["user_id"])


        # Update history database
        db.execute("INSERT INTO history (user_id, stock, amount, price, type, time) VALUES (?, ?, ?, ?, ?, ?)", session["user_id"],
                   symbol, amount_of_shares, total_price, "purchase", current_time)

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    rows = db.execute("SELECT stock, amount, price, type, time FROM history WHERE user_id = ?", session["user_id"])

    number = 0
    for row in rows:
        number += 1
        row["number"] = number

    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        info = lookup(request.form.get("symbol"))

        if info:
            return render_template("quoted.html", name=info["name"], price=info["price"], symbol=info["symbol"])
        else:
            return apology("Try again!", 400)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    session.clear()

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide password", 400)

        # Ensure passwords match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        # Ensure password is proper
        elif not check_password(request.form.get("password")):
            return apology("password must contain at least one digit, one special character and one upper letter", 400)

        # Generate hash of the password
        hashed_password = generate_password_hash(request.form.get("password"))

        # Ensure username does not exist
        usernames = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if len(usernames) > 0:
            return apology("username already exists", 400)
        else:
            name = request.form.get("username")

        # Add username and hash into the database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", name, hashed_password)

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    available_stocks = db.execute("SELECT stock FROM portfolios WHERE user_id = ?", session["user_id"])

    if request.method == "POST":

        current_time = datetime.now()

        symbol = request.form.get("symbol")
        amount_of_shares = request.form.get("shares")

        # Ensure user submitted positive integer
        if not amount_of_shares:
            return apology("must provide positive integer", 400)
        else:
            amount_of_shares = int(amount_of_shares)

        if amount_of_shares < 1:
            return apology("must provide positive integer", 400)

        # Ensure proper stock symbol was submitted and user has this stock
        users_stocks = list()
        rows_of_stocks = db.execute("SELECT stock FROM portfolios WHERE user_id = ?", session["user_id"])
        for row in rows_of_stocks:
            users_stocks.append(row["stock"])

        if not symbol:
            return apology("must provide stock symbol", 400)

        elif not lookup(symbol):
            return apology("must provide proper stock symbol", 400)

        elif symbol not in users_stocks:
            return apology("sorry you don't have this stock", 400)

        # Ensure user has enough shares
        number_of_users_shares = db.execute("SELECT amount FROM portfolios WHERE stock = ? AND user_id = ?",
                                            symbol, session["user_id"])[0]["amount"]

        if amount_of_shares > number_of_users_shares:
            return apology("sorry you don't have enough shares", 400)

        # Calculate the price
        price_for_one_share = lookup(symbol)["price"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = cash[0]["cash"]
        total_price = price_for_one_share * amount_of_shares
        new_cash = cash + total_price

        # Add cash to the user
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])

        # Remove the shares
        if number_of_users_shares - amount_of_shares == 0:
            db.execute("DELETE FROM portfolios WHERE stock = ? AND user_id = ?", symbol, session["user_id"])

        else:
            new_amount_of_shares = number_of_users_shares - amount_of_shares
            db.execute("UPDATE portfolios SET amount = ? WHERE stock = ? AND user_id = ?",
                       new_amount_of_shares, symbol, session["user_id"])

        # Update history database
        db.execute("INSERT INTO history (user_id, stock, amount, price, type, time) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], symbol, amount_of_shares, total_price, "sale", current_time)

        return redirect("/")

    else:
        return render_template("sell.html", available_stocks=available_stocks)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "POST":

        old_password = request.form.get("old_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("password_confirmation")

        # Ensure proper old password was submitted
        hashed_password = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])
        hashed_password = hashed_password[0]["hash"]
        if not check_password_hash(hashed_password, old_password):
            return apology("you typed the wrong password", 400)

        # Ensure that new password is proper
        elif not check_password(new_password):
            return apology("password must contain at least one digit, one special character and one upper letter", 400)

        # Ensure passwords match
        elif new_password != confirmation:
            return apology("password don't match", 400)

        # Update the database
        new_password = generate_password_hash(new_password)
        db.execute("UPDATE users SET hash = ? WHERE id = ?", new_password, session["user_id"])

        return redirect("/")

    else:
        return render_template("change_password.html")