/**
 * @file
 * @author Ryan Rohrer <ryan.rohrer@gmail.com>
 *
 * @section DESCRIPTION
 * Utilities to help with dealing with files.
 */
#ifndef file_h
#define file_h

#include "fennec.h"
#include "utilities/string.h"

/**
 * A memory mapped file.
 */
typedef struct {
  void *data;
  uint32_t size;
} file_data;

/**
 * Load all of a file into ram.
 *
 * @param path - the path of the file to load.
 * @return - the file_data containing a memory mapped file.
 */
file_data file_load_all(string const *path);

/**
 * Clean up the filedata that was loaded.
 *
 * @param data - the filedata that will be cleaned up.
 */
void file_data_free(file_data *data);

#endif