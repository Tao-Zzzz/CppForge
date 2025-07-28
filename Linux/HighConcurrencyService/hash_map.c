//
// Created by ubuntu on 25-5-14.
//
#include "hash_map.h"

// djb2 hash function
static uint64_t hash_djb2(const char *str)
{
    uint64_t hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + (uint8_t)c;
    }
    return hash;
}

// Create a new hash map with given number of buckets
hashmap_t *hashmap_create(size_t bucket_count)
{
    hashmap_t *map = malloc(sizeof(hashmap_t));
    if (!map)
        return NULL;
    map->bucket_count = bucket_count;
    map->buckets = calloc(bucket_count, sizeof(hashmap_entry_t *));
    if (!map->buckets)
    {
        free(map);
        return NULL;
    }
    return map;
}

// Insert or update a key-value pair
int hashmap_put(hashmap_t *map, const char *key, void *value)
{
    if (!map || !key)
        return -1;
    uint64_t hash = hash_djb2(key);
    size_t idx = hash % map->bucket_count;
    hashmap_entry_t *entry = map->buckets[idx];

    // Check if key exists
    while (entry)
    {
        if (strcmp(entry->key, key) == 0)
        {
            entry->value = value;
            return 0;
        }
        entry = entry->next;
    }

    // Create new entry
    entry = malloc(sizeof(hashmap_entry_t));
    if (!entry)
        return -1;
    entry->key = strdup(key);
    if (!entry->key)
    {
        free(entry);
        return -1;
    }
    entry->value = value;
    // Insert at head of list
    entry->next = map->buckets[idx];
    map->buckets[idx] = entry;
    return 0;
}

// Retrieve value by key, or NULL if not found
void *hashmap_get(hashmap_t *map, const char *key)
{
    if (!map || !key)
        return NULL;
    uint64_t hash = hash_djb2(key);
    size_t idx = hash % map->bucket_count;
    hashmap_entry_t *entry = map->buckets[idx];
    while (entry)
    {
        if (strcmp(entry->key, key) == 0)
        {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

// Remove entry by key, return 0 if removed, -1 if not found
int hashmap_remove(hashmap_t *map, const char *key)
{
    if (!map || !key)
        return -1;
    uint64_t hash = hash_djb2(key);
    size_t idx = hash % map->bucket_count;
    hashmap_entry_t *entry = map->buckets[idx];
    hashmap_entry_t *prev = NULL;
    while (entry)
    {
        if (strcmp(entry->key, key) == 0)
        {
            if (prev)
                prev->next = entry->next;
            else
                map->buckets[idx] = entry->next;
            free(entry->key);
            free(entry);
            return 0;
        }
        prev = entry;
        entry = entry->next;
    }
    return -1;
}

// Destroy hash map, free all memory
void hashmap_destroy(hashmap_t *map)
{
    if (!map)
        return;
    for (size_t i = 0; i < map->bucket_count; i++)
    {
        hashmap_entry_t *entry = map->buckets[i];
        while (entry)
        {
            hashmap_entry_t *next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    free(map->buckets);
    free(map);
}

// 一个简单的整数哈希：用“32 位整型混合”算法
static uint64_t hash_int(int key)
{
    uint32_t x = (uint32_t)key;
    // Robert Jenkins’ 32 bit integer mix
    x = (x + 0x7ed55d16) + (x << 12);
    x = (x ^ 0xc761c23c) ^ (x >> 19);
    x = (x + 0x165667b1) + (x << 5);
    x = (x + 0xd3a2646c) ^ (x << 9);
    x = (x + 0xfd7046c5) + (x << 3);
    x = (x ^ 0xb55a4f09) ^ (x >> 16);
    return (uint64_t)x;
}

// 插入整数键
int hashmap_put_int(hashmap_t *map, int key, void *value)
{
    if (!map)
        return -1;
    uint64_t hash = hash_int(key);
    size_t idx = hash % map->bucket_count;

    // 以下与 hashmap_put() 类似，只不过不用处理 key 字符串复制
    hashmap_entry_t *e = map->buckets[idx];
    while (e)
    {
        // 这里需要把 entry->key 改为存 int，比如用额外字段或直接把 key 变成 void*
        // 如果你想在 entry 中直接保存 int key，可以把 key 字段改为 int key;
        // 下面假设 entry->int_key 存了原 int
        if (e->int_key == key)
        {
            e->value = value;
            return 0;
        }
        e = e->next;
    }

    // 新节点
    e = malloc(sizeof(*e));
    if (!e)
        return -1;
    e->int_key = key; // 存下整数 key
    e->value = value;
    e->key = NULL;
    e->next = map->buckets[idx];
    map->buckets[idx] = e;
    return 0;
}

// 查询整数键
void *hashmap_get_int(hashmap_t *map, int key)
{
    if (!map)
        return NULL;
    uint64_t hash = hash_int(key);
    size_t idx = hash % map->bucket_count;
    for (hashmap_entry_t *e = map->buckets[idx]; e; e = e->next)
    {
        if (e->int_key == key)
            return e->value;
    }
    return NULL;
}

// Remove by integer key
int hashmap_remove_int(hashmap_t *map, int key)
{
    if (!map)
        return -1;
    uint64_t hash = hash_int(key);
    size_t idx = hash % map->bucket_count;

    hashmap_entry_t *e = map->buckets[idx];
    hashmap_entry_t *prev = NULL;
    while (e)
    {
        if (e->int_key == key)
        {
            // 从链表摘除
            if (prev)
                prev->next = e->next;
            else
                map->buckets[idx] = e->next;
            free(e);
            return 0;
        }
        prev = e;
        e = e->next;
    }
    return -1; // 没找到
}
