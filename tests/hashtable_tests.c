#include "data_structures/dynamic_array.h"
#include "data_structures/hashtable.h"
#include "utilities/test_helpers.h"
#include <stdio.h>
#include <string.h>

int test_basic() {
  hashtable h = hashtable_new_string(sizeof(int));

  char const *strings[] = {"hello", "what", "butts", "buuuuuuuts",
                           "cat",   "bat",  "rat",   "helllooooooooo"};
  int strings_length = sizeof(strings) / sizeof(char *);

  for (int i = 0; i < strings_length; ++i) {
    hashtable_insert(&h, strings[i], &i);
  }

  for (int i = 0; i < strings_length; ++i) {
    FAIL_IF(!hashtable_exists(&h, strings[i]),
            "Hashtable failed to find string.\n");
  }

  hashtable_free(&h);

  return 0;
}

dynamic_array make_word_list(unsigned count) {
  dynamic_array a = dynamic_array_reserved_new(sizeof(char *), count);

  for (unsigned i = 0; i < count; ++i) {
    char buffer[64];
    sprintf(buffer, "test string %u", i);

    char *new_string = strdup(buffer);
    dynamic_array_push_back(&a, &new_string);
  }

  return a;
}

void free_word_list(dynamic_array a) {
  for (unsigned i = 0; i < a.size; ++i) {
    free(*(char **)dynamic_array_get_at(&a, i));
  }

  dynamic_array_free(&a);
}

int test_lookup_remove_and_shrink() {
  hashtable h = hashtable_new_string(sizeof(unsigned));

  unsigned word_list_size = 129000;
  dynamic_array word_list = make_word_list(word_list_size);

  for (unsigned i = 0; i < word_list_size; ++i) {
    hashtable_insert(&h, *(char **)dynamic_array_get_at(&word_list, i), &i);
  }

  for (unsigned i = 0; i < word_list_size; ++i) {
    FAIL_IF(
        !hashtable_exists(&h, *(char **)dynamic_array_get_at(&word_list, i)),
        "Hashtable failed to find string.\n");
    unsigned *test =
        hashtable_lookup(&h, *(char **)dynamic_array_get_at(&word_list, i));
    FAIL_IF(*test != i, "Hashtable lookup value is wrong.\n");
  }

  for (unsigned i = 0; i < word_list.size; i += 101) {
    char *current_string = *(char **)dynamic_array_get_at(&word_list, i);
    hashtable_remove(&h, current_string);
    dynamic_array_remove(&word_list, i);
    free(current_string);
  }

  for (unsigned i = 0; i < word_list.size; ++i) {
    FAIL_IF(
        !hashtable_exists(&h, *(char **)dynamic_array_get_at(&word_list, i)),
        "Hashtable failed to find string after removals.\n");
  }

  hashtable_shrink(&h);

  for (unsigned i = 0; i < word_list.size; ++i) {
    FAIL_IF(
        !hashtable_exists(&h, *(char **)dynamic_array_get_at(&word_list, i)),
        "Hashtable failed to find string after shrinking.\n");
  }

  free_word_list(word_list);
  hashtable_free(&h);

  return 0;
}

int main(void) {
  RETURN_IF_FAILED(test_basic());
  RETURN_IF_FAILED(test_lookup_remove_and_shrink());
  return 0;
}
