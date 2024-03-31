#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool only_digits(string key);
char rotate(char letter, int key);


int main(int argc, string argv[])
{
    // Checking the command-line arguments
    if (argc != 2 || only_digits(argv[1]) != true)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Getting the text from a user and printing the first output
    string text = get_string("plaintext: ");
    int key = atoi(argv[1]);
    int length_of_text = strlen(text);
    printf("ciphertext: ");

    // Iterating through the text, rotating each letter if needed
    for (int i = 0, n = length_of_text; i < n; i++)
    {
        char letter = rotate(text[i], key);
        printf("%c", letter);
    }
    printf("\n");
}


// Checks if the given string contains only digits
bool only_digits(string key)
{
    int length = strlen(key);
    for (int i = 0, n = length; i < n; i++)
    {
        int digit = isdigit(key[i]);
        if (digit == 0)
        {
            return false;
        }
    }
    return true;
}

// Rotates the char using given key
char rotate(char letter, int key)
{
    if (isalpha(letter) == 0)
    {
        return letter;
    }
    else
    {
        if (islower(letter) > 0)
        {
            letter -= 97;
            if (letter + key < 26)
            {
                letter += key + 97;
                return letter;
            }
            else
            {
                letter = (letter + key) % 26 + 97;
                return letter;
            }
        }
        else
        {
            letter -= 65;
            if (letter + key < 26)
            {
                letter += key + 65;
                return letter;
            }
            else
            {
                letter = (letter + key) % 26 + 65;
                return letter;
            }
        }
    }
}
