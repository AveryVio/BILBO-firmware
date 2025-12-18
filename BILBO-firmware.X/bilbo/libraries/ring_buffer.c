/*
 * File:   ring_buffer.c
 * Author: Miroslav Soukup
 * Description: Header file of ring buffer library.
 * 
 * Modified
 */

#include "ring_buffer.h"

#include <stdint.h> // data types 
#include <string.h> // memset() function 

#define BUFFER_SIZE MIC_RING_BUFFER_SIZE // buffer size

typedef struct ring_buffer_descriptor{
    uint16_t buffer[BUFFER_SIZE];
    uint16_t capacity;
    volatile uint8_t write_index;
    volatile uint8_t read_index;
} ring_buffer_t;


void ring_buffer_init (ring_buffer_t *me, uint16_t capacity) {
    me->capacity = capacity;                                        // store capacity of ring buffer
    memset(me->buffer, 0, (me->capacity) * sizeof(*(me->buffer)));  // clear all bytes of buffer
    me->write_index = 0;                                            // clear write index
    me->read_index  = 0;                                            // clear read index
}

inline uint16_t ring_buffer_get_next (uint16_t index, uint16_t capacity) {
    return ( (index >= (capacity - 1)) ? (0) : (index + 1) );
}


inline uint16_t ring_buffer_get_next_write (ring_buffer_t *me) {
    return ring_buffer_get_next(me->write_index, me->capacity);
}

inline uint8_t ring_buffer_is_full (ring_buffer_t *me) {
    return ( me->read_index == ring_buffer_get_next_write(me) );
}

uint16_t ring_buffer_write (ring_buffer_t *me, uint16_t value) {
    /*if (ring_buffer_is_full(me)) return 0;*/                      // if full -> error
    me->buffer[me->write_index] = value;                        // write down value
    me->write_index = ring_buffer_get_next_write(me);           // get and set new next write index
    return 1;
}


inline uint16_t ring_buffer_get_next_read (ring_buffer_t *me) {
    return ring_buffer_get_next(me->read_index, me->capacity);
}

inline uint8_t ring_buffer_is_empty (ring_buffer_t *me) {
    return ( me->read_index == me->write_index );
}

uint16_t ring_buffer_read (ring_buffer_t *me, uint16_t *value) {
    if (ring_buffer_is_empty(me)) return 0;                     // if empty -> error
    *value = me->buffer[me->read_index];                        // read out value
    me->read_index = ring_buffer_get_next_read(me);             // get and set new next read index
    return 1;
}