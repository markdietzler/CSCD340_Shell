#include "makeArgs.h"

void clean(int argc, char **argv){
  //free the array rows first
  for(int w = 0; w < argc; w++){
    free(argv[w]);
  }
  free(argv);//free the whole array
}// end clean

void printargs(int argc, char **argv){
	int x;
	for(x = 0; x < argc; x++)
		printf("%s\n", argv[x]);
}// end printargs

int makeargs(char *s, char*** argv){
  int count = 0;
  char copy[MAX];
  char * save = NULL; //bookmark pointer for strtok_r to save its place

  //make a copy of the incoming string to tokenize for the token count
  strcpy(copy, s);

  /*
  / do the token count
  */

  //prime read
  char * token = strtok_r(copy, " ", &save);
  //read the rest of the string
  while(token != NULL){
    count++;
    token = strtok_r(NULL, " ", &save);
  }
  
  //if an empty line was sent in, no use allocating memory
  if(count < 1){
    return -1;
  }
  
  //set up the pointer array
  (*argv) = (char **)calloc(count + 1, sizeof(char *));

  //do prime read on string s and put it in the array
  token = strtok_r(s, " ", &save);
  (*argv)[0] = (char*)calloc(strlen(token)+1, sizeof(char));
  strcpy((*argv)[0],token);

  //fill in rest of the array
  for(int x = 1; x < count; x++){
    token = strtok_r(NULL, " ", &save);
    (*argv)[x] = (char*)calloc(strlen(token)+1, sizeof(char));
    strcpy((*argv)[x],token);
  }

  return count;
}// end makeArgs
