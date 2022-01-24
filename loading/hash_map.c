#include "hash_map.h"

int hash_map_create(
    hash_map *map, 
    size_t size, 
    size_t (*hash_func)(HM_KEY),
    int (*key_cmp)(HM_KEY, HM_KEY))
{
    LOG("Creating hash map: ");
    hash_node **entries = calloc(size, sizeof *entries);
    if(entries == NULL)
        return 0;

    *map = (hash_map) {
        .size = size,
        .hash_func = hash_func,
        .key_cmp = key_cmp,
        .entries = entries
    };

    return 1;
}




void hash_map_free(hash_map *map)
{
    for(size_t i = 0; i < map->size; i++)
        if(map->entries[i])
            _hash_node_free(map->entries[i]);

    free(map->entries);
}




int hash_map_insert(
    hash_map *map, 
    HM_KEY key, 
    HM_VALUE value)
{
    size_t index = map->hash_func(key) % map->size;

    if(map->entries[index] != NULL)
    {
        hash_node *existing_node = map->entries[index];

        if(map->key_cmp(key, existing_node->key))
        {
        #if HM_FREE_VALUES
            free(existing_node->value);
        #endif
            existing_node->value = value;

            return 1;
        }

        while(existing_node->next != NULL)
        {
            existing_node = existing_node->next;

            if(map->key_cmp(key, existing_node->key))
            {
            #if HM_FREE_VALUES
                free(existing_node->value);
            #endif
                existing_node->value = value;

                return 1;
            }
        }

        hash_node *insert_node = malloc(sizeof *insert_node);
        if(insert_node == NULL)
            return 0;
        

        *insert_node = (hash_node) {
            .key = key,
            .value = value,
            .next = NULL
        };

        existing_node->next = insert_node;
        return 1;
    }
    else
    {
        hash_node *insert_node = malloc(sizeof *insert_node);
        if(insert_node == NULL)
            return 0;

        *insert_node = (hash_node) {
            .key = key,
            .value = value,
            .next = NULL
        };

        map->entries[index] = insert_node;
        return 1;
    }

    return 0;
}




int hash_map_find(
    hash_map *map, 
    HM_KEY key, 
    HM_VALUE *value_out)
{
    size_t index = map->hash_func(key) % map->size;

    if(map->entries[index] != NULL)
    {
        hash_node *node = map->entries[index];

        if(map->key_cmp(key, node->key))
        {
            *value_out = node->value;
            return 1;
        }

        while(node->next != NULL)
        {
            node = node->next;

            if(map->key_cmp(key, node->key))
            {
                *value_out = node->value;
                return 1;
            }
        }

        return 0;
    }
    else
    {
        return 0;
    }

    return 0;
}




void _hash_node_free(hash_node *node)
{
#if HM_FREE_KEYS
    free(node->key);
#endif

#if HM_FREE_VALUES
    free(node->value);
#endif

    if(node->next)
        _hash_node_free(node->next);

    free(node);
}