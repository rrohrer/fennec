#include "data_structures/hashtable.h"

#define HASHTABLE_MAX_LOAD_FACTOR 0.85f
#define HASHTABLE_INITIAL_CAPACITY 16

struct hashtable_bucket;
typedef struct {
  struct hashtable_bucket *next;
  bool is_valid;
  uint32_t probe_count;
  void *key;
  char value[];
} hashtable_bucket;

static uint32_t hashtable_string_hash(void const *string) {
  char const *s = (char const *)string;
  uint32_t hash = 0;
  while (*s)
    hash = hash * 101 + *s++;
  return hash;
}

static bool hashtable_string_comparison(void const *string1,
                                        void const *string2) {
  return strcmp((char const *)string1, (char const *)string2) == 0;
}

static void *hashtable_string_copy(void const *string) {
  return strdup((char const *)string);
}

static void *hashtable_passthrough_copy(void const *data) {
  return (void *)data;
}

static float hashtable_calculate_load_factor(hashtable const *table) {
  if (table->capacity == HASHTABLE_INITIAL_CAPACITY) {
    return 1.f;
  }

  return (float)table->size / (float)table->capacity;
}

static uint32_t hashtable_calculate_bucket_size(hashtable const *table) {
  return sizeof(hashtable_bucket) + table->object_size;
}

static void hashtable_reallocate(hashtable *table, uint32_t new_capacity) {
  char *old_data = table->data;
  uint32_t old_capacity = table->capacity;

  uint32_t bucket_size = hashtable_calculate_bucket_size(table);
  uint32_t new_size = bucket_size * new_capacity;
  table->data = (char *)malloc(new_size);
  memset(table->data, 0, new_size);
  table->capacity = new_capacity;
  table->size = 0;

  if (old_data == NULL) {
    return;
  }

  hash_key_copy_function_type copy_func = table->copy_function;
  table->copy_function = hashtable_passthrough_copy;
  for (uint32_t i = 0; i < old_capacity; ++i) {
    hashtable_bucket *current =
        (hashtable_bucket *)(old_data + bucket_size * i);

    if (current->is_valid) {
      hashtable_insert(table, current->key, current->value);
    }
  }
  table->copy_function = copy_func;

  free(old_data);
}

static void hashtable_fill_bucket(hashtable *table, hashtable_bucket *bucket,
                                  void const *key, void const *value) {
  bucket->is_valid = true;
  bucket->key = table->copy_function(key);
  memcpy(bucket->value, value, table->object_size);
}

static hashtable_bucket *hashtable_get_last_in_chain(hashtable_bucket *start) {
  hashtable_bucket *current = (hashtable_bucket *)start->next;
  while (current) {
    if ((hashtable_bucket *)current->next == start)
      return current;
    current = (hashtable_bucket *)current->next;
  }
  return start;
}

static hashtable_bucket *hashtable_lookup_key(hashtable const *table,
                                              void const *key) {
  if (table->capacity == HASHTABLE_INITIAL_CAPACITY) {
    return NULL;
  }

  uint32_t hashed_key = table->hash_function(key);
  uint32_t index = hashed_key & (table->capacity - 1);
  uint32_t bucket_size = hashtable_calculate_bucket_size(table);

  hashtable_bucket *start =
      (hashtable_bucket *)(table->data + (bucket_size * index));
  if (start->probe_count != 0 || !start->is_valid)
    return NULL;

  hashtable_bucket *current = start;

  do {
    if (table->comparison_function(current->key, key))
      return current;
    current = (hashtable_bucket *)current->next;
  } while (current != start);

  return NULL;
}

hashtable hashtable_new(uint32_t object_size, hash_function_type hash_function,
                        hash_comparison_function_type comparison_function,
                        hash_key_copy_function_type copy_function) {
  return (hashtable){0,                          // size
                     HASHTABLE_INITIAL_CAPACITY, // capacity
                     object_size,                // object size
                     hash_function,
                     comparison_function,
                     copy_function,
                     NULL}; // data pointer
}

hashtable hashtable_new_string(uint32_t object_size) {
  return hashtable_new(object_size, hashtable_string_hash,
                       hashtable_string_comparison, hashtable_string_copy);
}

hashtable_bucket *hashtable_quadtratic_probe(hashtable *table,
                                             uint32_t bucket_size,
                                             uint32_t hashed_key) {
  uint32_t counter = 1;
  while (true) {
    /*
     * Quadtratic probing isn't garanteed to finish if the table is over 1/2
     * full and NOT a power of 2. IFF it's a power of 2, the only sequence
     * garanteed to finish is triangle numbers (thanks knuth). Use triangle
     * numbers to find the next bucket.
     */
    uint32_t triangle = (counter * counter + counter) / 2;
    uint32_t index = (hashed_key + triangle) & (table->capacity - 1);
    hashtable_bucket *current =
        (hashtable_bucket *)(table->data + bucket_size * index);
    if (!current->is_valid) {
      current->probe_count = counter;
      return current;
    }
    ++counter;
  }
}

