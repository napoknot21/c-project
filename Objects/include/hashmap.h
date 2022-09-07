//
// Created by Kevin on 05/07/2022.
//

#ifndef C_PROJECT_HASHMAP_H
#define C_PROJECT_HASHMAP_H

#include <stdlib.h>

typedef struct HashMap HashMap;
typedef struct HashMapData HashMapData;

struct HashMap {
	HashMapData **mData;
	size_t mCapacity;
	size_t mKeyNumber;
	size_t mDummyNumber;
	float mMinRatio, mMaxRatio;

	void (*mFree)(void *val);

	void *(*mCpy)(void *dst, void *src, size_t size);
};

int HashMap_get(HashMap *map, char *name, void *dst);

HashMap *HashMap_new(void (*freeData)(void *), void *(*cpy)(void *, void *, size_t));

int HashMap_put(HashMap *map, char *name, void *value, size_t size);

void HashMap_free(HashMap *map);

int HashMap_remove(HashMap *map, char *name);

#endif //C_PROJECT_HASHMAP_H
