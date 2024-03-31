#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool cycle(int winner, int loser, int first_winner);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Iterating through each candidate
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        // If the name is valid, recording their rank in a ballot
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Iterating through each candidate
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        // If it is not the last candidate in the ballot, iterating through every other candidated
        if (i < candidate_count - 1)
        {
            for (int j = i + 1, m = candidate_count; j < m; j++)
            {
                // Uploding preferences array, sicne this candidate was preferred over any other behind them in the ballot
                preferences[ranks[i]][ranks[j]]++;
            }
        }
        // Don't include the last candidate in the ballot
        else
        {
            break;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Making array of all possible pairs, setting all to false to keep track of them later
    bool possible_pairs[candidate_count][candidate_count];

    for (int i = 0, n = candidate_count; i < n; i++)
    {
        for (int j = i, m = candidate_count; j < m; j++)
        {
            possible_pairs[i][j] = false;
        }
    }
    // Setting the counter of pairs to 0
    pair_count = 0;

    // Iterating through each possible pair
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        for (int j = i + 1, m = candidate_count + 1; j < m; j++)
        {
            // Checking if this particular pair hasn't been already checked
            if (!possible_pairs[i][j] && !possible_pairs[j][i])
            {

                // Checking who is winner and who is loser in this pair and updating the global array of pairs
                if (preferences[i][j] > preferences[j][i])
                {
                    // Updating the number of pairs
                    pair_count++;

                    pairs[pair_count - 1].winner = i;
                    pairs[pair_count - 1].loser = j;
                }
                else if (preferences[i][j] < preferences[j][i])
                {
                    // Updating the number of pairs
                    pair_count++;

                    pairs[pair_count - 1].winner = j;
                    pairs[pair_count - 1].loser = i;
                }
                // Marking this pair as already checked
                possible_pairs[i][j] = true;
                possible_pairs[j][i] = true;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Implementing buble sort to sort the pairs
    for (int i = 0, n = pair_count; i < n - 1; i++)
    {
        for (int j = 0, m = pair_count; j < m - 1; j++)
        {
            int first_strength = preferences[pairs[j].winner][pairs[j].loser];
            int second_strength = preferences[pairs[j + 1].winner][pairs[j + 1].loser];

            if (first_strength < second_strength)
            {
                pair first_pair = pairs[j];
                pair second_pair = pairs[j + 1];
                pairs[j] = second_pair;
                pairs[j + 1] = first_pair;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0, n = pair_count; i < n; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        // Check if this pair will create a cycle or not
        if (!(cycle(winner, loser, winner)))
        {
            // If not - we lock the pair
            locked[winner][loser] = true;
        }
    }
    return;
}

// Check if the given pare will lead to cycle
bool cycle(int winner, int loser, int first_winner)
{
    // Base case for our recursion-function. If the first winner in a pair than after few pairs becomes the loser in the same chain - it's a cycle.
    if (loser == first_winner)
    {
        return true;
    }
    // Iterating through other chains in the winner-looser pairs
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        if (locked[loser][i])
        {
            // Calling itself if our winner was already looser in another pair
            bool one_more_cycle = cycle(loser, i, first_winner);
            if (one_more_cycle)
            {
                return true;
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // Iterating through each candidate
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        bool winner = true;
        // Checking if he is locked over by somebody
        for (int j = 0, m = candidate_count; j < m; j++)
        {
            if (locked[j][i])
            {
                winner = false;
            }
        }
        // Print the winner
        if (winner)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}