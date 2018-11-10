// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <stdint.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// define linked list struct as per the CS50 short on linked lists
typedef struct sllist
{
    char *value;
    struct sllist *next;
} LIST_NODE;

// Prototypes
uint32_t generate_hash(const char *word);
int hash(uint32_t key, char *word);
void shift(LIST_NODE **head, char *value);
bool check(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);
int freelist(LIST_NODE *head);

#endif // DICTIONARY_H
