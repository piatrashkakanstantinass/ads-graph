#include <stdlib.h>
#include <stdbool.h>
#include "arrayList.h"
#include "graph.h"

#define CONNECTED    (void*)1
#define DISCONNECTED (void*)0

Graph *graphCreate() {
  Graph *graph = (Graph*)malloc(sizeof(Graph));
  if (graph == NULL)
    return NULL;
  graph->matrix = arrayListCreate();
  graph->nodes = arrayListCreate();
  if (!(graph->matrix && graph->nodes)) {
    free(graph->matrix);
    free(graph->nodes);
    free(graph);
    return NULL;
  }
  return graph; 
}

void graphFree(Graph *g) {
  if (g == NULL)
    return;
  arrayListFree(g->nodes);
  for (int i = 0; i < g->matrix->size; ++i) {
    arrayListFree(g->matrix->data[i]);
  }
  arrayListFree(g->matrix);
  free(g);
}

bool increaseMatrix(Graph *g) {
  if (g == NULL)
    return false;
  int newSize = g->matrix->size + 1;
  if (newSize <= 0)
    return false;
  ArrayList* row = arrayListCreate();
  if (row == NULL)
    return false;
  for (int i = 0; i < newSize; ++i) {
    if (!arrayListPush(row, DISCONNECTED)) {
      arrayListFree(row);
      return false;
    }
  }
  if (!arrayListPush(g->matrix, row)) {
    arrayListFree(row);
    return false;
  }
  return true;
}

bool graphAddNode(Graph *g, Node *n) {
  if (g == NULL)
    return false;
  for (int i = 0; i < g->nodes->size; ++i) {
    if (g->nodes->data[i] == n)
      return false;
  } 
  if (!arrayListPush(g->nodes, n))
    return false;
  if (!increaseMatrix(g)) {
    --g->nodes->size;
    return false;
  }
  return true;
}

void graphCreateDirectionalConnection(Graph *g, Node *from, Node *to) {
  if (g == NULL)
    return;
  size_t fromIndex = 0;
  size_t toIndex = 0;
  bool fromFound = false;
  bool toFound = false;
  for (size_t i = 0; i < g->nodes->size; ++i) {
    if (g->nodes->data[fromIndex] == from) {
      fromFound = true;
      fromIndex = i;
    }
    if (g->nodes->data[toIndex] == to) {
      toFound = true;
      toIndex = i;
    }
  }
  if (!(fromFound & toFound))
    return;
  ((ArrayList*)g->matrix->data[fromIndex])->data[toIndex] = CONNECTED;
}

bool graphNodeComesBefore(Graph *g, Node *n1, Node *n2) {
  
}
