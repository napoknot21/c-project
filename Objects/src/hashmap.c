//
// Created by Kevin on 05/07/2022.
//
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "exec_error.h"
#include "hashmap.h"

enum Type {
    NONE, DUMMY, FULL
};

enum Type;
struct HashMapData {
    size_t mHash;
    char *mName;
    void *mData;
    enum Type mType;
};

#define DUMMY_DATA(h) ((HashMapData) {.mName = "", .mHash = (h), .mData = NULL, .mType = DUMMY})
#define NONE_DATA ((HashMapData) {.mName = "", .mHash = 0, .mData = NULL, .mType = NONE})
#define HASHMAP_INITIAL_SIZE 11
#define HASHMAP_MIN_RATIO ((float)0.25)
#define HASHMAP_MAX_RATIO ((float)0.75)
#define HASH_A ((sqrt(5)-1)/2)

static void resize(HashMap *map, size_t newSize);

static size_t hash(const char *name) {
    size_t value = 0;
    size_t len = strlen(name);
    for (size_t i = 0; i < len; i++) {
        value += (size_t) (name[i] * pow(31, len - i));
    }
    return value;
}

static size_t hash1(double capacity, size_t hash) {
    return (size_t) ((double) hash / HASH_A * capacity);
}

static size_t hash2(size_t hash) {
    return (2 * hash + 1) % LLONG_MAX;
}

static size_t find(HashMap *map, char *name, int flag) {
    size_t hashVal = hash(name);
    size_t ind = hash1((double) map->mCapacity, hashVal) % map->mCapacity;
    size_t step = hash2(hashVal);
    size_t dummyInit = map->mCapacity + 1;
    size_t dummy = dummyInit;

    for (size_t i = 0; i < map->mCapacity; i++) {
        if (map->mData[ind].mType == NONE) {
            if (dummy == dummyInit) dummy = ind;
            break;
        }
        if (map->mData[ind].mType == DUMMY) {
            if (dummy == dummyInit) dummy = ind;
        }
        else if (map->mData[ind].mHash == hashVal && map->mCmp(name, map->mData[ind].mData)) {
            return ind;
        }
        ind = (ind + step) % map->mCapacity;
    }
    return (flag) ? dummy : -1;
}


HashMap *HashMap_new(int (*cmpData)(void *, void *), void (*freeData)(void *)) {
    HashMap *map = malloc(sizeof(HashMap));
	if (map == NULL) {
        perror_src("");
        return NULL;
    }
    map->mMinRatio = HASHMAP_MIN_RATIO;
    map->mMaxRatio = HASHMAP_MAX_RATIO;
    map->mCapacity = HASHMAP_INITIAL_SIZE;
    map->mData = malloc(sizeof(HashMapData) * map->mCapacity);
    for (size_t i = 0; i < map->mCapacity; i++) {
        map->mData[i] = NONE_DATA;
    }
    if (map->mData == NULL) {
        perror_src("");
        free(map);
        return NULL;
        
    }
    map->mKeyNumber = 0;
    map->mDummyNumber = 0;
    map->mCmp = cmpData;
    map->mFree = freeData
    return map;
}

int HashMap_put(HashMap *map, char *name, void *value) {
    long long hashVal = hash(name);
    long long empty = find(map, name, 1);
    if (empty == -1) return 0;

    HashMapData new = {.mName = name, .mData = value, .mType = FULL, .mHash = hashVal};
    if (map->mData[empty].mType == DUMMY) {
        map->mDummyNumber--;
        map->mKeyNumber++;
    }
    if (map->mData[empty].mType == NONE) {
        map->mKeyNumber++;
    }
    map->mData[empty] = new;
    if (map->mDummyNumber + map->mKeyNumber >= (size_t) ((double) map->mCapacity * map->mMaxRatio)) {
        int ratio = (map->mDummyNumber < map->mKeyNumber)? 2 : 1;
        resize(map, ratio * map->mCapacity);
    }
    return 1;
}

void *HashMap_get(HashMap *map, char *name) {
    long long pos = find(map, name, 0);
    if (pos == -1) return NULL;
    return map->mData[pos].mData;
}

static void resize(HashMap *map, size_t newSize) {
    HashMapData *old = map->mData;
    size_t oldLen = map->mCapacity;
    HashMapData *new = malloc(newSize * sizeof(HashMapData));
    for (int i = 0; i < newSize; i++) new[i] = NONE_DATA;
    if (new == NULL) {
        perror_src("");
        return;
    }
    map->mData = new;
    map->mCapacity = newSize;
    map->mKeyNumber = 0;
    map->mDummyNumber = 0;
    for (int i = 0; i < oldLen; i++) {
        if (old[i].mType != NONE && old[i].mType != DUMMY) HashMap_put(map, old[i].mName, old[i].mData);
    }
    free(old);
}

int HashMap_remove(HashMap *map, char *name) {
    long long pos = find(map, name, 0);
    if (pos == -1) return -1;
    HashMapData del = map->mData[pos];
    map->mData[pos] = DUMMY_DATA(del.mHash);
    if (del.mType == FULL) map->mFree(&del);
    map->mDummyNumber++;
    map->mKeyNumber--;
    if (map->mDummyNumber + map->mKeyNumber <= (size_t) ((double) map->mCapacity * map->mMinRatio)) {
        resize(map, (map->mCapacity / 2));
    }
    return 1;
}

void HashMap_free(HashMap *map) {
    if (map == NULL) return;
    for (size_t i = 0; i < map->mCapacity; i++) {
        HashMapData data = map->mData[i];
        if (data.mType == FULL) {
            map->mFree(&data);
        }
    }
    free(map);
}