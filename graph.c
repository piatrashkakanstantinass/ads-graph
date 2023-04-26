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
  for (size_t i = 0; i < g->nodes->size - 1; ++i) {
    arrayListPush(g->matrix->data[i], DISCONNECTED);
  }
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

#include <stdio.h>

void graphCreateDirectionalConnection(Graph *g, Node *from, Node *to) {
  if (g == NULL)
    return;
  size_t fromIndex = 0;
  size_t toIndex = 0;
  bool fromFound = false;
  bool toFound = false;
  for (size_t i = 0; i < g->nodes->size; ++i) {
    if (g->nodes->data[i] == from) {
      fromFound = true;
      fromIndex = i;
    }
    if (g->nodes->data[i] == to) {
      toFound = true;
      toIndex = i;
    }
  }
  if (!(fromFound & toFound))
    return;
  ((ArrayList*)g->matrix->data[fromIndex])->data[toIndex] = CONNECTED;
}

size_t indexOfNode(Graph *g, Node * n) {
  if (g == NULL)
    return -1;
  for (size_t i = 0; i < g->nodes->size; ++i) {
    if (g->nodes->data[i] == n)
      return i;
  }
  return -1;
}

Node *nodeOfIndex(Graph *g, size_t index) {
  if (g == NULL || g->nodes->size <= index)
    return NULL;
  return g->nodes->data[index];
}

void dfs(Graph *g, Node *n, Node **visited, size_t *visitedCnt) {
    for (size_t i = 0; i < *visitedCnt; ++i) {
      if (visited[i] == n)
        return;
    }
    visited[(*visitedCnt)++] = n;
    size_t index = indexOfNode(g, n);
    for (size_t i = 0; i < g->nodes->size; ++i) {
      if (((ArrayList*)g->matrix->data[index])->data[i] == CONNECTED)
        dfs(g, nodeOfIndex(g, i), visited, visitedCnt);
    } 
}

bool graphNodeComesBefore(Graph *g, Node *n1, Node *n2) {
  if (!g || !n1 || !n2 || g->nodes->size <= 0 || indexOfNode(g, n1) < 0 || indexOfNode(g, n2) < 0) 
    return false;
  Node **visited = (Node**)malloc(sizeof(Node*)*g->nodes->size);
  if (visited == NULL)
    return false;
  size_t visitedCnt = 0;
  dfs(g, n1, visited, &visitedCnt);
  bool found = false;
  for (size_t i = 0; i < visitedCnt; ++i) {
    if (visited[i] == n2)
      found = true;
  }
  free(visited);
  return found;
}
