#include "utilities/path.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>

bool path_exists(string const *path) {
  DIR *directory = opendir(path->data);
  if (directory) {
    closedir(directory);
    return true;
  }

  FILE *file = fopen(path->data, "r");
  if (file) {
    fclose(file);
    return true;
  }

  return false;
}

char path_get_system_slash() { return '/'; }

string path_join(string const *first, string const *second) {
  string slashes = string_wrap_cstring("/\\");
  bool ends_with_slash = string_find_last_any(first, &slashes, first->length) ==
                         (int32_t)(first->length - 1);
  bool starts_with_slash = string_find_first_any(second, &slashes, 0) == 0;

  if (ends_with_slash && starts_with_slash) {
    string substr = string_new_substring(first->data, 0, second->length - 1);
    string_append(&substr, second);
    return substr;
  } else if (ends_with_slash || starts_with_slash) {
    string f = string_new(first->data);
    string_append(&f, second);
    return f;
  }

  char const slash[] = {path_get_system_slash(), 0};
  string slash_s = string_wrap_cstring(slash);
  string result = string_new(first->data);
  string_append(&result, &slash_s);
  string_append(&result, second);
  return result;
}

string path_to_system_slashes(string const *path) {
  char const *replace_slash = (path_get_system_slash() == '/') ? "\\" : "/";
  string replace_this = string_wrap_cstring(replace_slash);
  char replace_with_slash[] = {path_get_system_slash(), 0};

  string replace_with = string_wrap_cstring(replace_with_slash);

  return string_replace(path, &replace_this, &replace_with);
}
