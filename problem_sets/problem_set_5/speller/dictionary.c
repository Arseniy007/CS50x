// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 17576;

// Hash table
node *table[N];

// Keep track of number of added words in the hash table
int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash the word
    unsigned int hash_index = hash(word);

    node *current_word_in_hash_table = table[hash_index];

    // Check if the word is in the only possibly list
    while (current_word_in_hash_table != NULL)
    {
        if (strcasecmp(current_word_in_hash_table->word, word) == 0)
        {
            return true;
        }
        current_word_in_hash_table = current_word_in_hash_table->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hash the word using its first the letters
    int first_three_letters[3];

    if (strlen(word) >= 3)
    {
        for (int i = 0; i < 3; i++)
        {
            first_three_letters[i] = tolower(word[i]) - 97;
        }
    }
    else if (strlen(word) == 2)
    {
        for (int i = 0; i < 2; i++)
        {
            first_three_letters[i] = tolower(word[i]) - 97;
        }
        first_three_letters[2] = 0;
    }
    else
    {
        first_three_letters[0] = tolower(word[0]) - 97;
        first_three_letters[1] = 0;
        first_three_letters[2] = 0;
    }
    unsigned int hash_index = first_three_letters[0] * 676 + first_three_letters[1] * 26 + first_three_letters[2];
    return hash_index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }
    char word[LENGTH + 1];

    // Read each word in the dictionary and add it to the hash table
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Create new node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }

        // Copy the word into our ner node
        strcpy(new_node->word, word);
        new_node->next = NULL;

        // Hash the word
        unsigned int hash_index = hash(word);

        word_count++;

        // Add the word to the hash table
        if (table[hash_index] == NULL)
        {
            table[hash_index] = new_node;
        }
        else
        {
            new_node->next = table[hash_index];
            table[hash_index] = new_node;
        }
    }

    // Close dictionary
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    unsigned int size_of_hash_table = N;

    for (int i = 0; i < size_of_hash_table; i++)
    {
        node *cursor = table[i];
        node *tmp = table[i];

        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}