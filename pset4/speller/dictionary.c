// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 28

// Global variables
unsigned int wordCount = 0;
int gate = 0;

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

node *clearNode (node *parent);
bool hasChild(node *parent);

// Represents a trie
node *root;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    node *cursor = root;

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        for (int i = 0; i < strlen(word); i++)
        {
            if (i == 0)
            {
                cursor = root;
            }

            gate = hash(word[i]);

            if(cursor->children[gate] == NULL)
            {
                cursor->children[gate] = createNewNode();
            }

            cursor = cursor->children[gate];
            if (i == strlen(word) - 1)
            {
                cursor->is_word = true;
                wordCount++;
            }
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return wordCount;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    char tmp[LENGTH + 1];
    node *cursor = root;
    bool verified = false;

    for (int i = 0; i < strlen(word) + 1; i++)
    {
        tmp[i] = word[i];
    }

    for (int i = 0; i < strlen(word); i++)
    {
        gate = hash(tmp[i]);
        if (cursor->children[gate] == NULL )
        {
            return false;
        }
        cursor = cursor->children[gate];
    }

    verified = cursor->is_word;
    if (verified == true)
    {
        return true;
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    bool inProgress = true;
    while (inProgress == true)
    {
        node *cursor = root;
        node *target = root;
        bool end = hasChild(cursor);

        while (end == true)
        {
            bool next = hasChild(target);

            if (next == true)
            {
                cursor = target;
                target = target->children[gate];
            }
            else
            {
                hasChild(cursor);
                int i = gate;
                clearNode(cursor);
                end = false;
            }
            inProgress = hasChild(root);

            if (inProgress == false)
            {
                free(root);
                return true;
            }
        }
    }
    return false;
}

bool endOfString(char c)
{
    char end = '\0';
    char value = c;

    if (value != end)
    {
        return false;
    }

    return true;
}

int hash(char letter)
{
    int code = -1;
    bool isLetter = isalpha(letter);
    char tmp = letter;

    if (isLetter == true)
    {
        tmp = tolower(tmp);
        code = tmp - 'a';
        return code;
    }
    code = N - 1;
    return code;
}

// Create a new node
struct node *createNewNode()
{
    node *child = (node*)malloc(sizeof(node));

    child->is_word = false;

    for (int i = 0; i < N; i++)
    {
        child->children[i] = NULL;
    }

    return child;
}

// Check if the node has a child
bool hasChild(node *parent)
{
    for (int i = 0; i < N - 1; i++)
    {
        gate = i;
        if (parent->children[i] != NULL)
        {
            return true;
        }
    }
    return false;
}

node *clearNode (node *parent)
{
    int target = gate;
    node *child = parent->children[target];
    if (child->is_word == true)
    {
        child->is_word = false;
        wordCount--;
    }
    bool loop = hasChild(child);

    if (loop != false)
    {
        clearNode(child);
    }

    free(child);
    parent->children[target] = NULL;

    return parent;
}
