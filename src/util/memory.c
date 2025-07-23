#include "djinni/util/util.h"

static void initialize() {
  Djinni_Util_Memory.elements = Djinni_Util_Array.initialize(16);
}

static void* malloc_trace(size_t size, const char *file, int line, const char *func) {
  void *p = malloc(size);

  DjinniMemoryItem *itm = malloc(sizeof(DjinniMemoryItem));
  itm->file = file; itm->line = line; itm->func = func;
  itm->pp = p;

  Djinni_Util_Array.insert(Djinni_Util_Memory.elements, itm);

  return p;
}

static void* realloc_trace(void *pp, size_t size, const char *file, int line, const char *func) {
  void *p = realloc(pp, size);
  Djinni_Util_Array.delete(Djinni_Util_Memory.elements, pp, NULL);

  DjinniMemoryItem *itm = malloc(sizeof(DjinniMemoryItem));
  itm->file = file; itm->line = line; itm->func = func;
  itm->pp = p;

  Djinni_Util_Array.insert(Djinni_Util_Memory.elements, itm);

  return p;
}

static void free_trace(void *p, const char *file, int line, const char *func) {
  Djinni_Util_Array.delete(Djinni_Util_Memory.elements, p, NULL);

  free(p);

  p = NULL;
}

static void inspect() {
  for (int i = 0; i < Djinni_Util_Memory.elements->used; i++) {
    DjinniMemoryItem *itm = Djinni_Util_Memory.elements->data[i];
    printf("Potential Leaked Memory Item: %s, %i, %s, %p\n", itm->file, itm->line, itm->func, itm->pp);
  }
}

static void destroy() {
  Djinni_Util_Array.destroy(Djinni_Util_Memory.elements, free);
}

struct Djinni_Util_MemoryStruct Djinni_Util_Memory = {
  .initialize = initialize,
  .malloc_trace = malloc_trace,
  .realloc_trace = realloc_trace,
  .free_trace = free_trace,
  .inspect = inspect,
  .destroy = destroy
};