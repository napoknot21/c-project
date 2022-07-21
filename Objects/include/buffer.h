//
// Created by Kevin on 05/07/2022.
//

#ifndef C_PROJECT_BUFFER_H
#define C_PROJECT_BUFFER_H
/**
 * Parser Buffer.
*/
typedef struct Buffer {
    char *mBuffer;
    int mCapacity;
    int mLength;
} Buffer;

/**
 * Create a dynamic mBuffer.
 * @return The buffer pointer.
 */
Buffer *Buffer_new();

/**
 * Clear the mBuffer and puts its mCapacity to the BUFFER_SIZE.
 * @param buffer The buffer pointer.
 * @return the buffer if everything went fine, NULL otherwise
 */
Buffer *Buffer_clear(Buffer *buffer);

/**
 * Free the mBuffer.
 *
 * @param buffer The buffer which will be free.
 * @return NULL.
 */
Buffer *Buffer_free(Buffer *buffer);

/**
 * Add the given element in the given mBuffer
 * @param buffer The buffer
 * @param e The element which will be added to the buffer
 * @return 1 if the element is added to the buffer, 0 otherwise
 */
int Buffer_add(Buffer *buffer, char e);


#endif //C_PROJECT_BUFFER_H
