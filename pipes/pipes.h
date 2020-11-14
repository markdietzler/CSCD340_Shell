#ifndef PIPES_H
#define PIPES_H

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>


int containsPipe(char *s);
char ** parsePrePipe(char *s, int * preCount);
char ** parseMidPipe(char *s, int * postCount);
char ** parsePostPipe(char *s, int * postCount);
void twoPipe(char *incomingCommand);
void onePipe(char *incomingCommand);
void noPipe(char *incomingCommand);
void forkIt(char ** argv);

#endif 
