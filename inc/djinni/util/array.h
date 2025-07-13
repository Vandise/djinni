#ifndef DJINNI_UTIL_ARRAY
#define DJINNI_UTIL_ARRAY 1

typedef struct DjinniArrayStruct {
  int size;
  int used;

  void** data;
} DjinniArray;

struct Djinni_Util_ArrayStruct {
  DjinniArray* (*initialize)(int);
  int (*insert)(DjinniArray*, void*);
  void (*delete)(DjinniArray*, void*, void (onDestroy)(void*));
  void (*removeIndex)(DjinniArray*, int);
  void (*inspect)(DjinniArray*);
  void (*destroy)(DjinniArray*, void (*onDestroy)(void*));
};

extern struct Djinni_Util_ArrayStruct Djinni_Util_Array;

#endif