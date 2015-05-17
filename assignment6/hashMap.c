#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hashMap.h"

/*the first hashing function you can use*/
int stringHash1(char * str) {
    int i;
    int r = 0;
    for (i = 0; str[i] != '\0'; i++)
        r += str[i];
    return r;
}

/*the second hashing function you can use*/
int stringHash2(char * str) {
    int i;
    int r = 0;
    for (i = 0; str[i] != '\0'; i++)
        r += (i + 1) * str[i]; /*the difference between stringHash1 and stringHash2 is on this line*/
    return r;
}

/* initialize the supplied hashMap struct*/
void _initMap(struct hashMap * ht, int tableSize) {
    int index;
    if (ht == NULL)
        return;
    ht->table = (hashLink**) malloc(sizeof(hashLink*) * tableSize);
    ht->tableSize = tableSize;
    ht->count = 0;
    for (index = 0; index < tableSize; index++)
        ht->table[index] = NULL;
}

/* allocate memory and initialize a hash map*/
hashMap *createMap(int tableSize) {
    assert(tableSize > 0);
    hashMap *ht;
    ht = malloc(sizeof(hashMap));
    assert(ht != 0);
    _initMap(ht, tableSize);
    return ht;
}

/*
 Free all memory used by the buckets.
 Note: Before freeing up a hashLink, free the memory occupied by key and value
 */
void _freeMap(struct hashMap * ht) {
    /* test for valid pointer */
    assert(ht->table != 0);

    int bucket;
    hashLink *next;

    /* loop through each bucket or until size is 0 */
    for (bucket = 0; ht->count > 0 && bucket < ht->tableSize; ++bucket) {
        /* delete each link in this bucket */
        while (ht->table[bucket] != 0) {
            next = ht->table[bucket]->next;
            free(ht->table[bucket]->key); /* free key */
            free(ht->table[bucket]); /* free hashLink */
            ht->table[bucket] = next;
            --ht->count;
        }
        /* set bucket to null */
        ht->table[bucket] = 0;
    }
    /* set tableSize to 0 and free the table of hashLinks */
    ht->tableSize = 0;
    free(ht->table);
}

/* Deallocate buckets and the hash map.*/
void deleteMap(hashMap *ht) {
    assert(ht != 0);
    /* Free all memory used by the buckets */
    _freeMap(ht);
    /* free the hashMap struct */
    free(ht);
}

/* 
 Resizes the hash table to be the size newTableSize
 */
void _setTableSize(struct hashMap * ht, int newTableSize) {
    /* test for valid table size */
    assert(newTableSize > ht->tableSize);

    /* allocate table with new size */
    hashLink **newTable = malloc(sizeof(hashLink *) * newTableSize);
    assert(newTable != 0);

    int bucket, newHash, newBucket, remaining;
    hashLink *next;

    /* zero-initialize newTable */
    for (bucket = 0; bucket < newTableSize; ++bucket) {
        newTable[bucket] = 0;
    }

    remaining = ht->count;

    /* loop through each bucket of old table, re-hash the keys, and move the 
     links to a bucket in the new table */
    for (bucket = 0; remaining > 0 && bucket < ht->tableSize; ++bucket) {
        while (ht->table[bucket] != 0) {
            /* conditional hashing based on HASHING_FUNCTION macro value */
#if HASHING_FUNCTION == 1
            newHash = stringHash1(ht->table[bucket]->key);
#elif HASHING_FUNCTION == 2
            newHash = stringHash2(ht->table[bucket]->key);
#endif
            /* find the new bucket */
            newBucket = newHash % newTableSize;
            /* save old next value */
            next = ht->table[bucket]->next;
            /* insert link in front of whatever is in the new bucket */
            ht->table[bucket]->next = newTable[newBucket];
            newTable[newBucket] = ht->table[bucket];
            /* remove link from old bucket */
            ht->table[bucket] = next;
            /* decrement links remaining */
            --remaining;
        }
    }
    /* make sure all links were copied */
    assert(remaining == 0);

    /* free the old table and set the hashMap to the new table */
    free(ht->table);
    ht->table = newTable;
    ht->tableSize = newTableSize;
}

/*
 insert the following values into a hashLink, you must create this hashLink but
 only after you confirm that this key does not already exist in the table. For example, you
 cannot have two hashLinks for the word "taco".
 
 if a hashLink already exists in the table for the key provided you should
 replace that hashLink--this requires freeing up the old memory pointed by hashLink->value
 and then pointing hashLink->value to value v.
 
 also, you must monitor the load factor and resize when the load factor is greater than
 or equal LOAD_FACTOR_THRESHOLD (defined in hashMap.h).
 */
