#ifndef DJINNI_UTIL_MEMORY
#define DJINNI_UTIL_MEMORY 1

#include "djinni/util/array.h"

#ifndef DJINNI_DEBUG_MEMORY

#define DJINNI_MALLOC(X) malloc(X)
#define DJINNI_REALLOC(X, S) realloc(X, S)
#define DJINNI_FREE(X) free(X)

#else

#define DJINNI_MALLOC(X) Djinni_Util_Memory.malloc_trace( X, __FILE__, __LINE__, __FUNCTION__)
#define DJINNI_REALLOC(X, S) Djinni_Util_Memory.realloc_trace(X, S, __FILE__, __LINE__, __FUNCTION__)
#define DJINNI_FREE(X) Djinni_Util_Memory.free_trace( X, __FILE__, __LINE__, __FUNCTION__)

#endif

typedef struct Djinni_Util_MemoryItemStruct {
  const char *file;
  int line;
  const char *func;
  void *pp;
} DjinniMemoryItem;

struct Djinni_Util_MemoryStruct {
  DjinniArray* elements;

  void (*initialize)();

  void* (*malloc_trace)(size_t size, const char *file, int line, const char *func);
  void* (*realloc_trace)(void *pp, size_t size, const char *file, int line, const char *func);
  void  (*free_trace)(void *pp, const char *file, int line, const char *func);
  void  (*inspect)();

  void (*destroy)();
};

extern struct Djinni_Util_MemoryStruct Djinni_Util_Memory;

#endif