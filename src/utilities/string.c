#include "utilities/string.h"

string string_new(char const *data) {
  string result;
  result.length = strlen(data);
  result.capacity = result.length + 1;
  result.data = strdup(data);
  return result;
}

string_range string_range_new(string *s, uint32_t start, uint32_t end) {
  return (string_range){s, start, end};
}

string string_new_substring(char const *data, uint32_t start, uint32_t end) {
  string result;
  result.length = end - start;
  result.capacity = result.length + 1;
  result.data = (char *)malloc(result.capacity);
  memcpy(result.data, data + start, result.length);
  result.data[result.length] = 0;
  return result;
}

string string_wrap_cstring(char const *cstring) {
  return (string){(char *)cstring, strlen(cstring), 0};
}

void string_free(string *s) {
  free(s->data);
  s->data = NULL;
  s->capacity = 0;
  s->length = 0;
}

void string_append(string *destination, string const *source) {
  uint32_t destination_min_capacity = source->length + destination->length + 1;

  if (destination->capacity < destination_min_capacity) {
    destination->capacity = destination_min_capacity;
    char *temp = (char *)malloc(destination_min_capacity);
    memcpy(temp, destination->data, destination->length);
    temp[destination->length] = 0;
    free(destination->data);
    destination->data = temp;
  }

  memcpy(destination->data + destination->length, source->data, source->length);
  destination->length = destination_min_capacity - 1;
  destination->data[destination->length] = 0;
}

bool string_contains(string const *s, string const *search) {
  return string_find_first(s, search, 0) != string_invalid_index;
}

int32_t string_compare(string const *s1, string const *s2) {
  int result = strcmp(s1->data, s2->data);
  if (result > 0) {
    return string_greater_than;
  } else if (result < 0) {
    return string_less_than;
  } else {
    return string_equal;
  }
}

int32_t string_range_compare(string_range const *r1, string_range const *r2) {
  for (uint32_t i = r1->start, j = r2->start; i < r1->end && j < r2->end;
       ++i, ++j) {
    if (r1->data->data[i] != r2->data->data[j]) {
      if (r1->data->data[i] < r2->data->data[i]) {
        return string_less_than;
      } else {
        return string_greater_than;
      }
    }
  }

  return string_equal;
}

int32_t string_find_first(string const *s, string const *substring,
                          uint32_t start) {
  if (substring->length == 0 || s->length == 0) {
    return string_invalid_index;
  }

  for (uint32_t i = start; i < s->length; ++i) {
    for (uint32_t j = 0, k = i; j < substring->length && k < s->length;
         ++j, ++k) {
      if (s->data[k] != substring->data[j]) {
        break;
      }

      if (j + 1 == substring->length) {
        return (int32_t)i;
      }
    }
  }

  return string_invalid_index;
}

int32_t string_find_first_any(string const *s, string const *character_set,
                              uint32_t start) {
  if (s->length == 0 || character_set->length == 0) {
    return string_invalid_index;
  }

  for (uint32_t i = start; i < s->length; ++i) {
    for (uint32_t j = 0; j < character_set->length; ++j) {
      if (s->data[i] == character_set->data[j]) {
        return (int32_t)i;
      }
    }
  }

  return string_invalid_index;
}

int32_t string_find_last(string const *s, string const *substring,
                         uint32_t start) {
  if (s->length == 0 || substring->length == 0) {
    return string_invalid_index;
  }

  for (int32_t i = start; i >= 0; --i) {
    for (int32_t j = substring->length - 1, k = i; j >= 0 && k >= 0; --j, --k) {
      if (s->data[k] != substring->data[j]) {
        break;
      }

      if (j == 0) {
        return k;
      }
    }
  }

  return string_invalid_index;
}

int32_t string_find_last_any(string const *s, string const *character_set,
                             uint32_t start) {
  if (s->length == 0 || character_set->length == 0) {
    return string_invalid_index;
  }

  for (int32_t i = (int32_t)start; i >= 0; --i) {
    for (uint32_t j = 0; j < character_set->length; ++j) {
      if (s->data[i] == character_set->data[j]) {
        return i;
      }
    }
  }

  return string_invalid_index;
}

string string_join(string const *strings, string const *separator,
                   uint32_t count) {
  string result;
  result.capacity = 0;
  result.length = separator->length * (count - 1);

  for (uint32_t i = 0; i < count; ++i) {
    result.length += strings[i].length;
  }

  result.capacity = result.length + 1;
  result.data = (char *)malloc(result.capacity);
  uint32_t result_write_index = 0;

  for (uint32_t i = 0; i < count; ++i) {
    if (i != 0 && separator->length != 0) {
      memcpy(result.data + result_write_index, separator->data,
             separator->length);
      result_write_index += separator->length;
    }

    memcpy(result.data + result_write_index, strings[i].data,
           strings[i].length);
    result_write_index += strings[i].length;
  }

  result.data[result.length] = 0;
  return result;
}

