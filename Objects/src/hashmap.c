//
// Created by Kevin on 05/07/2022.
//

//TODO: Fix sur la HashMap qui cause la supression de donnnes en particulier sur les unboundedInt
//TODO fix HASHMAP
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "exec_error.h"
#include "hashmap.h"

#include "lib.h"

enum Type {
	HASHMAP_NONE,
	HASHMAP_DUMMY,
	HASHMAP_FULL
};

enum Type;

struct HashMapData {
	size_t mHash;
	char *mName;
	void *mData;
	enum Type mType;
};

#define DUMMY_DATA(h) ((HashMapData) {.mName = "", .mHash = (h), .mData = NULL, .mType = HASHMAP_DUMMY})
#define NONE_DATA ((HashMapData) {.mName = "", .mHash = 0, .mData = NULL, .mType = HASHMAP_NONE})
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
		if (map->mData[ind].mType == HASHMAP_NONE) {
			if (dummy == dummyInit) dummy = ind;
			break;
		}
		if (map->mData[ind].mType == HASHMAP_DUMMY) {
			if (dummy == dummyInit) dummy = ind;
		}
		else if (map->mData[ind].mHash == hashVal && str_equals(name, map->mData[ind].mName)) {
			return ind;
		}
		ind = (ind + step) % map->mCapacity;
	}
	return (flag) ? dummy : dummyInit;
}


HashMap *HashMap_new(int objectSize, int (*cmpData)(const char *, void *), void (*freeData)(void *)) {
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
	map->mFree = freeData;
	map->mObjectSize = objectSize;
	map->spare = NULL;
	return map;
}

void *HashMap_put(HashMap *map, char *name, void *value) {
	/*size_t hashVal = hash(name);
	size_t empty = find(map, name, 1);
	if (empty == map->mCapacity + 1) return NULL;
	if (map->mData[empty].mType == HASHMAP_DUMMY) {
		map->mDummyNumber--;
		map->mKeyNumber++;
	}
	if (map->mData[empty].mType == HASHMAP_NONE) {
		map->mKeyNumber++;
	}
	if (map->mData[empty].mData != NULL) {
		map->spare = map->mData[empty].mData;
	}
	else {
		map->spare = NULL;
	}
	void *data = malloc(sizeof(map->mObjectSize));
	if (data == NULL) return 0;
	void *tmp = memcpy(data, value, map->mObjectSize);
	if (tmp == NULL) {
		free(data);
		return NULL;
	}
	HashMapData new = {.mName = name, .mData = data, .mType = HASHMAP_FULL, .mHash = hashVal};
	map->mData[empty] = new;
	if (map->mDummyNumber + map->mKeyNumber >= (size_t) ((double) map->mCapacity * map->mMaxRatio)) {
		int ratio = (map->mDummyNumber < map->mKeyNumber) ? 2 : 1;
		resize(map, ratio * map->mCapacity);
	}
	return map->spare;*/
	return NULL;
}

void *HashMap_get(HashMap *map, char *name) {
	size_t pos = find(map, name, 0);
	if (pos == map->mCapacity + 1) return NULL;
	//return map->mData[pos].mData;
	return NULL;
}

static void resize(HashMap *map, size_t newSize) {
	HashMapData *old = map->mData;
	size_t oldLen = map->mCapacity;
	HashMapData *new = malloc(newSize * sizeof(HashMapData));
	for (size_t i = 0; i < newSize; i++) {
		new[i] = NONE_DATA;
	}
	if (new == NULL) {
		perror_src("");
		return;
	}
	map->mData = new;
	map->mCapacity = newSize;
	map->mKeyNumber = 0;
	map->mDummyNumber = 0;
	for (size_t i = 0; i < oldLen; i++) {
		if (old[i].mType != HASHMAP_NONE && old[i].mType != HASHMAP_DUMMY) {
			HashMap_put(map, old[i].mName, old[i].mData);
		}
	}
	free(old);
}

int HashMap_remove(HashMap *map, char *name) {		//TODO: fix Donnees corrompues
	long long pos = find(map, name, 0);
	if (pos == -1) return -1;
	HashMapData del = map->mData[pos];
	map->mData[pos] = DUMMY_DATA(del.mHash);
	if (del.mType == HASHMAP_FULL) map->mFree(&del);
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
		if (data.mType == HASHMAP_FULL) {
			map->mFree(&data);
		}
	}
	free(map);
}
