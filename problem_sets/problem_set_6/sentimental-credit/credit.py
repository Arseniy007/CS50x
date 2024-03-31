def main():
    # Get the card_number from the user
    card_number = input("Number: ")
    length_of_number = len(card_number)

    # Check its validity and if valid - print the type of the card
    if valid(card_number, length_of_number):
        type_of_card = get_type_of_card(card_number, length_of_number)
        print(type_of_card)
    else:
        print("INVALID")


def valid(card_number, length_of_number):
    card_number = int(card_number)
    first_sum, second_sum = 0, 0

    for i in range(length_of_number):
        digit = card_number % 10
        if i % 2 == 0:
            first_sum += digit
        else:
            digit *= 2
            if digit < 10:
                second_sum += digit
            else:
                second_sum += (digit % 10) + 1
        card_number //= 10

    result = (first_sum + second_sum) % 10
    if result == 0:
        return True
    else:
        return False


def get_type_of_card(card_number, length_of_number):
    if length_of_number == 13:
        if card_number[0] == "4":
            return "VISA"
        else:
            return "INVALID"
    elif length_of_number == 15:
        if card_number[:2] == "34" or card_number[:2] == "37":
            return "AMEX"
        else:
            return "INVALID"
    elif length_of_number == 16:
        if card_number[:2] in ["51", "52", "53", "54", "55"]:
            return "MASTERCARD"
        elif card_number[0] == "4":
            return "VISA"
        else:
            return "INVALID"
    else:
        return "INVALID"


if __name__ == "__main__":
    main()
