#include "utilities/string.h"
#include "utilities/test_helpers.h"
#include <stdio.h>

#include <string.h>

int test_string_append() {
  string s = string_new("hello");
  string world_cstr = string_wrap_cstring(" world");
  string_append(&s, &world_cstr);

  string hello_world_cstr = string_wrap_cstring("hello world");
  FAIL_IF(!string_contains(&s, &hello_world_cstr),
          "String append did not return the correct result.\n");

  FAIL_IF(s.length != strlen(s.data),
          "String append did not concatinate length properly.\n");
  string_free(&s);
  return 0;
}

int test_string_find_first() {
  string s = string_wrap_cstring("test string.");
  string substr = string_wrap_cstring("str");
  int32_t index = string_find_first(&s, &substr, 0);
  FAIL_IF(index != 5, "String find first returned the wrong index.\n");

  string substr2 = string_wrap_cstring("ng");
  index = string_find_first(&s, &substr2, index);
  FAIL_IF(index != 9, "String find first returned the wrong index when started "
                      "from non zero.\n");

  string substr3 = string_wrap_cstring("not found");
  index = string_find_first(&s, &substr3, 0);

  FAIL_IF(index != string_invalid_index,
          "String find first found a string that didn't exist.\n");

  return 0;
}

int test_string_find_first_any() {
  string s = string_wrap_cstring("Hello World!");
  string glyphs = string_wrap_cstring("@#!");

  int32_t index = string_find_first_any(&s, &glyphs, 0);
  FAIL_IF(
      index != 11,
      "String find first any failed couldn't find the exclamation point.\n");

  string bad_glyphs = string_wrap_cstring("zzZ");
  index = string_find_first_any(&s, &bad_glyphs, 0);
  FAIL_IF(index != string_invalid_index,
          "String find first any found a glyph that didn't exist.\n");

  return 0;
}

int test_string_find_last() {
  string s = string_wrap_cstring("World World");
  string world = string_wrap_cstring("World");

  int32_t index = string_find_last(&s, &world, s.length);
  FAIL_IF(index != 6,
          "String find last failed with finding the wrong World.\n");

  string not_found = string_wrap_cstring("not found");
  index = string_find_last(&s, &not_found, s.length);
  FAIL_IF(index != string_invalid_index,
          "String find last found something that didnt exist.\n");

  return 0;
}

int test_string_find_last_any() {
  string s = string_wrap_cstring("Hello #World!");
  string search = string_wrap_cstring("$#");

  int32_t index = string_find_last_any(&s, &search, s.length);
  FAIL_IF(index != 6, "Failed to find the last instance of # in the string.\n");

  return 0;
}

int test_string_join() {
  string a = string_wrap_cstring("a");
  string b = string_wrap_cstring("b");
  string c = string_wrap_cstring("c");
  string seperator = string_wrap_cstring(" ");

  dynamic_array s = dynamic_array_new(sizeof(string));
  dynamic_array_push_back(&s, &a);
  dynamic_array_push_back(&s, &b);
  dynamic_array_push_back(&s, &c);

  string completed = string_join((string *)s.data, &seperator, s.size);
  string result = string_wrap_cstring("a b c");
  FAIL_IF(string_compare(&completed, &result) != string_equal,
          "String join failed to join correctly.\n");

  string_free(&completed);
  dynamic_array_free(&s);
  return 0;
}

int test_string_replace() {
  string replace_me = string_wrap_cstring("I like cats and cats are cute");
  string replace_me_cat = string_wrap_cstring("cat");
  string replace_same_length_dog = string_wrap_cstring("dog");
  string replace_same_length_result =
      string_replace(&replace_me, &replace_me_cat, &replace_same_length_dog);
  string replace_same_length_correct =
      string_wrap_cstring("I like dogs and dogs are cute");
  FAIL_IF(string_compare(&replace_same_length_result,
                         &replace_same_length_correct) != string_equal,
          "String failed to replace same length.\n");

  string replace_shorter = string_wrap_cstring("zz");
  string replace_shorter_result =
      string_replace(&replace_me, &replace_me_cat, &replace_shorter);
  string replace_shorter_correct =
      string_wrap_cstring("I like zzs and zzs are cute");
  FAIL_IF(string_compare(&replace_shorter_result, &replace_shorter_correct) !=
              string_equal,
          "String replace failed to replace a shorter string.\n");

  string replace_longer = string_wrap_cstring("zebra");
  string replace_longer_result =
      string_replace(&replace_me, &replace_me_cat, &replace_longer);
  string replace_longer_correct =
      string_wrap_cstring("I like zebras and zebras are cute");
  FAIL_IF(string_compare(&replace_longer_result, &replace_longer_correct) !=
              string_equal,
          "String replace failed to replace a longer string.\n");

  string_free(&replace_same_length_result);
  string_free(&replace_shorter_result);
  string_free(&replace_longer_result);

  return 0;
}

int main(void) {
  RETURN_IF_FAILED(test_string_append());
  RETURN_IF_FAILED(test_string_find_first());
  RETURN_IF_FAILED(test_string_find_first_any());
  RETURN_IF_FAILED(test_string_find_last());
  RETURN_IF_FAILED(test_string_find_last_any());
  RETURN_IF_FAILED(test_string_join());
  RETURN_IF_FAILED(test_string_replace());
  return 0;
}
