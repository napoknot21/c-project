//
// Created by Kevin on 05/07/2022.
//

#ifndef C_PROJECT_BUFFER_H
#define C_PROJECT_BUFFER_H
/**
 * Parser Buffer.
*/
typedef struct Buffer {
    char *buffer;
    int capacity;
    int length;
} Buffer;

/**
 * Create a dynamic buffer.
 * @return The buffer pointer.
 */
Buffer *buffer_new();

/**
 * Clear the buffer and puts its capacity to the BUFFER_SIZE.
 * @param buffer The buffer pointer.
 * @return the buffer if everything went fine, NULL otherwise
 */
Buffer *buffer_clear(Buffer *buffer);

/**
 * Free the buffer.
 *
 * @param buffer The buffer which will be free.
 * @return NULL.
 */
Buffer *buffer_free(Buffer *buffer);

/**
 * Add the given element in the given buffer
 * @param buffer The buffer
 * @param e The element which will be added to the buffer
 * @return 1 if the element is added to the buffer, 0 otherwise
 */
int buffer_add(Buffer *buffer, char e);


#endif //C_PROJECT_BUFFER_H
