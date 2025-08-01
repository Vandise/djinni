#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* djinni_read_file(char *filename) {
  char *buffer = NULL;
  long  length;
  FILE *file;
  
  file = fopen(filename, "rb");
  
  if (file) {
  	fseek(file, 0, SEEK_END);
  	length = ftell(file);
  	fseek(file, 0, SEEK_SET);
  
  	buffer = malloc(length);
  	memset(buffer, 0, length);
  	fread(buffer, 1, length, file);
  
  	fclose(file);
  }
  
  return buffer;
}