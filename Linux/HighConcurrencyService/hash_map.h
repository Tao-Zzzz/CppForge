//
// Created by ubuntu on 25-5-14.
//

#ifndef HASH_MAP_H
#define HASH_MAP_H
// Simple Hash Map in C (string keys, void* values)
// Separate chaining with linked lists, djb2 hash function
// Usage:
//   hashmap_t *map = hashmap_create(1024);
//   hashmap_put(map, "key1", (void*)value1);
//   void *val = hashmap_get(map, "key1");
//   hashmap_remove(map, "key1");
//   hashmap_destroy(map);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Hash map entry
typedef struct hashmap_entry
{
    char *key;
    int int_key;
    void *value;
    struct hashmap_entry *next;
} hashmap_entry_t;

// Hash map
typedef struct
{
    size_t bucket_count;
    hashmap_entry_t **buckets;
} hashmap_t;

// Create a new hash map with given number of buckets
extern hashmap_t *hashmap_create(size_t bucket_count);

// Insert or update a key-value pair
extern int hashmap_put(hashmap_t *map, const char *key, void *value);
// Retrieve value by key, or NULL if not found
extern void *hashmap_get(hashmap_t *map, const char *key);

// Remove entry by key, return 0 if removed, -1 if not found
extern int hashmap_remove(hashmap_t *map, const char *key);
// Destroy hash map, free all memory
extern void hashmap_destroy(hashmap_t *map);

extern void *hashmap_get_int(hashmap_t *map, int key);

extern int hashmap_put_int(hashmap_t *map, int key, void *value);

extern int hashmap_remove_int(hashmap_t *map, int key);

#endif // HASH_MAP_H
