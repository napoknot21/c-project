//
// Created by Kevin on 05/07/2022.
//
#include "hashmap.h"
#define HASHMAP_INITIAL_SIZE 11
#define HASHMAP_MIN_RATIO ((float)0.25)
#define HASHMAP_MAX_RATIO ((float)0.75)
#define HASH_A ((sqrt(5)-1)/2)


static long long hash(const char *name) {
    double value = 0;
    double len = (double) strlen(name);
    for (int i = 0; i < len; i++) {
        value += (int) name[i] * (pow(31, len - i));
    }
    return (long long) value;
}

static long long hash1(double capacity, long long hash) {
    return (long long) (((double) (hash) / HASH_A) * capacity);
}

static long long hash2(long long hash) {
    return (long long) (2 * hash + 1) % LLONG_MAX;
}

static HashMap *HashMap_new() {
    HashMap *map = malloc(sizeof(HashMap));
    if (map == NULL) {
        printErr("");
        return NULL;
    }
    map->minRatio = HASHMAP_MIN_RATIO;
    map->maxRatio = HASHMAP_MAX_RATIO;
    map->capacity = HASHMAP_INITIAL_SIZE;
    map->data = malloc(sizeof(HashMapData) * map->capacity);
    for (int i = 0; i < map->capacity; i++) {
        map->data[i] = NONE_DATA;
    }
    if (map->data == NULL) {
        printErr("");
        free(map);
        return NULL;
    }
    map->keyNumber = 0;
    map->dummyNumber = 0;
    return map;
}

static long long find(HashMap *map, const char *name, int flag) {
    long long hashVal = hash(name);
    long long ind = hash1((double) map->capacity, hashVal) % (long long) map->capacity;
    long long step = hash2(hashVal);
    long long dummy = -1;
    for (int i = 0; i < map->capacity; i++) {
        if (map->data[ind].type == NONE) {
            if (dummy == -1) dummy = ind;
            break;
        }
        if (map->data[ind].type == DUMMY) {
            if (dummy == -1) dummy = ind;
        } else if (map->data[ind].hash == hashVal && str_equals(map->data[ind].function.name, name)) {
            return ind;
        }
        ind = (ind + step) % (long long) map->capacity;
    }
    return (flag) ? dummy : -1;
}

static int hashMap_put(HashMap *map, Function function, FuncType type) {
    long long hashVal = hash(function.name);
    long long empty = find(map, function.name, 1);
    if (empty == -1) return 0;

    HashMapData new = {.function = function, .type = type, .hash = hashVal};
    if (map->data[empty].type == DUMMY) {
        map->dummyNumber--;
        map->keyNumber++;
    }
    if (map->data[empty].type == NONE) {
        map->keyNumber++;
    }
    map->data[empty] = new;
    if (map->dummyNumber + map->keyNumber >= (long long) ((double) map->capacity * map->maxRatio)) {
        int ratio = (map->dummyNumber < map->keyNumber) ? 2 : 1;
        resize(map, ratio * map->capacity);
    }
    return 1;
}

static HashMapData hashMap_get(HashMap *map, const char *name) {
    long long pos = find(map, name, 0);
    if (pos == -1) return NONE_DATA;
    return map->data[pos];
}

static void resize(HashMap *map, size_t newSize) {
    HashMapData *old = map->data;
    size_t oldLen = map->capacity;
    HashMapData *new = malloc(newSize * sizeof(HashMapData));
    for (int i = 0; i < newSize; i++) new[i] = NONE_DATA;
    if (new == NULL) {
        printErr("");
        return;
    }
    map->data = new;
    map->capacity = newSize;
    map->keyNumber = 0;
    map->dummyNumber = 0;
    for (int i = 0; i < oldLen; i++) {
        if (old[i].type == NONE || old[i].type == DUMMY) continue;
        else hashMap_put(map, old[i].function, old[i].type);
    }
    free(old);
}

static void hashMapData_free(HashMapData data) {
    if (data.type == NONE || data.type == DUMMY) return;
    function_free(data.function);
}

static int HashMap_remove(HashMap *map, char *name) {
    long long pos = find(map, name, 0);
    if (pos == -1) return -1;
    HashMapData del = map->data[pos];
    map->data[pos] = DUMMY_DATA(del.hash);
    hashMapData_free(del);
    map->dummyNumber++;
    map->keyNumber--;
    if (map->dummyNumber + map->keyNumber <= (long long) ((double) map->capacity * map->minRatio)) {
        resize(map, (map->capacity / 2));
    }
    return 1;
}

static void hashMap_free(HashMap *map) {
    if (map == NULL) return;
    for (int i = 0; i < map->capacity; i++) {
        hashMapData_free(map->data[i]);
    }
    free(map);
}





