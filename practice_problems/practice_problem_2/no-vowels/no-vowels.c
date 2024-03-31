#include <cs50.h>
#include <stdio.h>
#include <string.h>

string replace(string word);


int main(int argc, string argv[])
{
    // Check command-line argiment
    if (argc != 2)
    {
        printf("Usage: ./no-vowels word\n");
        return 1;
    }

    // Convert and print word
    string word = argv[1];
    string converted_word = replace(word);
    printf("%s\n", converted_word);
}

// Converts every vowel in the given word
string replace(string word)
{
    int length = strlen(word);
    for (int i = 0, n = length; i < n; i++)
    {
        char letter = word[i];

        switch (letter)
        {
            case 'a':
                word[i] = '6';
                break;

            case 'e':
                word[i] = '3';
                break;

            case 'i':
                word[i] = '1';
                break;

            case 'o':
                word[i] = '0';
                break;
        }
    }
    return word;
}