void insertMap(struct hashMap * ht, KeyType k, ValueType v)
{
    /* test for valid hashMap and key */
    assert(ht != 0);
    assert(k != 0);
    assert(strlen(k) > 0);

    int hash, bucket;
    hashLink *lnk;

    /* conditional hashing based on HASHING_FUNCTION macro value */
#if HASHING_FUNCTION == 1
    hash = stringHash1(k);
#elif HASHING_FUNCTION == 2
    hash = stringHash2(k);
#endif

    /* get current link (if any) in the bucket */
    bucket = hash % capacity(ht);
    lnk = ht->table[bucket];
    while (lnk != 0) {
        if (strcmp(lnk->key, k) == 0) {
            lnk->value = v; /* can't free an int, so just reassign */
            return;
        }
        lnk = lnk->next;
    }
    /* if we get here, the key was not already in the bucket.
     allocate a new one and insert it into the bucket */
    lnk = malloc(sizeof(hashLink));
    assert(lnk != 0);
    lnk->key = k;
    lnk->value = v;
    lnk->next = ht->table[bucket];
    ht->table[bucket] = lnk;
    ++ht->count;

    /* see if rebalancing is needed */
    if (tableLoad(ht) >= LOAD_FACTOR_THRESHOLD)
    _setTableSize(ht, capacity(ht) * 2);
}

/*
 this returns the value (which is void*) stored in a hashLink specified by the key k.
 
 if the user supplies the key "taco" you should find taco in the hashTable, then
 return the value member of the hashLink that represents taco.
 
 if the supplied key is not in the hashtable return NULL.
 */
ValueType* atMap(struct hashMap * ht, KeyType k)
{
    /* test for valid hashMap and key */
    assert(ht != 0);
    assert(k != 0);
    assert(strlen(k) > 0);

    int hash;
    hashLink *lnk;

    /* conditional hashing based on HASHING_FUNCTION macro value */
#if HASHING_FUNCTION == 1
    hash = stringHash1(k);
#elif HASHING_FUNCTION == 2
    hash = stringHash2(k);
#endif

    /* check bucket for key */
    lnk = ht->table[hash % capacity(ht)];
    while (lnk != 0) {
        if (strcmp(lnk->key, k) == 0)
        return &lnk->value;
        lnk = lnk->next;
    }

    /* key not found */
    return NULL;
}

/*
 a simple yes/no if the key is in the hashtable. 
 0 is no, all other values are yes.
 */
int containsKey(struct hashMap * ht, KeyType k)
{
    /* test for valid hashMap and key */
    assert(ht != 0);
    assert(k != 0);
    assert(strlen(k) > 0);

    int hash;
    hashLink *lnk;

    /* conditional hashing based on HASHING_FUNCTION macro value */
#if HASHING_FUNCTION == 1
    hash = stringHash1(k);
#elif HASHING_FUNCTION == 2
    hash = stringHash2(k);
#endif

    /* check bucket for key */
    lnk = ht->table[hash % capacity(ht)];
    while (lnk != 0) {
        if (strcmp(lnk->key, k) == 0)
        return 1;
        lnk = lnk->next;
    }

    /* key not found */
    return 0;
}

/*
 find the hashlink for the supplied key and remove it, also freeing the memory
 for that hashlink. it is not an error to be unable to find the hashlink, if it
 cannot be found do nothing (or print a message) but do not use an assert which
 will end your program.
 */
void removeKey(struct hashMap * ht, KeyType k)
{
    /* test for valid hashMap */
    assert(ht != 0);
}

/*
 returns the number of hashLinks in the table
 */
int size(struct hashMap *ht) {
    /* test for valid hashMap */
    assert(ht != 0);

    return ht->count;
}

/*
 returns the number of buckets in the table
 */
int capacity(struct hashMap *ht) {
    /* test for valid hashMap */
    assert(ht != 0);

    return ht->tableSize;
}

/*
 returns the number of empty buckets in the table, these are buckets which have
 no hashlinks hanging off of them.
 */
int emptyBuckets(struct hashMap *ht) {
    /* test for valid hashMap */
    assert(ht != 0);

    int i, used = 0;
    for (i = 0; i < ht->tableSize; ++i)
        if (ht->table[i] != 0)
            ++used;

    return ht->tableSize - used;
}

/*
 returns the ratio of: (number of hashlinks) / (number of buckets)
 
 this value can range anywhere from zero (an empty table) to more then 1, which
 would mean that there are more hashlinks then buckets (but remember hashlinks
 are like linked list nodes so they can hang from each other)
 */
float tableLoad(struct hashMap *ht) {
    /* test for valid hashMap */
    assert(ht != 0);

    return ((float) ht->count) / ht->tableSize;
}

void printMap(struct hashMap * ht) {
    int i;
    struct hashLink *temp;
    for (i = 0; i < capacity(ht); i++) {
        temp = ht->table[i];
        if (temp != 0) {
            printf("\nBucket Index %d -> ", i);
        }
        while (temp != 0) {
            printf("Key:%s|", temp->key);
            printValue(temp->value);
            printf(" -> ");
            temp = temp->next;
        }
    }
}
