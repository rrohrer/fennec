/**
 * @file
 * @author Ryan Rohrer <ryan.rohrer@gmail.com>
 *
 * @section DESCRIPTION
 * A collection of utilities for operating on strings + a wrapper for a
 * slightly more sophisticated string.
 */
#ifndef string_h
#define string_h

#include "data_structures/dynamic_array.h"
#include "fennec.h"

/**
 * A string that is null terminated, as well as having a cached
 * length and capacity.
 *
 * Length is what strlen() would return. Capacity is the size allocated
 * (includes null)
 */
typedef struct {
  char *data;
  uint32_t length;
  uint32_t capacity;
} string;

/**
 * A range in a string.  This describes several characters contained in a
 * string.
 *
 * Start is inclusive and end is exclusive.
 */
typedef struct {
  string *data;
  uint32_t start;
  uint32_t end;
} string_range;

/**
 * Various constants for the string functions.
 */
typedef enum {
  string_invalid_index = -1,
  string_less_than = -1,
  string_equal = 0,
  string_greater_than = 1
} string_constants;

/**
 * Create a string from a C string.
 *
 * @param data - the C string to turn into a string.
 * @return - a new string containing the contents of that C string.
 */
string string_new(char const *data);

/**
 * Create a string range from a string + range.
 *
 * @param s - the string that this range will reference.
 * @param start - the first character of the range that this spans.
 * @param end - one past the last element of the range this spans.
 * @return - a string_range that references a slice of string.
 */
string_range string_range_new(string *s, uint32_t start, uint32_t end);

/**
 * Create a string from a buffer and a range. This does not have to be
 * null terminated.
 *
 * @param data - thr buffer that a range is being pulled from.
 * @param start - the index of th first character to take.
 * @param end - the EXCLUSIVE end of the string. The element at this index will
 * not get copied.
 * @return - a string built from the range included.
 */
string string_new_substring(char const *data, uint32_t start, uint32_t end);

/**
 * Create a string from a cstring. This DOES NOT ALLOCATE so do not free it.
 *
 * @param cstring - the c string to wrap in the string struct.
 * return - the string that points to the cstring.
 */
string string_wrap_cstring(char const *cstring);

/**
 * Deallocate and clean up a string.
 *
 * @param s - the string to be deallocated.
 */
void string_free(string *s);

/**
 * Append a string to another string.
 *
 * @param destination - pointer to a string that will get appended to.
 * @param source - the content that is getting appended.
 */
void string_append(string *destination, string const *source);

/**
 * Checks if a string ever contains a substring.
 *
 * @param s - the string that will be checked.
 * @param search - the string to search for.
 * @return - true if 'search' was ever found.
 */
bool string_contains(string const *s, string const *search);

/**
 * Returns the lexographical comparison of two stings.
 *
 * @param s1 - the first string to compare.
 * @param s2 - the second string to compare.
 * @return - string_equal if they are the same, string_less_than if s1 < s2,
 *           string_greater_than if s1 > s2.
 */
int32_t string_compare(string const *s1, string const *s2);

/**
 * Returns the lexographical comparison of two sting_ranges.
 *
 * @param r1 - the first string_range to compare.
 * @param r2 - the second string_range to compare.
 * @return - string_equal if they are the same, string_less_than if r1 < r2,
 *           string_greater_than if r1 > r2.
 */
int32_t string_range_compare(string_range const *r1, string_range const *r2);

/**
 * Find the first instance of a substring in the string.
 *
 * @param s - the string to search.
 * @param substring - the substring to be searched for.
 * @param start - the point in the string to start searching from.
 * @return - the index of the first element of substring in string, or
 * string_invalid_index if not found.
 */
int32_t string_find_first(string const *s, string const *substring,
                          uint32_t start);

/**
 * Find the first instance of a any character in character_set in the string.
 *
 * @param s - the string to search.
 * @param character_set - the set of characters to be searched for.
 * @param start - the point in the string to start searching from.
 * @return - the index of the first element of substring in string, or
 * string_invalid_index if not found.
 */
int32_t string_find_first_any(string const *s, string const *character_set,
                              uint32_t start);

/**
 * Find the last instance of a substring in the string.
 *
 * @param s - the string to search.
 * @param substring - the substring to be searched for.
 * @param start - the point in the string to start searching (left) from.
 * @return - the index of the first element of substring in string, or
 * string_invalid_index if not found.
 */
int32_t string_find_last(string const *s, string const *substring,
                         uint32_t start);

/**
 * Find the last instance of a any character in character_set in the string.
 *
 * @param s - the string to search.
 * @param character_set - the set of characters to be searched for.
 * @param start - the point in the string to start searching (left) from.
 * @return - the index of the first element of substring in string, or
 * string_invalid_index if not found.
 */
int32_t string_find_last_any(string const *s, string const *character_set,
                             uint32_t start);

/**
 * Joins and array of strings into one new string.
 *
 * @param strings - a pointer to an array of strings.
 * @param separator - the string to use between elements of the array.
 * @param count - the length of the array that is getting joined, in elements.
 * @return - a string that contains all of the substrings concatinated with the
 * separator.
 */
string string_join(string const *strings, string const *separator,
                   uint32_t count);

/**
 * Replaces all instances of search with the contents of replace_with.
 * @param s - the original string to have items replaced.
 * @param search - the search string to lookup and replace in s.
 * @param replace_with - the string getting inserted wherever search is found.
 * @return - a new string with all instances of search replaced with
 * replace_with.
 */
string string_replace(string const *s, string const *search,
                      string const *replace_with);

/**
 * Split a string by seperator.  Creates a dynamic array of string_range's
 * That include all the substrings and NOT the seperator.
 *
 * @param s - the string that will be split up.
 * @param seperator - the substring to split by, and that will be stripped.
 * @return - a dynamic_array of string_range structs that contain all the
 * splits. NOTE: caller must call dynamic_array_free when done.
 */
dynamic_array string_split(string const *s, string const *seperator);

/**
 * Split a string by any character in the split set, dropping all elements of
 * the split set.
 *
 * @param * s - the string to split.
 * @param * character_set - the characters to split on, and drop.
 * @return - a dynamic_array containing string_range structs that contain the
 * splits. NOTE: caller must call dynamic_array_free when done.
 */
dynamic_array string_split_any(string const *s, string const *character_set);

/**
 * Split a string after the seperator (and include it).
 *
 * @param s - the string to be split up.
 * @param seperator - the seperator to split after.
 * @return - a dynamic_array of string_range's that contain all the splits +
 * seperators. NOTE: caller must call dynamic_array_free when done.
 */
dynamic_array string_split_after(string const *s, string const *seperator);

/**
 * Removes all elments in cutset to the left AND right of the given string.
 *
 * @param s - the string to be trimmed.
 * @param cutset - the characters that will be trimmed from the string.
 * @return - a string_range that references the the trimmed string.
 */
string_range string_trim(string const *s, string const *cutset);

/**
 * Removes all elments in cutset to the left of the given string.
 *
 * @param s - the string to be trimmed.
 * @param cutset - the characters that will be trimmed from the string.
 * @return - a string_range that references the the trimmed string.
 */
string_range string_trim_left(string const *s, string const *cutset);

/**
 * Removes all elments in cutset to the right of the given string.
 *
 * @param s - the string to be trimmed.
 * @param cutset - the characters that will be trimmed from the string.
 * @return - a string_range that references the the trimmed string.
 */
string_range string_trim_right(string const *s, string const *cutset);

#endif