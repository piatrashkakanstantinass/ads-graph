#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "arrayList.h"
#include "graph.h"

#define str(x) #x
#define xstr(x) str(x)

#define LINE_BUFFER_SIZE 1024

FILE *fp;
Graph *graph;

void skipLine(FILE *fp) {
  int c;
  while ((c = fgetc(fp)) != '\n' && c != EOF)
    continue;
}

char *readNodeName(FILE *fp) {
  char *buffer = (char*)malloc(LINE_BUFFER_SIZE+1);
  buffer[0] = '\0';
  if (fscanf(fp, "%" xstr(LINE_BUFFER_SIZE) "[^\n]", buffer) == EOF || buffer[0] == '\0') {
    free(buffer);
    return NULL;
  }
  skipLine(fp); 
  return buffer;
}

Node *findNode(char *str) {
  for (size_t i = 0; i < graph->nodes->size; ++i) {
    if (strcmp(((Node*)graph->nodes->data[i])->value, str) ==  0) {
      return graph->nodes->data[i];
    }
  }
  return NULL;
}

void readNodes() {
  char *nodeName;
  while ((nodeName = readNodeName(fp))) {
    if (findNode(nodeName)) {
      free(nodeName);
      continue;
    }
    Node *node = (Node*)malloc(sizeof(Node));
    node->value = nodeName;
    graphAddNode(graph, node);
  }
}

void readConnections() {
  while (true) {
    int c = fgetc(fp);
    if (c == EOF)
      return;
    if (c == '\n')
      continue;
    else {
      ungetc(c, fp);
      char *n1Name = readNodeName(fp); 
      char *n2Name = readNodeName(fp);
      if (!n1Name || !n2Name) {
        free(n1Name);
        free(n2Name);
        return;
      }
      Node* n1 = findNode(n1Name);
      Node* n2 = findNode(n2Name);
      if (!n1 || !n2) {
        free(n1Name);
        free(n2Name);
        continue;
      }
      graphCreateDirectionalConnection(graph, n1, n2);
    }
  }
}

Node *askForNode(char *msg) {
  while (true) {
    printf("%s", msg);
    char *buffer = (char*)malloc(LINE_BUFFER_SIZE + 1);
    scanf("%" xstr(LINE_BUFFER_SIZE) "[^\n]", buffer);
    Node *node = findNode(buffer);
    if (node == NULL) {
      printf("This node does not exist\n");
      int c;
      while ((c = getchar()) != EOF && c != '\n')
        continue;
      continue;
    }
    int c;
    while ((c = getchar()) != EOF && c != '\n')
      continue;
    return node;
  }
}


int main(int argc, char **argv) {
  printf("ADS4 solution. Usage: main.exe data_file\nMake sure to use same format as included data file.\n");
  if (argc == 1)
    return 1;
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("Could not open data file\n");
    return 1;
  }
  graph = graphCreate();
  readNodes();
  readConnections();
  // for (size_t i = 0; i < graph->nodes->size; ++i) {
  //   for (size_t j = 0; j < graph->nodes->size; ++j) {
  //     printf("%p ", ((ArrayList*)graph->matrix->data[i])->data[j]);
  //   }
  //   printf("\n");
  // }
  printf("You can check node names in data file\n");
  Node *n1 = askForNode("Enter first node (subject): ");
  Node *n2 = askForNode("Enter second node (subject that should be listened only after first subject): ");
  if (graphNodeComesBefore(graph, n1, n2)) {
    printf("You should listen to first subject before second\n");
  } else {
    printf("You do not have to listen to first subject before second\n");
  }
  graphFree(graph);
  fclose(fp);
  return 0;
}
