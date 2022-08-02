// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 676 + 1;
int total_words = 0;
unsigned int key;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    key = hash(word);
    node *n = table[key];
    while (n != NULL)
    {
        if (strcasecmp(word, n->word) != 0)
        {
            n = n->next;
        }
        else
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int h1 = 0;
    int h2 = 0;
    // TODO: Improve this hash function
    if (word[1] == 0)
    {
        return N;
    }
    else
    {
        h1 = toupper(word[0]) - 'A';
        h2 = toupper(word[1]) - 'A';
        return (h1 * 26) + h2;
    }

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    char buffer[LENGTH + 1];
    //Open dictionary file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    //Read strings from file one at a time
    while (fscanf(dict, "%s", buffer) != EOF)
    {
        //Create a new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, buffer);
        n->next = NULL;
        //Hash word to obtain a hash value
        key = hash(buffer);
        //Insert node into hash table at that location
        n->next = table[key];
        table[key] = n;
        total_words++;
    }
    fclose(dict);
    return true;

}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (total_words > 0)
    {
        return total_words;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{

    // Iterate through buckets
    for (int i = 0; i < N; i++)
    {
        node *n = table[i];
        while (n != NULL)
        {
            node *tmp = n;
            n = n->next;
            free(tmp);
        }
    }
    return true;
}
