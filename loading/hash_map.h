#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdlib.h>
#include <stdio.h>

#include "../materials/material.h"

#define LOG(msg) /*printf(msg)*/


/* TYPES ------------------------------------ */
#define HM_KEY char *
#define HM_VALUE material_t *

/* 
    WARNING: If HM_KEY and/or HM_VALUE are pointer types,
    and the hash_map is considered in ownership of the resources pointed to,
    HM_FREE_KEYS and HM_FREE_VALUES must be set to 1 respectively
*/

#define HM_FREE_KEYS 1
#define HM_FREE_VALUES 0

/* ------------------------------------------ */


typedef struct hash_node hash_node;
typedef struct hash_node {
    HM_KEY key;
    HM_VALUE value;
    hash_node *next;
} hash_node;

typedef struct hash_map {
    size_t size;
    size_t (*hash_func)(HM_KEY);
    int (*key_cmp)(HM_KEY,HM_KEY);

    hash_node **entries;
} hash_map;


int hash_map_create(
    hash_map *map, size_t size, 
    size_t (*hash_func)(HM_KEY), 
    int (*key_cmp)(HM_KEY, HM_KEY));

void hash_map_free(hash_map *map);

int hash_map_insert(
    hash_map *map, 
    HM_KEY key, 
    HM_VALUE value);

int hash_map_find(
    hash_map *map, 
    HM_KEY key, 
    HM_VALUE *value_out);

void _hash_node_free(hash_node *node);

#endif