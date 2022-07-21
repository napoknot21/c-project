//
// Created by Kevin on 05/07/2022.
//

#include <stdlib.h>


#include "mBuffer.h"
#include "exec_error.h"

#define BUFFER_SIZE 16

Buffer *Buffer_new() {
    Buffer *buffer = malloc(sizeof(Buffer));
    if (buffer == NULL) {
        perror_src("");
        return NULL;
    }
    buffer->mBuffer = calloc(BUFFER_SIZE, sizeof(char));
    if (buffer->mBuffer == NULL) {
        perror_src("");
        free(buffer);
        return NULL;
    }
    buffer->capacity = BUFFER_SIZE;
    buffer->length = 0;
    return buffer;
}

Buffer *Buffer_clear(Buffer *buffer) {
    free(buffer->mBuffer);
    buffer->mBuffer = calloc(BUFFER_SIZE, sizeof(char));
    if (buffer->mBuffer == NULL) {
        perror_src("");
        free(buffer);
        return NULL;
    }
    buffer->capacity = BUFFER_SIZE;
    buffer->length = 0;
    return buffer;
}

Buffer *Buffer_free(Buffer *buffer) {
    free(buffer->mBuffer);
    free(buffer);
    return NULL;
}

int Buffer_add(Buffer *buffer, const char e) {
    if (buffer == NULL) {
        perror_src("");
        return 0;
    }
    if (buffer->length + 1 >= buffer->capacity) {
        char *tmp = realloc(buffer->mBuffer, 2 * buffer->capacity);
        if (tmp == NULL) {
            perror_src("");
            return 0;
        }
        buffer->capacity *= 2;
        buffer->mBuffer = tmp;
    }
    buffer->mBuffer[buffer->length++] = e;
    return 1;
}