#ifndef COMLIST
#define COMLIST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

struct comnode {

int index;
char * command;
struct comnode * next;
struct comnode * prev;
};
typedef struct comnode ComNode;

struct comlist {
  ComNode * head;
  int size;
  int max;
};
typedef struct comlist ComList;

ComList * comlist();

void pushCom(ComList * theList, int sentInIndex, char *incomingCommand);

void bangCom(ComList * theList, char *returnedCommand, int rewindIndex);

void cleanComList(ComList * theList);

void printComList(ComList * theList, int historySize);

void dumpComList(ComList * theList, int historyFileSize);

#endif
