//
// Created by Kevin on 05/07/2022.
//

#ifndef C_PROJECT_HASHMAP_H
#define C_PROJECT_HASHMAP_H

#include <stdlib.h>

typedef struct HashMap HashMap;
typedef struct HashMapData HashMapData;

struct HashMap {
    HashMapData *mData;
    size_t mCapacity;
    size_t mKeyNumber;
    size_t mDummyNumber;
    float mMinRatio, mMaxRatio;

    int (*mCmp)(const char *, const void *);

    void (*mFree)(void *);
};

void *HashMap_get(HashMap *map, char *name);

HashMap *HashMap_new(int (*cmpData)(void *, void *), void (*freeData)(void *));

int HashMap_put(HashMap *map, char *name, void *value);

void HashMap_free(HashMap *map);

int HashMap_remove(HashMap *map, char *name);

#endif //C_PROJECT_HASHMAP_H
