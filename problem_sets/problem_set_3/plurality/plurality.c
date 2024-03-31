#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0, n = candidate_count; i < n; i++)
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Implementing buble sort to sort the candidates
    for (int i = 0, n = candidate_count; i < n - 1; i++)
    {
        for (int j = 0, m = candidate_count; j < m - 1; j++)
        {
            int votes_1 = candidates[j].votes;
            int votes_2 = candidates[j + 1].votes;

            if (votes_1 > votes_2)
            {
                candidate one = candidates[j];
                candidate two = candidates[j + 1];
                candidates[j] = two;
                candidates[j + 1] = one;
            }
        }
    }
    // Calculate the number of winners
    candidate winner = candidates[candidate_count - 1];
    int winners = 0;
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        if (winner.votes == candidates[i].votes)
        {
            winners++;
        }
    }
    // Printing the result
    if (winners == 1)
    {
        printf("%s\n", winner.name);
    }
    else
    {
        for (int j = 1, w = winners; j < w + 1; j++)
        {
            printf("%s\n", candidates[candidate_count - j].name);
        }
    }
    return;
}