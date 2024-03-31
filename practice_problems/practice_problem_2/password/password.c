#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

bool valid(string password);

int main(void)
{
    // Get input from the user
    string password = get_string("Enter your password: ");

    // Print the result
    if (valid(password))
    {
        printf("Your password is valid!\n");
    }
    else
    {
        printf("Your password needs at least one uppercase letter, lowercase letter, number and symbol\n");
    }
}

// Checks if the password is valid
bool valid(string password)
{
    bool upper_case_letter = false;
    bool lower_case_letter = false;
    bool number = false;
    bool symbol = false;
    int length = strlen(password);

    // Interating through the each letter
    for (int i = 0, n = length; i < n; i++)
    {
        char letter = password[i];

        if (isupper(letter) > 0)
        {
            upper_case_letter = true;
        }
        else if (islower(letter) > 0)
        {
            lower_case_letter = true;
        }
        else if (isdigit(letter) > 0)
        {
            number = true;
        }
        else if (isalnum(letter) == 0)
        {
            symbol = true;
        }
    }
    // Reterning the result
    if (upper_case_letter && lower_case_letter && number && symbol)
    {
        return true;
    }
    else
    {
        return false;
    }
}