#include <cs50.h>
#include <stdio.h>

int check_the_length(long card_number);
bool checksum(long card_number, int length);
string get_type_of_card(long card_number, int length);

int main(void)
{
    // Getting the card number
    long card_number = get_long("Number: ");
    // Getting its length
    int length = check_the_length(card_number);
    // Checking if it's valid
    bool valid = checksum(card_number, length);
    if (valid)
    {
        // Getting and printing cards type
        string type_of_card = get_type_of_card(card_number, length);
        printf("%s", type_of_card);
    }
    else
    {
        printf("INVALID\n");
    }
}

// Checks the length of the given card_number
int check_the_length(long card_number)
{
    int length_of_number = 0;
    while (card_number > 0)
    {
        card_number /= 10;
        length_of_number++;
    }
    return length_of_number;
}

// Checks if card is valid using Luhn’s Algorithm
bool checksum(long card_number, int length)
{
    int first_sum = 0;
    int second_sum = 0;
    for (int i = 0; i < length; i++)
    {
        int digit = card_number % 10;
        if (i % 2 == 0)
        {
            first_sum += digit;
        }
        else
        {
            digit *= 2;
            if (digit < 10)
            {
                second_sum += digit;
            }
            else
            {
                int sum_of_product_numbers = (digit % 10) + 1;
                second_sum += sum_of_product_numbers;
            }
        }
        card_number /= 10;
    }
    int total_sum = first_sum + second_sum;
    int result = total_sum % 10;
    if (result == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Checks again if the given card is vaid and if so, returns the name of name of card's the payment systems
string get_type_of_card(long card_number, int length)
{
    if (length == 15)
    {
        int first_two_digits = card_number / 10000000000000;
        if (first_two_digits == 34 || first_two_digits == 37)
        {
            return "AMEX\n";
        }
        else
        {
            return "INVALID\n";
        }
    }
    else if (length == 13)
    {
        int first_digit = card_number / 1000000000000;
        if (first_digit == 4)
        {
            return "VISA\n";
        }
        else
        {
            return "INVALID\n";
        }
    }
    else if (length == 16)
    {
        int first_two_digits = card_number / 100000000000000;
        int first_digit = first_two_digits / 10;
        if (first_two_digits == 51 || first_two_digits == 52 || first_two_digits == 53 || first_two_digits == 54 || first_two_digits == 55)
        {
            return "MASTERCARD\n";
        }
        else if (first_digit == 4)
        {
            return "VISA\n";
        }
        else
        {
            return "INVALID\n";
        }
    }
    else
    {
        return "INVALID\n";
    }
}