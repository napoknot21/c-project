//
// Created by Kevin on 05/07/2022.
//

#include "Buffer.h"

#include <stdlib.h>

#include "exec_error.h"

static size_t BUFFER_SIZE = 16;

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
	buffer->mCapacity = BUFFER_SIZE;
	buffer->mLength = 0;
	return buffer;
}

Buffer *Buffer_clear(Buffer *buffer) {
	char* del = buffer->mBuffer;
	buffer->mBuffer = NULL;
	buffer->mBuffer = calloc(BUFFER_SIZE, sizeof(char));
	free(del);
	if (buffer->mBuffer == NULL) {
		perror_src("");
		free(buffer);
		return NULL;
	}
	buffer->mCapacity = BUFFER_SIZE;
	buffer->mLength = 0;
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
	if (buffer->mLength >= buffer->mCapacity) {
		char *tmp = realloc(buffer->mBuffer, 2 * buffer->mCapacity);
		if (tmp == NULL) {
			perror_src("");
			return 0;
		}
		buffer->mCapacity *= 2;
		buffer->mBuffer = tmp;
	}
	buffer->mBuffer[buffer->mLength++] = e;
	return 1;
}
