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
	void *spare;
	size_t mCapacity;
	size_t mKeyNumber;
	size_t mDummyNumber;
	size_t mObjectSize;
	float mMinRatio, mMaxRatio;

	int (*mCmp)(const char *, void *);

	void (*mFree)(void *);
};

void *HashMap_get(HashMap *map, char *name);

HashMap *HashMap_new(int objectSize, int (*cmpData)(const char *, void *), void (*freeData)(void *));

void *HashMap_put(HashMap *map, char *name, void *value);

void HashMap_free(HashMap *map);

int HashMap_remove(HashMap *map, char *name);

#endif //C_PROJECT_HASHMAP_H
