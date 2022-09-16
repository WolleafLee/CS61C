#include "hashtable.h"
#include <stdlib.h>
#include<string.h>

/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *)) {
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));
  newTable->size = size;
  newTable->data = malloc(sizeof(struct HashBucket *) * size);
  for (i = 0; i < size; ++i) {
    newTable->data[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/*
 * This inserts a key/data pair into a hash table.  To use this
 * to store strings, simply cast the char * to a void * (e.g., to store
 * the string referred to by the declaration char *string, you would
 * call insertData(someHashTable, (void *) string, (void *) string).
 * Because we only need a set data structure for this spell checker,
 * we can use the string as both the key and data.
 */
void insertData(HashTable *table, void *key, void *data) {
  // HINT:
  // 1. Find the right hash bucket location with table->hashFunction.
  // 2. Allocate a new hash bucket struct.
  // 3. Append to the linked list or create it if it does not yet exist. 
  unsigned int hashCode = table->hashFunction(key);
  int pos = hashCode % table->size;
  char* _key = malloc(sizeof(char) * strlen(key));
  char* _data = malloc(sizeof(char) * strlen(data));
  strcpy(_key, key);
  strcpy(_data, data);
  struct HashBucket *newNode = malloc(sizeof(struct HashBucket));
  newNode->key = _key;
  newNode->data = _data;
  newNode->next = NULL;
  if (table->data[pos] == NULL) {
    table->data[pos] = newNode;
  } else {
    struct HashBucket* temp = table->data[pos];
    while (temp->next != NULL) {
      if (table->equalFunction(temp->key, key)) {
        temp->data = data;
        return;
      }
      temp = temp->next;
    }
    temp->next = newNode;
  }
}

/*
 * This returns the corresponding data for a given key.
 * It returns NULL if the key is not found. 
 */
void *findData(HashTable *table, void *key) {
  // HINT:
  // 1. Find the right hash bucket with table->hashFunction.
  // 2. Walk the linked list and check for equality with table->equalFunction.
  unsigned int hashCode = table->hashFunction(key);
  int pos = hashCode % table->size;
  struct HashBucket* temp = table->data[pos];
  while (temp != NULL)
  {
    if (table->equalFunction(temp->key, key)) {
      return temp->data;
    }
    temp = temp->next;
  }
  return NULL;
}
