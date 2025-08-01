#include <stdlib.h>
#include <string.h>
#include "djinni/util/array.h"

DjinniArray* djinni_array_initialize(int nElements) {
  DjinniArray* array = malloc(sizeof(DjinniArray));

  array->size = nElements;
  array->used = 0;
  array->data = malloc(sizeof(void*) * nElements);
  memset(array->data, 0, sizeof(sizeof(void*) * nElements));

  return array;
}

int djinni_array_insert(DjinniArray* array, void* data) {
  if (array->used == array->size) {
    array->size = (array->size * 3) / 2 + 8;
    array->data = (void**)(realloc(
      array->data, array->size * sizeof(void*)
    ));
  }

  int index = array->used;

  array->data[array->used++] = data;

  return index;
}

void djinni_array_remove_index(DjinniArray* array, int index) {
  /*
  array->data[index] = NULL;

  for (int i = index; i < array->used - 1; i++) {
    array->data[i] = array->data[i + 1];
  }

  array->used--;

  preserve in case order for drawing

  */
  if (index < 0 || index >= array->used) {
    return;
  }

  array->data[index] = array->data[array->used - 1];
  array->used--;
}

void djinni_array_delete_index(DjinniArray* array, int index, void (onDestroy)(void*)) {
  if(onDestroy != NULL) { onDestroy(array->data[index]); }

  array->data[index] = array->data[array->used - 1];
  array->used--;
}

void djinni_array_delete(DjinniArray* array, void* data, void (onDestroy)(void*)) {
  int index = -1;

  for (int i = 0; i < array->used; i++) {
    if (array->data[i] == data) {
      index = i;
      break;
    }
  }

  if (index < 0) {
    return;
  }

  if(onDestroy != NULL) { onDestroy(array->data[index]); }

  array->data[index] = NULL;

  for (int i = index; i < array->used - 1; i++) {
    array->data[i] = array->data[i + 1];
  }

  array->used--;
}

void djinni_array_inspect(DjinniArray* array) {

}

void djinni_array_destroy(DjinniArray* array, void (onDestroy)(void*)) {
  for (int i = 0; i < array->used; i++) {
    if(onDestroy != NULL) { onDestroy(array->data[i]); }
  }

  free(array->data);
  free(array);

  array = NULL;
}