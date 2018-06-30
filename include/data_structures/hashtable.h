/**
 * @file
 * @author Ryan Rohrer <ryan.rohrer@gmail.com>
 *
 * @section DESCRIPTION
 * A coalescing hashtable.  Optimized for fast lookups even after collisions.
 */
#ifndef hashtable_h
#define hashtable_h

#include "fennec.h"

/**
 * Hashing function type.  Takes KEY data and returns an uint32_t.
 */
typedef uint32_t (*hash_function_type)(void const *);

/**
 * Comparison function type. Takes two KEY data pointers and returns true if
 * they are equal.
 */
typedef bool (*hash_comparison_function_type)(void const *, void const *);

/**
 * Copy function for keys. Takes a key, returns a newly allocated key that needs
 * to be manually free'd.
 */
typedef void *(*hash_key_copy_function_type)(void const *);

/**
 * A hashtable (aka dictionary).
 */
typedef struct {
  uint32_t size;
  uint32_t capacity;
  uint32_t object_size;
  hash_function_type hash_function;
  hash_comparison_function_type comparison_function;
  hash_key_copy_function_type copy_function;
  char *data;
} hashtable;

/**
 * Constructor for a new hashtable.
 *
 * @param object_size - the size of the object being stored in this table.
 * @param hash_function - function that describes how to hash keys.
 * @param comparison_function - function that tells if two keys are equal.
 * @param copy_function - function that cany allocate new copies of keys.
 * @return - a newly constructed hashtable.
 */
hashtable hashtable_new(uint32_t object_size, hash_function_type hash_function,
                        hash_comparison_function_type comparison_function,
                        hash_key_copy_function_type copy_function);

/**
 * Constructor for a new hashtable that is meant to use strings as keys.
 *
 * @param object_size - the size of the object being stored in this table.
 * @return - a newly constructed hashtable.
 */
hashtable hashtable_new_string(uint32_t object_size);

/**
 * Insert an element into the hashtable. The element and key are copied.
 *
 * @param table - the hashtable that the element is being inserted into.
 * @param key - the item that can be used to access this item later.
 * @param value - the item being inserted into the hashtable, will be copied.
 */
void hashtable_insert(hashtable *table, void const *key, void const *value);

/**
 * Remove an element from the hashtable.
 *
 * @param table - the hashtable that will have an element removed from it.
 * @param key - the key of which element to remove.
 */
void hashtable_remove(hashtable *table, void const *key);

/**
 * Lookup to see if this key exists in the hashtable.
 *
 * @param table - the hashtable to check.
 * @key - the item to check to see if it exists.
 * @return - true if the item exists in the table.
 */
bool hashtable_exists(hashtable const *table, void const *key);

/**
 * Looks into the hashtable and returns the item that key refers to (NULL if
 * doesn't exist).
 *
 * @param table - the hashtable to look into.
 * @param key - the key to look up.
 * @return - the item, if found or NULL if not.
 */
void *hashtable_lookup(hashtable *table, void const *key);

/**
 * Returns the next node in the hashtable (memory layout wise). NULL when there
 * are no more.
 *
 * @param table - the hashtable to iterate over.
 * @param last_position - the last position returned (continue point of
 * iteration). NULL to start from the beginning of the table.
 * @return - the next node in the table (memory order wise)
 */
void *hashtable_iterate(hashtable *table, void *last_position);

/**
 * Shrinks the hashtable to slightly under it's optimum load_factor.
 *
 * @param table - the hashtable to shrink.
 */
void hashtable_shrink(hashtable *table);

/**
 * Deallocates the internal data members and cleans up a hashtable.
 *
 * @param table - the hashtable that's being cleaned up.
 */
void hashtable_free(hashtable *table);

#endif
