#include "utilities/path.h"
#include "utilities/test_helpers.h"
#include <stdio.h>

int test_path_exists() {
  string path = string_wrap_cstring("Makefile");
  FAIL_IF(!path_exists(&path), "Failed to find a file that was there.\n");

  string path_not_here = string_wrap_cstring("this_isnt_real");
  FAIL_IF(path_exists(&path_not_here), "Found a path that wasn't there\n");

  return 0;
}

int test_path_join() {
  string a = string_wrap_cstring("hello/");
  string b = string_wrap_cstring("/world");
  string result = path_join(&a, &b);
  string correct_result = string_wrap_cstring("hello/world");
  FAIL_IF(string_compare(&result, &correct_result) != string_equal,
          "Simple path join failed.\n");
  string_free(&result);

  a = string_wrap_cstring("hello");
  b = string_wrap_cstring("world");
  result = path_join(&a, &b);
  FAIL_IF(string_compare(&result, &correct_result) != string_equal,
          "Simple path join(1) failed.\n");
  string_free(&result);

  return 0;
}

int test_path_to_system_slashes() {
  string wrong = string_wrap_cstring("this\\is\\wrong");
  string right = string_wrap_cstring("this/is/wrong");
  string result = path_to_system_slashes(&wrong);
  FAIL_IF(string_compare(&result, &right),
          "Failed to convert wrong to right.\n");
  string_free(&result);

  result = path_to_system_slashes(&right);
  FAIL_IF(string_compare(&result, &right),
          "Failed to convert right to right.\n");
  string_free(&result);

  return 0;
}

int main(void) {
  RETURN_IF_FAILED(test_path_exists());
  RETURN_IF_FAILED(test_path_join());
  RETURN_IF_FAILED(test_path_to_system_slashes());
  return 0;
}