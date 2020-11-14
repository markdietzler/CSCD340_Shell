#ifndef ALIASLIST
#define ALIASLIST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

struct aliasnode {
  char * command;
  char * alias;
  struct aliasnode * next;
  struct aliasnode * prev;
};
typedef struct aliasnode ANode;

struct aliaslist {
  ANode * head;
  int size;
};
typedef struct aliaslist AList;

AList * alistlinkedlist();

void pushAlias(AList * theList, char* incomingCommand, char* incomingAlias);

void updateAlias(AList * theList, char* incomingCommand, char* incomingAlias);

void peekCommand(AList * theList, char* incomingAlias, char *returnedCommand);

void unAlias(AList * theList, const char *incomingAlias);

void cleanList(AList * theList);

void printList(AList * theList);

#endif
