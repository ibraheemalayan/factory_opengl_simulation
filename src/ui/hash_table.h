#ifndef __UI_HASH_TABLE_H_
#define __UI_HASH_TABLE_H_

#include "./structs.h"
#include "../include.h"

Item *items[500];

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 1000 // Size of the Hash Table

unsigned long hash_function(int k)
{
    return k % CAPACITY;
}

typedef struct Ht_item Ht_item;

// Define the Hash Table Item here
struct Ht_item
{
    int key;
    Item *value;
};

typedef struct LinkedList LinkedList;

// Define the Linkedlist here
struct LinkedList
{
    Ht_item *item;
    LinkedList *next;
};

typedef struct HashTable HashTable;

// Define the Hash Table here
struct HashTable
{
    // Contains an array of pointers
    // to items
    Ht_item **items;
    LinkedList **overflow_buckets;
    int size;
    int count;
};

static LinkedList *allocate_list()
{
    // Allocates memory for a Linkedlist pointer
    LinkedList *list = (LinkedList *)calloc(1, sizeof(LinkedList));
    return list;
}

static LinkedList *linkedlist_insert(LinkedList *list, Ht_item *item)
{
    // Inserts the item onto the Linked List
    if (!list)
    {
        LinkedList *head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    }

    else if (list->next == NULL)
    {
        LinkedList *node = allocate_list();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    LinkedList *temp = list;
    while (temp->next)
    {
        temp = temp->next;
    }

    LinkedList *node = allocate_list();
    node->item = item;
    node->next = NULL;
    temp->next = node;

    return list;
}

static Ht_item *linkedlist_remove(LinkedList *list)
{
    // Removes the head from the linked list
    // and returns the item of the popped element
    if (!list)
        return NULL;
    if (!list->next)
        return NULL;
    LinkedList *node = list->next;
    LinkedList *temp = list;
    temp->next = NULL;
    list = node;
    Ht_item *it = NULL;
    memcpy(temp->item, it, sizeof(Ht_item));
    free(temp->item->value);
    free(temp->item);
    free(temp);
    return it;
}

static void free_linkedlist(LinkedList *list)
{
    LinkedList *temp = list;
    if (!list)
        return;
    while (list)
    {
        temp = list;
        list = list->next;
        free(temp->item->value);
        free(temp->item);
        free(temp);
    }
}

static LinkedList **create_overflow_buckets(HashTable *table)
{
    // Create the overflow buckets; an array of linkedlists
    LinkedList **buckets = (LinkedList **)calloc(table->size, sizeof(LinkedList *));
    for (int i = 0; i < table->size; i++)
        buckets[i] = NULL;
    return buckets;
}

static void free_overflow_buckets(HashTable *table)
{
    // Free all the overflow bucket lists
    LinkedList **buckets = table->overflow_buckets;
    for (int i = 0; i < table->size; i++)
        free_linkedlist(buckets[i]);
    free(buckets);
}

Ht_item *create_item(int key, Item *value)
{
    // Creates a pointer to a new hash table item
    Ht_item *item = (Ht_item *)malloc(sizeof(Ht_item));
    item->key = key;
    item->value = value;

    return item;
}

HashTable *create_table(int size)
{
    // Creates a new HashTable
    HashTable *table = (HashTable *)malloc(sizeof(HashTable) + sizeof(Ht_item) * size);
    table->size = size;
    table->count = 0;

    table->items = (Ht_item **)calloc(table->size, sizeof(Ht_item *));

    // for (int i = 0; i < table->size; i++)
    //     table->items[i] = NULL;
    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}

void free_item(Ht_item *item)
{
    // Frees an item
    free(item->value);
    free(item);
}

void free_hashtable(HashTable *table)
{
    // Frees the table
    for (int i = 0; i < table->size; i++)
    {
        Ht_item *item = table->items[i];
        if (item != NULL)
            free_item(item);
    }

    free_overflow_buckets(table);
    free(table->items);
    free(table);
}

void handle_collision(HashTable *table, unsigned long index, Ht_item *item)
{
    LinkedList *head = table->overflow_buckets[index];

    if (head == NULL)
    {
        // We need to create the list
        head = allocate_list();
        head->item = item;
        table->overflow_buckets[index] = head;
        return;
    }
    else
    {
        // Insert to the list
        table->overflow_buckets[index] = linkedlist_insert(head, item);
        return;
    }
}

void ht_insert(HashTable *table, int key, Item *value)
{
    // Create the item
    Ht_item *item = create_item(key, value);

    // Compute the index
    int index = hash_function(key);

    Ht_item *current_item = table->items[index];

    if (current_item == NULL)
    {
        // Key does not exist.
        if (table->count == table->size)
        {

            red_stdout();
            // Hash Table Full
            printf("Insert Error: Hash Table is full\n");
            // Remove the created item
            free_item(item);

            reset_stdout();

            exit(1);
        }

        // Insert directly
        table->items[index] = item;
        table->count++;
    }

    else
    {

        // Scenario 1: We only need to update value
        if (current_item->key == key)
        {
            printf("Key %d exists\n", key);

            free(table->items[index]->value);
            table->items[index]->value = value;
            free_item(item);
            return;
        }

        else
        {
            printf("Key %d collides\n", key);

            // Scenario 2: Collision
            handle_collision(table, index, item);
            return;
        }
    }
}

Item *ht_search(HashTable *table, int key)
{
    // Searches the key in the hashtable
    // and returns NULL if it doesn't exist
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    // Ensure that we move to items which are not NULL
    while (item != NULL)
    {
        if (item->key == key)
            return item->value;
        if (head == NULL)
            return NULL;
        item = head->item;
        head = head->next;
    }
    return NULL;
}

void ht_delete(HashTable *table, int key)
{
    // Deletes an item from the table
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    if (item == NULL)
    {
        // Does not exist. Return
        return;
    }
    else
    {
        if (head == NULL && item->key == key)
        {
            // No collision chain. Remove the item
            // and set table index to NULL
            table->items[index] = NULL;
            free_item(item);
            table->count--;
            return;
        }
        else if (head != NULL)
        {
            // Collision Chain exists
            if (item->key == key)
            {
                // Remove this item and set the head of the list
                // as the new item

                free_item(item);
                LinkedList *node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key, node->item->value);
                free_linkedlist(node);
                table->overflow_buckets[index] = head;
                return;
            }

            LinkedList *curr = head;
            LinkedList *prev = NULL;

            while (curr)
            {
                if (curr->item->key == key)
                {
                    if (prev == NULL)
                    {
                        // First element of the chain. Remove the chain
                        free_linkedlist(head);
                        table->overflow_buckets[index] = NULL;
                        return;
                    }
                    else
                    {
                        // This is somewhere in the chain
                        prev->next = curr->next;
                        curr->next = NULL;
                        free_linkedlist(curr);
                        table->overflow_buckets[index] = head;
                        return;
                    }
                }
                curr = curr->next;
                prev = curr;
            }
        }
    }
}

void print_search(HashTable *table, int key)
{
    Item *val;
    if ((val = ht_search(table, key)) == NULL)
    {
        printf("%d does not exist\n", key);
        return;
    }
    else
    {
        printf("Key:%d, Value: ", key);
        print_item(val);
    }
}

void print_hashtable(HashTable *table)
{
    printf("\n-------------------\n");
    for (int i = 0; i < table->size; i++)
    {
        if (table->items[i])
        {
            printf("Index:%d, Key:%d, Value: ", i, table->items[i]->key);
            print_item(table->items[i]->value);

            if (table->overflow_buckets[i])
            {
                printf(" => Overflow Bucket => ");
                LinkedList *head = table->overflow_buckets[i];
                while (head)
                {
                    printf("Key:%d, Value: ", head->item->key);
                    print_item(table->items[i]->value);
                    head = head->next;
                }
            }
            printf("\n");
        }
    }
    printf("-------------------\n");
}

#endif
