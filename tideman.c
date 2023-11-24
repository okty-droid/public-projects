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
void print_winner(void);
int connected(int winner, int loser);

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

    // TODO done
    for (int k = 0; k < candidate_count; k++)
    {
        if (strcmp(candidates[k], name) == 0)
        {
            ranks[rank] = k;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
// preferences[i][j] is number of voters who prefer i over j
void record_preferences(int ranks[])
{
    for (int first = 0; first < candidate_count; first++)
    {
        for (int second = first + 1; second < candidate_count; second++)
        {
            preferences[ranks[first]][ranks[second]]++;
        }
    }
    // TODO done
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int first = 0; first < candidate_count; first++)
    {
        for (int second = 0; second < candidate_count; second++)
        {
            if (preferences[first][second] > preferences[second][first])
            {
                pairs[pair_count].winner = first;
                pairs[pair_count].loser = second;
                pair_count++;
            }
        }
    }

    // TODO done
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair a;
    pair b;
    for (int time = 0; time < pair_count; time++) // amount of times iterated, for bubble sort
    {
        for (int t = 0; t < pair_count - 1; t++) //goes through and swaps stuff
        {
            if (preferences[pairs[t + 1].winner][pairs[t + 1].loser] > preferences[pairs[t].winner][pairs[t].loser])
            {
                //swap pairst and pairs t+1
                a = pairs[t];
                pairs[t] = pairs[t + 1];
                pairs[t + 1] = a;
            }
        }
    }
    // TODO
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
// locked[i][j] means i is locked in over j
// can make it so that each each connected component has the same 'value' of all members, which is value of member added first?
//add in egde locked[winner][loser] if no cycle
// change values in value so each connected component has same value everywhere.
//MAY NEED TO SET VALUE TO 0 INITIALLY???
void lock_pairs(void)
{

    for (int p = 0; p < pair_count; p++)
    {
        if (connected(pairs[p].winner, pairs[p].loser) == 0)
        {
            locked[pairs[p].winner][pairs[p].loser] = 1;

            //printf("added edge from %i to %i", pairs[p].winner, pairs[p].loser);
        }
    }
    // TODO done i think, claims q unused???
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int w = 0; w < candidate_count; w++)
    {
        int total = 0;
        for (int w2 = 0; w2 < candidate_count; w2++)
        {
            total += locked[w2][w];
        }
        if (total == 0)
        {
            printf("%s\n", candidates[w]);
            return;
        }
    }
    // TODO
    return;
}

// want 0 if not connected, 1 if connected
int connected(int winner, int loser)
{
    int value[candidate_count];

    for (int p = 0; p < candidate_count; p++)
    {
        value[p] = 0;
    }

    value[loser] = 1;
    for (int loops = 0; loops < candidate_count; loops++)
    {
        for (int l1 = 0; l1 < candidate_count; l1++)
        {

            for (int l2 = 0; l2 < candidate_count; l2++)
            {
                if (locked[l1][l2] == 1 && value[l1] == 1)
                {
                    value[l2] = 1;
                }
            }
        }

    }

    return value[winner];
}
