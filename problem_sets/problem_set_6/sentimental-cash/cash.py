from cs50 import get_float


def main():
    # Get the amount of change
    amount = int(get_amount() * 100)

    # Calcutale quarters
    quarters = calculate_quarters(amount)
    amount -= quarters * 25

    # Calculate dimes
    dimes = calculate_dimes(amount)
    amount -= dimes * 10

    # Calculate nickels
    nickels = calculate_nickels(amount)

    # Calculate nickels
    amount -= nickels * 5

    # Calculate pennies
    pennies = calculate_pennies(amount)
    amount -= pennies
    
    # Get the amount of coins
    cents = quarters + dimes + nickels + pennies
    print(cents)


def get_amount():
    while True:
        amount = get_float("Change owed: ")
        if amount > 0:
            return amount
        else:
            continue


def calculate_quarters(amount):
    return amount // 25


def calculate_dimes(amount):
    return amount // 10


def calculate_nickels(amount):
    return amount // 5


def calculate_pennies(amount):
    return amount


if __name__ == "__main__":
    main()
