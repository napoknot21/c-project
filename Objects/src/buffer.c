//
// Created by Kevin on 05/07/2022.
//

#include <stdlib.h>


#include "buffer.h"
#include "exec_error.h"

#define BUFFER_SIZE 16

Buffer *Buffer_new() {
    Buffer *buffer = malloc(sizeof(Buffer));
    if (buffer == NULL) {
        perror_src("");
        return NULL;
    }
    buffer->buffer = calloc(BUFFER_SIZE, sizeof(char));
    if (buffer->buffer == NULL) {
        perror_src("");
        free(buffer);
        return NULL;
    }
    buffer->capacity = BUFFER_SIZE;
    buffer->length = 0;
    return buffer;
}

Buffer *Buffer_clear(Buffer *buffer) {
    free(buffer->buffer);
    buffer->buffer = calloc(BUFFER_SIZE, sizeof(char));
    if (buffer->buffer == NULL) {
        perror_src("");
        free(buffer);
        return NULL;
    }
    buffer->capacity = BUFFER_SIZE;
    buffer->length = 0;
    return buffer;
}

Buffer *Buffer_free(Buffer *buffer) {
    free(buffer->buffer);
    free(buffer);
    return NULL;
}

int Buffer_add(Buffer *buffer, const char e) {
    if (buffer == NULL) {
        perror_src("");
        return 0;
    }
    if (buffer->length + 1 >= buffer->capacity) {
        char *tmp = realloc(buffer->buffer, 2 * buffer->capacity);
        if (tmp == NULL) {
            perror_src("");
            return 0;
        }
        buffer->capacity *= 2;
        buffer->buffer = tmp;
    }
    buffer->buffer[buffer->length++] = e;
    return 1;
}