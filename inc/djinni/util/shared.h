#ifndef DJINNI_UTIL_SHARED
#define DJINNI_UTIL_SHARED 1

#include <stdlib.h>
#include <string.h>

struct Djinni_Util_LoggerStruct;
typedef struct DjinniArrayStruct DjinniArray;
typedef struct Djinni_Util_MemoryItemStruct DjinniMemoryItem;

char* djinniReadFile(char *filename);

#endif