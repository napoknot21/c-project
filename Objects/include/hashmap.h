//
// Created by Kevin on 05/07/2022.
//

#ifndef C_PROJECT_HASHMAP_H
#define C_PROJECT_HASHMAP_H
#include <stdlib.h>
typedef struct HashMap HashMap;
typedef struct HashMapData HashMapData;
enum Type {
    NONE, DUMMY, FULL
};

struct HashMapData {
    size_t mHash;
    char * mName;
    void* mData;
    enum Type mType;
};
struct HashMap {
    HashMapData *mData;
    size_t mCapacity;
    size_t mKeyNumber;
    size_t mDummyNumber;
    float mMinRatio, mMaxRatio;
    int (*cmp) (const char*, const void*);
    void (*mFree)(void*);
};

HashMapData hashMap_get(HashMap *map, char *name);

HashMap *HashMap_new();

int hashMap_put(HashMap *map, char* name, void* value);

void hashMap_free(HashMap *map);

int HashMap_remove(HashMap *map, char *name);
#endif //C_PROJECT_HASHMAP_H