void hashtable_insert(hashtable *table, void const *key, void const *value) {
  float load_factor = hashtable_calculate_load_factor(table);
  if (load_factor > HASHTABLE_MAX_LOAD_FACTOR) {
    hashtable_reallocate(table, table->capacity << 1);
  }

  uint32_t hashed_key = table->hash_function(key);
  uint32_t index = hashed_key & (table->capacity - 1);
  uint32_t bucket_size = hashtable_calculate_bucket_size(table);

  hashtable_bucket *current =
      (hashtable_bucket *)(table->data + index * bucket_size);
  table->size += 1;

  if (!current->is_valid) {
    goto hashtable_insert_here;
  }

  if (current->probe_count == 0) {
    hashtable_bucket *new_bucket =
        hashtable_quadtratic_probe(table, bucket_size, hashed_key);
    hashtable_bucket *last_bucket = hashtable_get_last_in_chain(current);
    new_bucket->next = (struct hashtable_bucket *)current;
    last_bucket->next = (struct hashtable_bucket *)new_bucket;
    hashtable_fill_bucket(table, new_bucket, key, value);
    return;
  }

  hashtable_bucket *new_bucket =
      hashtable_quadtratic_probe(table, bucket_size, hashed_key);
  hashtable_bucket *last_bucket = hashtable_get_last_in_chain(current);
  memcpy(new_bucket, current, bucket_size);
  if (last_bucket == current) {
    new_bucket->next = (struct hashtable_bucket *)new_bucket;
  } else {
    last_bucket->next = (struct hashtable_bucket *)new_bucket;
  }
  current->probe_count = 0;

hashtable_insert_here:
  hashtable_fill_bucket(table, current, key, value);
  current->next = (struct hashtable_bucket *)current;
}

void hashtable_remove(hashtable *table, void const *key) {
  hashtable_bucket *found = hashtable_lookup_key(table, key);
  if (found == NULL) {
    return;
  }

  table->size -= 1;
  free(found->key);

  hashtable_bucket *last = hashtable_get_last_in_chain(found);
  hashtable_bucket *next = (hashtable_bucket *)found->next;
  uint32_t bucket_size = hashtable_calculate_bucket_size(table);

  if (found->probe_count == 0) {
    if (next != found) {
      memcpy(found, next, bucket_size);
      found->probe_count = 0;

      hashtable_bucket *temp = next;
      next = found;
      found = temp;
    }
  }

  last->next = (struct hashtable_bucket *)next;
  memset(found, 0, bucket_size);
}

bool hashtable_exists(hashtable const *table, void const *key) {
  return hashtable_lookup_key(table, key) != NULL;
}

void *hashtable_lookup(hashtable *table, void const *key) {
  hashtable_bucket *bucket = hashtable_lookup_key(table, key);
  if (bucket == NULL) {
    return NULL;
  }

  return bucket->value;
}

void *hashtable_iterate(hashtable *table, void *last_position) {
  uint32_t bucket_size = hashtable_calculate_bucket_size(table);
  uint32_t index = 0;

  if (table->data == NULL) {
    return NULL;
  }

  if (last_position != NULL) {
    index = (uint32_t)((char *)last_position - table->data) / bucket_size + 1;
  }

  hashtable_bucket *current =
      (hashtable_bucket *)(table->data + index * bucket_size);
  hashtable_bucket *end =
      (hashtable_bucket *)(table->data + table->capacity * bucket_size);

  while (current < end) {
    if (current->is_valid) {
      return current->value;
    }

    current++;
  }

  return NULL;
}

void hashtable_shrink(hashtable *table) {
  uint32_t new_capacity =
      (uint32_t)ceil((float)table->size / HASHTABLE_MAX_LOAD_FACTOR);
  uint32_t new_pow2_capacity = 2;
  while (new_capacity > new_pow2_capacity) {
    new_pow2_capacity = new_pow2_capacity << 1;
  }

  hashtable_reallocate(table, new_pow2_capacity);
}

void hashtable_free(hashtable *table) {
  if (table->data == NULL) {
    return;
  }

  uint32_t bucket_size = hashtable_calculate_bucket_size(table);

  for (uint32_t i = 0; i < table->capacity; ++i) {
    hashtable_bucket *current =
        (hashtable_bucket *)(table->data + i * bucket_size);

    if (current->key) {
      free(current->key);
    }
  }

  free(table->data);

  *table = hashtable_new(table->object_size, table->hash_function,
                         table->comparison_function, table->copy_function);
}
