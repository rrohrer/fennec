#include "utilities/file.h"
#include <stdio.h>

file_data file_load_all(string const *path) {
  FILE *ifp = fopen(path->data, "rb");
  if (!ifp) {
    return (file_data){NULL, 0};
  }

  file_data result;
  fseek(ifp, 0, SEEK_END);
  result.size = ftell(ifp);
  result.data = malloc(result.size);

  fseek(ifp, 0, SEEK_SET);
  fread(result.data, 1, result.size, ifp);

  fclose(ifp);

  return result;
}

void file_data_free(file_data *data) {
  free(data->data);
  data->size = 0;
}