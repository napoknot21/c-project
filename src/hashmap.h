//
// Created by Kevin on 05/07/2022.
//
#include <stdlib.h>
#ifndef C_PROJECT_HASHMAP_H
#define C_PROJECT_HASHMAP_H
#define DUMMY_DATA(h) ((HashMapData) {.hash = (h), .function = FUNCTION_NULL, .type=DUMMY})
#define NONE_DATA ((HashMapData) {.hash = 0,.function = FUNCTION_NULL, .type=NONE})

typedef struct HashMap HashMap;
typedef struct HashMapData HashMapData;

struct HashMapData {
    long long hash;
    Function function;
    FuncType type;
};
struct HashMap {
    HashMapData *data;
    size_t capacity;
    size_t keyNumber;
    size_t dummyNumber;
    float minRatio, maxRatio;
};

static void resize(HashMap *map, size_t new);

static HashMapData hashMap_get(HashMap *map, const char *name);

static HashMap *HashMap_new();

static void hashMapData_free(HashMapData data);

static long long hash(const char *name);

static long long hash1(double capacity, long long hash);

static long long hash2(long long hash);

static long long find(HashMap *map, const char *name, int flag);

static int hashMap_put(HashMap *map, Function function, FuncType type);

static void hashMap_free(HashMap *map);

static int HashMap_remove(HashMap *map, char *name);
#endif //C_PROJECT_HASHMAP_H
