#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "arrayList.h"

ArrayList* arrayListCreate() {
  ArrayList* list = (ArrayList*)malloc(sizeof(ArrayList)); 
  if (list == NULL)
    return NULL;
  list->data = NULL;
  list->size = 0;
  list->allocatedSize = 0;
  return list;
}

void arrayListFree(ArrayList* a) {
  if (a == NULL)
    return;
  free(a->data);
  free(a);
}

void arrayListExtend(ArrayList* a) {
  if (a == NULL)
    return;
  size_t newSize;
  if (a->allocatedSize > 0 && SIZE_MAX - a->allocatedSize < a->allocatedSize)
    newSize = SIZE_MAX;
  else
    newSize = a->allocatedSize > 0 ? a->allocatedSize * 2 : 1;
  void **newData = (void**)realloc(a->data, newSize * sizeof(void*));
  if (newData == NULL)
    return;
  a->data = newData;
  a->allocatedSize = newSize;
  for (size_t i = a->size; i < a->allocatedSize; ++i)
    a->data[i] = NULL;
}

bool arrayListPush(ArrayList* a, void *v) {
  if (a == NULL)
    return false;
  if (a->size >= a->allocatedSize)
    arrayListExtend(a);
  if (a->size >= a->allocatedSize)
    return false;
  a->data[a->size++] = v;
  return true;
}
