#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int calculate_level(float number_of_letters_per_100_words, float number_of_sentences_per_100_words);


int main(void)
{
    string text = get_string("Text: ");
    // Getting numbers of letters, words and sentences in the text
    int number_of_letters = count_letters(text);
    int number_of_words = count_words(text);
    int number_of_sentences = count_sentences(text);

    // Calculating the number of letters and sentences per 100 words
    float number_of_letters_per_100_words = (float) number_of_letters / number_of_words * 100;
    float number_of_sentences_per_100_words = (float) number_of_sentences / number_of_words * 100;

    // Calcucaling the level of the given text
    int index = calculate_level(number_of_letters_per_100_words, number_of_sentences_per_100_words);

    // Printing the result
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// Counts letters in the given text
int count_letters(string text)
{
    int number_of_letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        int letter = isalpha(text[i]);
        if (letter != 0)
        {
            number_of_letters++;
        }
    }
    return number_of_letters;
}

// Counts words in the given text
int count_words(string text)
{
    int number_of_spaces = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        int space = isblank(text[i]);
        if (space != 0)
        {
            number_of_spaces++;
        }
    }
    int number_of_words = number_of_spaces + 1;
    return number_of_words;
}

// Counts sentences in the given text
int count_sentences(string text)
{
    int number_of_sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            number_of_sentences++;
        }
    }
    return number_of_sentences;
}

// Calculate the level of the given text using "Coleman-Liau index"
int calculate_level(float number_of_letters_per_100_words, float number_of_sentences_per_100_words)
{
    double index = 0.0588 * number_of_letters_per_100_words - 0.296 * number_of_sentences_per_100_words - 15.8;
    index = round(index);
    return (int) index;
}