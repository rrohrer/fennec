#include "data_structures/dynamic_array.h"

static void dynamic_array_grow_if_needed(dynamic_array *array) {
  if (array->size > array->capacity) {
    array->capacity = (uint32_t)ceil((array->capacity + 10) * 1.8);
    array->data =
        (char *)realloc(array->data, array->capacity * array->object_size);
  }
}

dynamic_array dynamic_array_new(uint32_t object_size) {
  return (dynamic_array){0, 0, object_size, NULL};
}

dynamic_array dynamic_array_reserved_new(uint32_t object_size,
                                         uint32_t reserve_count) {
  dynamic_array array = dynamic_array_new(object_size);
  dynamic_array_reserve(&array, reserve_count);
  return array;
}

void dynamic_array_reserve(dynamic_array *array, uint32_t reserve_count) {
  if (array->capacity < reserve_count) {
    array->capacity = reserve_count;
    array->data = realloc(array->data, array->capacity * array->object_size);
  }
}

void dynamic_array_push_back(dynamic_array *array, void const *data) {
  uint32_t insertion_point = array->size++;
  dynamic_array_grow_if_needed(array);

  memcpy(array->data + insertion_point * array->object_size, data,
         array->object_size);
}

void dynamic_array_pop_back(dynamic_array *array) {
  if (array->size > 0) {
    array->size -= 1;
  }
}

void *dynamic_array_get_back(dynamic_array *array) {
  if (dynamic_array_is_empty(array)) {
    return NULL;
  }

  return dynamic_array_get_at(array, array->size - 1);
}

void *dynamic_array_get_front(dynamic_array *array) {
  if (dynamic_array_is_empty(array)) {
    return NULL;
  }

  return dynamic_array_get_at(array, 0);
}

void *dynamic_array_get_at(dynamic_array *array, uint32_t index) {
  if (index >= array->size) {
    return NULL;
  }

  return array->data + index * array->object_size;
}

void dynamic_array_remove(dynamic_array *array, uint32_t index) {
  if (index >= array->size) {
    return;
  }

  array->size -= 1;

  void *destination = array->data + index * array->object_size;
  void *source = array->data + (index + 1) * array->object_size;
  uint32_t move_size = (array->size - index) * array->object_size;

  memmove(destination, source, move_size);
}

bool dynamic_array_is_empty(dynamic_array const *array) {
  return array->size == 0;
}

void dynamic_array_clear(dynamic_array *array) { array->size = 0; }

void dynamic_array_shrink(dynamic_array *array) {
  if (array->size == array->capacity) {
    return;
  }

  array->capacity = array->size;
  array->data = realloc(array->data, array->capacity * array->object_size);
}

void dynamic_array_free(dynamic_array *array) {
  if (array->data) {
    free(array->data);
  }
}
