#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

bool only_letters(string key);
bool no_duplicates(string key);
char change_the_letter(char letter, string key);


int main(int argc, string argv[])
{

    // Checking the command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    string key = argv[1];
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    if (only_letters(key) != true || no_duplicates(key) != true)
    {
        printf("Key is invalid");
        return 1;
    }

    // Getting input from the user
    string text = get_string("plaintext: ");
    int length = strlen(text);
    printf("ciphertext: ");

    // Iterating through the given text changing each letter
    for (int i = 0; i < length; i++)
    {
        char letter = change_the_letter(text[i], key);
        printf("%c", letter);
    }
    printf("\n");
    return 0;
}

// Checks if users key contains only letters
bool only_letters(string key)
{
    int length = strlen(key);
    for (int i = 0; i < length; i++)
    {
        int alpha = isalpha(key[i]);
        if (alpha == 0)
        {
            return false;
        }
    }
    return true;
}

// Checks if there is no duplicates (same letters) in users key
bool no_duplicates(string key)
{
    int length = strlen(key);
    for (int i = 0; i < length; i++)
    {
        char letter = key[i];
        int counter = 0;
        for (int j = 0; j < length; j++)
        {
            char duplicate = key[j];
            if (letter == duplicate)
            {
                counter++;
            }
        }
        if (counter != 1)
        {
            return false;
        }
    }
    return true;
}

// Changes letter using the users key
char change_the_letter(char letter, string key)
{
    if (isupper(letter) > 0)
    {
        int letters_number = letter - 65;
        letter = toupper(key[letters_number]);
        return letter;
    }
    else if (islower(letter) > 0)
    {
        int letters_number = letter - 97;
        letter = tolower(key[letters_number]);
        return letter;
    }
    else
    {
        return letter;
    }
}