#include "djinni/util/shared.h"
#include "djinni/util/logger.h"
#include "djinni/util/array.h"

static DjinniArray* initialize(int nElements) {
  Djinni_Util_Logger.log_dev("Djinni::Util::Array.initialize(size: %d)", nElements);

  DjinniArray* array = malloc(sizeof(DjinniArray));

  array->size = nElements;
  array->used = 0;
  array->data = malloc(sizeof(void*) * nElements);
  memset(array->data, 0, sizeof(sizeof(void*) * nElements));

  return array;
}

static void insert(DjinniArray* array, void* data) {
  Djinni_Util_Logger.log_dev("Djinni::Util::Array.insert(array:(%p), element:(%p))", array, data);

  if (array->used == array->size) {
    Djinni_Util_Logger.log_debug("\tDjinni::Util::Array.insert(resizing array)");
    array->size = (array->size * 3) / 2 + 8;
    array->data = (void**)(realloc(
      array->data, array->size * sizeof(void*)
    ));
  }

  array->data[array->used++] = data;
}

static void delete(DjinniArray* array, void* data, void (onDestroy)(void*)) {
  Djinni_Util_Logger.log_dev("Djinni::Util::Array.delete(array:(%p), element:(%p))", array, data);

  int index = -1;

  for (int i = 0; i < array->used; i++) {
    if (array->data[i] == data) {
      index = i;
      break;
    }
  }

  if (index < 0) {
    Djinni_Util_Logger.log_warn("Djinni::Util::Array.delete(array:(%p), element:(%p)) - element not found", array, data);
    return;
  }

  if(onDestroy != NULL) { onDestroy(array->data[index]); }

  array->data[index] = NULL;

  for (int i = index; i < array->used - 1; i++) {
    array->data[i] = array->data[i + 1];
  }

  array->used--;
}

static void inspect(DjinniArray* array) {
  Djinni_Util_Logger.log_debug("Djinni::Util::Array(address:(%p), size:(%d), used: (%d))", array, array->size, array->used);
  for (int i = 0; i < array->used; i++) {
    Djinni_Util_Logger.log_debug("\tDjinni::Util::Array::Element(address:(%p))", array->data[i]);
  }
}

static void destroy(DjinniArray* array, void (onDestroy)(void*)) {
  Djinni_Util_Logger.log_dev("Djinni::Util::Array.destroy(array:(%p))", array);

  for (int i = 0; i < array->used; i++) {
    if(onDestroy != NULL) { onDestroy(array->data[i]); }
  }

  free(array);

  array = NULL;
}

struct Djinni_Util_ArrayStruct Djinni_Util_Array = {
  .initialize = initialize,
  .insert = insert,
  .delete = delete,
  .inspect = inspect,
  .destroy = destroy
};