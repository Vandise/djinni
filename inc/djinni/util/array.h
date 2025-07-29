#ifndef DJINNI_UTIL_ARRAY
#define DJINNI_UTIL_ARRAY 1

typedef struct DjinniArrayStruct {
  int size;
  int used;

  void** data;
} DjinniArray;

DjinniArray* djinni_array_initialize(int nElements);
int djinni_array_insert(DjinniArray* array, void* data);
void djinni_array_removeIndex(DjinniArray* array, int index);
void djinni_array_delete(DjinniArray* array, void* data, void (onDestroy)(void*));
void djinni_array_inspect(DjinniArray* array);
void djinni_array_destroy(DjinniArray* array, void (onDestroy)(void*));

#endif