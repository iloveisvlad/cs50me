// dictionary.c

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Number of buckets in hash table
#define N 65536

// Represents a node in a linked list
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Hash table
static node *table[N];

// Count of words loaded
static unsigned int word_count = 0;


/**
 * djb2 hash function (case‑insensitive)
 * Source: http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned int hash(const char *word)
{
    unsigned long h = 5381;
    int c;
    while ((c = tolower((unsigned char)*word++)))
    {
        h = ((h << 5) + h) + c; // h * 33 + c
    }
    return h % N;
}

/**
 * Loads dictionary into memory; returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        return false;
    }

    char buffer[LENGTH + 1];
    // Read one word at a time
    while (fscanf(file, "%45s", buffer) != EOF)
    {
        // Allocate node
        node *n = malloc(sizeof(node));
        if (!n)
        {
            fclose(file);
            return false;
        }

        // Copy word into node
        strcpy(n->word, buffer);
        // Compute hash bucket
        unsigned int bucket = hash(buffer);
        // Insert at head of list
        n->next = table[bucket];
        table[bucket] = n;

        word_count++;
    }

    fclose(file);
    return true;
}

/**
 * Returns true if word is in dictionary else false.
 * Case-insensitive.
 */
bool check(const char *word)
{
    // Lowercase copy of word
    char lower[LENGTH + 1];
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        lower[i] = tolower((unsigned char)word[i]);
    }
    lower[len] = '\0';

    unsigned int bucket = hash(lower);
    for (node *cursor = table[bucket]; cursor != NULL; cursor = cursor->next)
    {
        if (strcmp(cursor->word, lower) == 0)
        {
            return true;
        }
    }
    return false;
}

/**
 * Returns number of words loaded into dictionary.
 */
unsigned int size(void)
{
    return word_count;
}

/**
 * Unloads dictionary from memory; returns true if successful else false.
 */
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
        table[i] = NULL;
    }
    return true;
}
