/*
 * File:   ring_buffer.h
 * Author: Miroslav Soukup
 * Description: Header file of ring buffer library.
 * 
 * Modified
 */

#include <stdint.h> // data types 
#include <string.h> // memset() function 

#define BUFFER_SIZE MIC_RING_BUFFER_SIZE // buffer size

typedef struct ring_buffer_descriptor ring_buffer_t;

struct ring_buffer_descriptor{
    uint16_t buffer[BUFFER_SIZE];
    uint16_t capacity;
    volatile uint8_t write_index;
    volatile uint8_t read_index;
};


void ring_buffer_init (ring_buffer_t *me, uint16_t capacity);

inline uint16_t ring_buffer_get_next (uint16_t index, uint16_t capacity);


inline uint16_t ring_buffer_get_next_write (ring_buffer_t *me);

inline uint8_t ring_buffer_is_full (ring_buffer_t *me);

uint16_t ring_buffer_write (ring_buffer_t *me, uint16_t value);


inline uint16_t ring_buffer_get_next_read (ring_buffer_t *me);

inline uint8_t ring_buffer_is_empty (ring_buffer_t *me);

uint16_t ring_buffer_read (ring_buffer_t *me, uint16_t *value);