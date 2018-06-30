#include "data_structures/dynamic_array.h"
#include "utilities/test_helpers.h"
#include <stdio.h>

int test_basic() {
  dynamic_array a = dynamic_array_new(sizeof(int));

  FAIL_IF(!dynamic_array_is_empty(&a),
          "Array incorrectly reports itself as non-empty.\n");

  int numbers[] = {1, 2, 3, 4, 5,  1, 2, 3, 4, 5, 6, 7, 4, 5, 6,  7, 8,
                   6, 5, 5, 4, 34, 5, 5, 3, 4, 5, 3, 7, 2, 5, 43, 5, 4};
  unsigned array_size = sizeof(numbers) / sizeof(int);
  for (unsigned i = 0; i < array_size; ++i) {
    dynamic_array_push_back(&a, &numbers[i]);
  }

  FAIL_IF(a.size != array_size, "Array doesn't correctly update size.\n");

  for (unsigned i = 0; i < array_size; ++i) {
    FAIL_IF(*(int *)dynamic_array_get_at(&a, i) != numbers[i],
            "Ordering of array is wrong.\n");
  }

  FAIL_IF(dynamic_array_is_empty(&a),
          "Array incorrectly reports itself as empty.\n");

  dynamic_array_clear(&a);
  FAIL_IF(!dynamic_array_is_empty(&a),
          "Array incorrectly reports itself as non-empty.\n");

  dynamic_array_free(&a);

  return 0;
}

int test_reserve() {
  unsigned reserve_size = 1300;

  dynamic_array a = dynamic_array_reserved_new(sizeof(unsigned), reserve_size);
  FAIL_IF(!dynamic_array_is_empty(&a),
          "Array incorrectly reports itself as non-empty.\n");

  FAIL_IF(a.capacity != reserve_size,
          "Array doesn't correctly update capacity.\n");
  FAIL_IF(a.size != 0, "Array doesn't correctly update size.\n");

  for (unsigned i = 0; i < reserve_size; ++i) {
    dynamic_array_push_back(&a, &i);
  }

  FAIL_IF(a.size != a.capacity, "Array size is not being tracked correctly.\n");

  for (unsigned i = 0; i < reserve_size; ++i) {
    FAIL_IF(*(unsigned *)dynamic_array_get_at(&a, i) != i,
            "Array didn't set the right value.\n");
  }

  dynamic_array_free(&a);

  return 0;
}

int test_get_back() {
  dynamic_array a = dynamic_array_new(sizeof(int));

  int numbers[] = {1, 2, 3, 4, 5};
  int array_size = sizeof(numbers) / sizeof(int);

  for (int i = 0; i < array_size; ++i) {
    dynamic_array_push_back(&a, &numbers[i]);
    FAIL_IF(*(int *)dynamic_array_get_back(&a) != numbers[i],
            "Array get back didn't return the right value.\n");
  }

  dynamic_array_free(&a);

  return 0;
}

int test_remove() {
  dynamic_array a = dynamic_array_new(sizeof(int));
  int numbers[] = {1, 2, 3, 4, 5};
  int numbers_removed[] = {1, 2, 4, 5};
  int array_size = sizeof(numbers) / sizeof(int);
  int array_removed_size = sizeof(numbers_removed) / sizeof(int);

  for (int i = 0; i < array_size; ++i) {
    dynamic_array_push_back(&a, &numbers[i]);
  }

  dynamic_array_remove(&a, 2);
  FAIL_IF(a.size != (unsigned)array_removed_size,
          "Array remove did not update size correctly.\n");

  for (int i = 0; i < array_removed_size; ++i) {
    FAIL_IF(*(int *)dynamic_array_get_at(&a, i) != numbers_removed[i],
            "Array remove didn't preserve order.\n");
  }

  dynamic_array_free(&a);

  return 0;
}

int test_shrink() {
  dynamic_array a = dynamic_array_reserved_new(sizeof(int), 400);

  int value = 42;
  dynamic_array_push_back(&a, &value);

  FAIL_IF(a.capacity != 400, "Array capacity is not correct.\n");
  FAIL_IF(a.size != 1, "Array size is not correct.\n");

  dynamic_array_shrink(&a);

  FAIL_IF(a.capacity != 1, "Array capacity is not correct.\n");
  FAIL_IF(a.size != 1, "Array size is not correct.\n");

  dynamic_array_free(&a);

  return 0;
}

int main(void) {
  RETURN_IF_FAILED(test_basic());
  RETURN_IF_FAILED(test_reserve());
  RETURN_IF_FAILED(test_get_back());
  RETURN_IF_FAILED(test_remove());
  RETURN_IF_FAILED(test_shrink());
  return 0;
}
