#include "djinni/util/util.h"

static void initialize() {
  Djinni_Util_Memory.elements = Djinni_Util_Array.initialize(16);
}

static void* malloc_trace(size_t size, const char *file, int line, const char *func) {
  return NULL;
}

static void* realloc_trace(void *pp, size_t size, const char *file, int line, const char *func) {
  return NULL;
}

static void free_trace(void *pp, const char *file, int line, const char *func) {

}

static void inspect() {

}

static void destroy() {
  Djinni_Util_Array.destroy(Djinni_Util_Memory.elements, NULL);
}

struct Djinni_Util_MemoryStruct Djinni_Util_Memory = {
  .initialize = initialize,
  .malloc_trace = malloc_trace,
  .realloc_trace = realloc_trace,
  .free_trace = free_trace,
  .inspect = inspect,
  .destroy = destroy
};