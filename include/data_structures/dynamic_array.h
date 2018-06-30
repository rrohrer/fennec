/**
 * @file
 * @author Ryan Rohrer <ryan.rohrer@gmail.com>
 *
 * @section DESCRIPTION
 * Dynamic array optimized for cache locality. Functionally similar as
 * std::vector in c++.
 */
#ifndef dynamic_array_h
#define dynamic_array_h

#include "fennec.h"

/**
 * A Dynamically expandable array.
 */
typedef struct {
  uint32_t size;
  uint32_t capacity;
  uint32_t object_size;
  char *data;
} dynamic_array;

/**
 * Constructor for a new dynamic_array.
 *
 * @param object_size - the sizeof() the data that will be stored in this array.
 * @return - a newly constructed and empty array.
 */
dynamic_array dynamic_array_new(uint32_t object_size);

/**
 * Constructor for a new dynamic_array with reserved capacity.
 *
 * @param object_size - the sizeof() the data that will be stored in the array.
 * @param reserve_count - the number of items reserved by default.
 * @return - the newly constructed and reserved dynamic_array.
 */
dynamic_array dynamic_array_reserved_new(uint32_t object_size,
                                         uint32_t reserve_cout);

/**
 * Reserve a certain number of items in this dynamic array.
 *
 * @param array - the array that will now have capacity >= N.
 * @param reserve_count - the number of slots to guarantee.
 */
void dynamic_array_reserve(dynamic_array *array, uint32_t reserve_cout);

/**
 * Append a value to the end of an array.
 *
 * @param array - the array getting appended to.
 * @param data - the data getting appended. (this will be copied)
 */
void dynamic_array_push_back(dynamic_array *array, void const *data);

/**
 * Remove the last element of an array.
 *
 * @param array - the array to have the last item removed from.
 */
void dynamic_array_pop_back(dynamic_array *array);

/**
 * Return a pointer to the last element in the array.
 *
 * @param array - the array that the pointer will be returned from.
 * @return - a pointer to the last element of the array, NULL if empty.
 */
void *dynamic_array_get_back(dynamic_array *array);

/**
 * Return a pointer to the first element in an array.
 *
 * @param array - the array that the element will be retrieved from.
 * @return - a pointer to the first element in the array, NULL if empty.
 */
void *dynamic_array_get_front(dynamic_array *array);

/**
 * Return a pointer to the element at the location specified by index.
 *
 * @param array - the array to have an element retrieved from it.
 * @param index - the index to get.
 * @return - the element from index, NULL if not found.
 */
void *dynamic_array_get_at(dynamic_array *array, uint32_t index);

/**
 * Remove an element from the given index.
 *
 * @param array - the array that the element will be removed from.
 * @param index - the index that will be removed.
 */
void dynamic_array_remove(dynamic_array *array, uint32_t index);

/**
 * Checks to see if this array is empty.
 *
 * @param array - the array to check.
 * @return - returns true if size == 0.
 */
bool dynamic_array_is_empty(dynamic_array const *array);

/**
 * Removes all elements in an array.
 *
 * @param array - the array that will be cleared.
 */
void dynamic_array_clear(dynamic_array *array);

/**
 * Resizes an array to hold only the elements it has. Makes size == capacity.
 *
 * @param array - the array that will be resized.
 */
void dynamic_array_shrink(dynamic_array *array);

/**
 * Deallocates a dynamic array entirely.
 *
 * @param array - the array that is being deallocated.
 */
void dynamic_array_free(dynamic_array *array);

#endif
