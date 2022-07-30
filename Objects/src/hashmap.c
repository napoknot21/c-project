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
	long long mHash;
	char *mName;
	void *mData;
	enum Type mType;
	size_t mSize;
};

static HashMapData *HashMapData_new(size_t hash, char *name, void *value, size_t size);
#define NONE_DATA ((HashMapData) {.mName = "", .mHash = 0, .mData = NULL, .mType = HASHMAP_NONE})
#define HASHMAP_INITIAL_SIZE 11
#define HASHMAP_MIN_RATIO ((float)0.25)
#define HASHMAP_MAX_RATIO ((float)0.75)
#define HASH_A ((sqrt(5)-1)/2)

static void resize(HashMap *map, size_t newSize);

static HashMapData *dummy(long long hash) {
	HashMapData *dummy = HashMapData_new(hash, "", NULL, 0);
	dummy->mType = HASHMAP_DUMMY;
	return dummy;
}

static void *HashMapData_free(HashMapData *data, void (*f)(void *)) {
	if (data == NULL) {
		return NULL;
	}
	free(data->mName);
	data->mName = NULL;
	if (f != NULL && data->mType == HASHMAP_FULL) {
		f(data->mData);
		data->mData = NULL;
	}
	free(data);
	return NULL;
}

static HashMapData *HashMapData_new(long long hash, char *name, void *value, size_t size) {
	HashMapData *new = malloc(sizeof(HashMapData));
	if (new == NULL) {
		perror_src("");
		return NULL;
	}
	size_t len = strlen(name);
	new->mName = malloc(sizeof(char) * (len + 1));
	if (new->mName == NULL) {
		free(new->mData);
		free(new);
		perror_src("");
		return NULL;
	}
	char *tmp = strncpy(new->mName, name, len);
	if (tmp == NULL) {
		free(new->mData);
		free(new->mName);
		free(new);
		perror_src("");
		return NULL;
	}
	new->mName = tmp;
	new->mName[len] = '\0';
	new->mHash = hash;
	new->mType = HASHMAP_FULL;
	new->mSize = size;
	new->mData = value;
	return new;
}

static long long hash(const char *name) {
	long long value = 0;
	long long len = strlen(name);
	for (long long i = 0; i < len; i++) {
		value += (long long) (name[i] * pow(31, len - i));
	}
	return llabs(value);
}

static long long hash1(double capacity, long long hash) {
	return llabs((long long) ((double) hash / HASH_A * capacity));
}

static long long hash2(long long hash) {
	return llabs(2 * hash + 1);
}

static long long find(HashMap *map, char *name, int flag) {
	long long hashVal = hash(name);
	long long ind = hash1((double) map->mCapacity, hashVal) % map->mCapacity;
	long long step = hash2(hashVal);
	long long dummy = -1;

	for (size_t i = 0; i < map->mCapacity; i++) {
		if (map->mData[ind] == NULL) {
			if (dummy == -1) dummy = ind;
			break;
		}
		if (map->mData[ind]->mType == HASHMAP_DUMMY) {
			if (dummy == -1) dummy = ind;
		}
		else if (map->mData[ind]->mHash == hashVal && str_equals(name, map->mData[ind]->mName)) {
			return ind;
		}
		ind = (ind + step) % map->mCapacity;
	}
	return (flag) ? dummy : -1;
}


HashMap *HashMap_new(void (*freeData)(void *), void *(*cpy)(void *, void *, size_t)) {
	HashMap *map = malloc(sizeof(HashMap));
	if (map == NULL) {
		perror_src("");
		return NULL;
	}
	map->mMinRatio = HASHMAP_MIN_RATIO;
	map->mMaxRatio = HASHMAP_MAX_RATIO;
	map->mCapacity = HASHMAP_INITIAL_SIZE;
	map->mData = calloc(map->mCapacity, sizeof(HashMapData));
	if (map->mData == NULL) {
		perror_src("");
		free(map);
		return NULL;

	}
	map->mKeyNumber = 0;
	map->mDummyNumber = 0;
	map->mFree = freeData;
	map->mCpy = (cpy == NULL) ? memcpy : cpy;
	return map;
}

int HashMap_put(HashMap *map, char *name, void *value, size_t size) {
	long long hashVal = hash(name);
	long long empty = find(map, name, 1);
	if (empty == -1) {
		return 0;
	}
	if (map->mData[empty] == NULL) {
		map->mKeyNumber++;
	}
	else if (map->mData[empty]->mType == HASHMAP_DUMMY) {
		HashMapData_free(map->mData[empty], map->mFree);
		map->mDummyNumber--;
		map->mKeyNumber++;
	}
	else {
		HashMapData_free(map->mData[empty], map->mFree);
	}
	void *data = malloc(size);
	if (data == NULL) {
		return -1;
	}

	if (map->mCpy(data, value, size) == NULL) {
		free(data);
		return -1;
	}
	map->mData[empty] = HashMapData_new(hashVal, name, data, size);

	if (map->mDummyNumber + map->mKeyNumber >= (size_t) ((double) map->mCapacity * map->mMaxRatio)) {
		int ratio = (map->mDummyNumber < map->mKeyNumber) ? 2 : 1;
		resize(map, ratio * map->mCapacity);
	}
	return 1;
}

int HashMap_get(HashMap *map, char *name, void *dst) {
	long long pos = find(map, name, 0);
	if (pos == -1) {
		return 0;
	}
	if (!map->mCpy(dst, map->mData[pos], map->mData[pos]->mSize)) {
		return 0;
	}
	return 1;
}

static void resize(HashMap *map, size_t newSize) {
	HashMapData **old = map->mData;
	size_t oldLen = map->mCapacity;
	HashMapData **new = calloc(newSize, sizeof(HashMapData));
	if (new == NULL) {
		perror_src("");
		return;
	}
	map->mData = new;
	map->mCapacity = newSize;
	map->mKeyNumber = 0;
	map->mDummyNumber = 0;
	for (size_t i = 0; i < oldLen; i++) {
		if (old[i] != NULL && old[i]->mType != HASHMAP_DUMMY) {
			HashMap_put(map, old[i]->mName, old[i]->mData, old[i]->mSize);
		}
	}
	for (size_t i = 0; i < oldLen; i++) {
		old[i]->mType = HASHMAP_DUMMY;
		old[i] = HashMapData_free(old[i], map->mFree);
	}
	free(old);
}

int HashMap_remove(HashMap *map, char *name) {		//TODO: fix Donnees corrompues
	long long pos = find(map, name, 0);
	if (pos == -1) {
		return -1;
	}
	HashMapData *del = map->mData[pos];
	map->mData[pos] = dummy(del->mHash);
	if (del->mType == HASHMAP_FULL) {
		HashMapData_free(del, map->mFree);
	}
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
		map->mData[i] = HashMapData_free(map->mData[i], map->mFree);
	}
	free(map->mData);
	free(map);
}