string string_replace(string const *s, string const *search,
                      string const *replace_with) {
  int32_t search_index = 0;
  int32_t found_substrings = 0;

  do {
    search_index = string_find_first(s, search, search_index);
    if (search_index != string_invalid_index) {
      search_index += search->length;
      found_substrings += 1;
    }
  } while (search_index != string_invalid_index);

  if (found_substrings == 0) {
    return string_new(s->data);
  }

  string result;
  result.length = s->length - (search->length * found_substrings) +
                  (replace_with->length * found_substrings);
  result.capacity = result.length + 1;
  result.data = (char *)malloc(result.capacity);

  int32_t replace_point = string_find_first(s, search, 0);
  int32_t s_i = 0;

  for (uint32_t i = 0; i < result.length;) {
    if (replace_point == s_i) {
      for (uint32_t j = 0; j < replace_with->length; ++j) {
        result.data[i + j] = replace_with->data[j];
      }
      replace_point =
          string_find_first(s, search, replace_point + search->length);
      i += replace_with->length;
      s_i += search->length;
    } else {
      result.data[i++] = s->data[s_i++];
    }
  }

  result.data[result.length] = 0;
  return result;
}

dynamic_array string_split(string const *s, string const *seperator) {
  dynamic_array result = dynamic_array_new(sizeof(string));
  int32_t search_start = 0;
  int32_t seperator_index;

  do {
    seperator_index = string_find_first(s, seperator, search_start);
    if (seperator_index != string_invalid_index &&
        (seperator_index - search_start) != 0) {
      string new_segment =
          string_new_substring(s->data, search_start, seperator_index);
      dynamic_array_push_back(&result, &new_segment);
      search_start = seperator_index + seperator->length;
    }
  } while (seperator_index != string_invalid_index);

  return result;
}

dynamic_array string_split_any(string const *s, string const *character_set) {
  dynamic_array result = dynamic_array_new(sizeof(string));
  int32_t search_start = 0;
  int32_t seperator_index;

  do {
    seperator_index = string_find_first_any(s, character_set, search_start);
    if (seperator_index != string_invalid_index &&
        (seperator_index - search_start) != 0) {
      string new_segment =
          string_new_substring(s->data, search_start, seperator_index);
      dynamic_array_push_back(&result, &new_segment);
      search_start = seperator_index + 1;
    }
  } while (seperator_index != string_invalid_index);

  return result;
}

dynamic_array string_split_after(string const *s, string const *seperator) {

  dynamic_array result = dynamic_array_new(sizeof(string));
  int32_t search_start = 0;
  int32_t seperator_index;

  do {
    seperator_index = string_find_first(s, seperator, search_start);
    if (seperator_index != string_invalid_index &&
        (seperator_index - search_start) != 0) {
      string new_segment = string_new_substring(
          s->data, search_start, seperator_index + seperator->length);
      dynamic_array_push_back(&result, &new_segment);
      search_start = seperator_index + seperator->length;
    }
  } while (seperator_index != string_invalid_index);

  return result;
}

string_range string_trim(string const *s, string const *cutset) {
  uint32_t new_start = 0;
  uint32_t new_end = s->length;

  for (uint32_t i = 0; i < s->length; ++i) {
    bool trim_this_index = false;

    for (uint32_t j = 0; j < cutset->length; ++j) {
      if (s->data[i] == cutset->data[j]) {
        trim_this_index = true;
        break;
      }
    }

    if (trim_this_index == false) {
      new_start = i;
      break;
    }
  }

  for (int32_t i = s->length - 1; i >= 0; --i) {
    bool trim_this_index = false;

    for (uint32_t j = 0; j < cutset->length; ++j) {
      if (s->data[i] == cutset->data[j]) {
        trim_this_index = true;
        break;
      }
    }

    if (trim_this_index == false) {
      new_end = i;
      break;
    }
  }

  return string_range_new((string *)s, new_start, new_end);
}

string_range string_trim_left(string const *s, string const *cutset) {
  uint32_t new_start = 0;
  uint32_t new_end = s->length;

  for (uint32_t i = 0; i < s->length; ++i) {
    bool trim_this_index = false;

    for (uint32_t j = 0; j < cutset->length; ++j) {
      if (s->data[i] == cutset->data[j]) {
        trim_this_index = true;
        break;
      }
    }

    if (trim_this_index == false) {
      new_start = i;
      break;
    }
  }

  return string_range_new((string *)s, new_start, new_end);
}

string_range string_trim_right(string const *s, string const *cutset) {
  uint32_t new_start = 0;
  uint32_t new_end = s->length;

  for (int32_t i = s->length - 1; i >= 0; --i) {
    bool trim_this_index = false;

    for (uint32_t j = 0; j < cutset->length; ++j) {
      if (s->data[i] == cutset->data[j]) {
        trim_this_index = true;
        break;
      }
    }

    if (trim_this_index == false) {
      new_end = i;
      break;
    }
  }

  return string_range_new((string *)s, new_start, new_end);
}
