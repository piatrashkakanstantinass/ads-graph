#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  void **data;
  size_t size;
  size_t allocatedSize;
} ArrayList;

ArrayList* arrayListCreate();
void arrayListFree(ArrayList* a);
bool arrayListPush(ArrayList* a, void *v);
#endif
