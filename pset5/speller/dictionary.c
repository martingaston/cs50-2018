// Implements a dictionary's functionality

#include <stdio.h>   // file functions and printf
#include <stdbool.h> // boolean type
#include <stdint.h>  // integer types
#include <string.h>  // string functions
#include <stdlib.h>  // malloc and free
#include <ctype.h>   // tolower

#include "dictionary.h"
#include "murmur3.h" // hashing implementation via https://github.com/PeterScott/murmur3

// define starting hash table & other globals

// TODO: the htable is sized at 2861 (a prime) to keep lots of room for minimal collisions
// a dynamic resizing method would be more space efficient :)

LIST_NODE *htable[2861]; // hash table global will house the dictionary
int htable_size = sizeof(htable) / sizeof(htable[0]);
int count = 0;
bool loaded = false;

/*
*   HASH FUNCTIONS
*/

uint32_t generate_hash(const char *word)
{
    // create hash output and seed value for Murmur3
    uint32_t hash[1];   /* output value for hash */
    uint32_t seed = 42; /* Seed value for hash */

    MurmurHash3_x86_32(word, strlen(word), seed, hash);
    return hash[0] % htable_size;
}

int hash(uint32_t key, char *word)
{
    if (htable[key] == NULL)
    {
        htable[key] = malloc(sizeof(LIST_NODE));
        htable[key]->value = malloc(strlen(word) + 1);
        strcpy(htable[key]->value, word);
        htable[key]->next = NULL;
    }
    else
    {
        shift(&htable[key], word);
    }

    return 0;
}

/*
*   LINKED LIST FUNCTIONS
*/

// i really don't understand the double pointer here :(
// this is based on the tutorial at https://www.learn-c.org/en/Linked_lists
void shift(LIST_NODE **head, char *value)
{
    LIST_NODE *new_head = malloc(sizeof(LIST_NODE));
    if (new_head == NULL)
    {
        fprintf(stderr, "Could not assign memory for %s.\n", value);
        return;
    }
    new_head->value = malloc(strlen(value) + 1);
    if (new_head->value == NULL)
    {
        fprintf(stderr, "Could not assign memory for %s.\n", value);
        return;
    }
    strcpy(new_head->value, value);
    new_head->next = *head;
    *head = new_head;
}

// Free individual linked lists recursively from head pointer
int freelist(LIST_NODE *head)
{
    if (!head || head == NULL)
    {
        return 0;
    }
    if (head->next != NULL)
    {
        freelist(head->next);
    }
    if (head->value != NULL)
    {
        free(head->value);
    }
    if (head != NULL)
    {
        free(head);
        return 0;
    }
    // if we get here something must have gone horribly wrong!
    fprintf(stderr, "Something went wrong :(\n");
    return 1;
}

/*
*   DICTIONARY FUNCTIONS
*/

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // convert to lowercase
    char *lword = malloc(strlen(word) * sizeof(word[0]) + 1); // sizeof(word) is just the size of the pointer, not its contents
    strcpy(lword, word);
    int len = strlen(lword);
    for (int i = 0; i < len; i++)
    {
        lword[i] = tolower(lword[i]);
    }

    uint32_t n = generate_hash(lword);

    LIST_NODE *current = htable[n];

    while (current != NULL)
    {
        if (strcmp(current->value, lword) == 0)
        {
            free(lword);
            return true;
        }
        current = current->next;
    }
    free(lword);
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary
    FILE *dictptr = fopen(dictionary, "r");
    if (dictptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    // load dictionary into linked list

    int index = 0;
    char word[LENGTH + 1];

    for (int c = fgetc(dictptr); c != EOF; c = fgetc(dictptr))
    {
        if (c == '\n')
        {
            count += 1;
            word[index] = '\0';
            index = 0;
            hash(generate_hash(word), word);
        }
        else
        {
            word[index] = c;
            index++;
        }
    }

    // close files and success
    fclose(dictptr);
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (loaded)
    {
        return count;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < htable_size; i++)
    {
        if (freelist(htable[i]) != 0)
        {
            fprintf(stderr, "Could not free hashtable row %i\n", i);
            return false;
        }
    }
    return true;
}