#ifndef GRAPH_H
#define GRAPH_H
#include <stdbool.h>
#include "arrayList.h"

typedef struct {
  ArrayList *matrix;
  ArrayList *nodes;
} Graph;

typedef struct {
  char *value;
} Node;

Graph *graphCreate();
void graphFree(Graph *g);
bool graphAddNode(Graph *g, Node *n);
void graphCreateDirectionalConnection(Graph *g, Node *from, Node *to);
bool graphNodeComesBefore(Graph *g, Node *n1, Node *n2);
#endif
