/**
 * @file
 * @author Ryan Rohrer <ryan.rohrer@gmail.com>
 *
 * @section DESCRIPTION
 * A collection of helper functions that deal with operating on filesystem
 * paths.
 */
#ifndef path_h
#define path_h

#include "fennec.h"
#include "utilities/string.h"

/**
 * Check to see if a given path exists in the filesystem. This works for files
 * and folders.
 *
 * @param path - the path to check and see if it exists.
 * @return - true if it exists, false if it does not.
 */
bool path_exists(string const *path);

/**
 * Return the slash character for the system this is running on.
 *
 * @return - either / or \ depending on if it's windows or UNIX.
 */
char path_get_system_slash(void);

/**
 * Combine two paths into a longer path.
 *
 * @param first - the beginning of the path that will be appended to.
 * @param second - the path that will be appended onto the base path.
 * @return - a string that contains the complete path.  Must call string_free on
 * it when done.
 */
string path_join(string const *first, string const *second);

/**
 * Convert a path to the slashes of the system this is running on.
 *
 * @param path - the path that will be converted.
 * @return - a string containing the path with the correct system slashes. Must
 * call string_free when done.
 */
string path_to_system_slashes(string const *path);

#endif