#include "comlist.h"

ComList * comlist(){

  ComList* newList = (ComList*)calloc(1, sizeof(ComList));

  ComNode* nn = (ComNode*)calloc(1,sizeof(ComNode));
  newList->head = nn;
  newList->head->prev = newList->head;
  newList->head->next = newList->head;
  newList->max = 0;
  newList->size = 0;
  return newList;

}

void pushCom(ComList * theList, int sentInIndex, char *incomingCommand){

  if(theList == NULL) {
    exit(-99);
  }

  if(incomingCommand == NULL){
    exit(-99);
  }

  //sets up the new node

  ComNode* nn = (ComNode*)calloc(1, sizeof(ComNode));
  nn->index = sentInIndex;
  nn->command = (char*)calloc(strlen(incomingCommand) + 1, sizeof(char));
  strcpy(nn->command, incomingCommand);

  //now we start the add process
  if(theList->size == 0){
    nn->next = theList->head;
    nn->prev = theList->head;
    theList->head->next = nn;
    theList->head->prev = nn;
    theList->size = theList->size + 1;
  } else {
    ComNode *cur = theList->head->prev;
    nn->next = theList->head;
    nn->prev = cur;
    cur->next = nn;
    theList->head->prev = nn;    
    theList->size = theList->size + 1;
  }

}

void bangCom(ComList * theList, char *returnedCommand, int rewindIndex){

  if(theList == NULL) {
    exit(-99);
  }
  if(rewindIndex == 1){
    ComNode* prev = theList->head->prev;

    strcpy(returnedCommand, prev->command);
  }
  else if(rewindIndex > 1){
    if(rewindIndex > theList->size){
      printf("mssh: !-%d: event not found\n", rewindIndex);
    } else { //rewind goes back farther than 1
      ComNode* prev = theList->head->prev;
      while(rewindIndex != 0){
        rewindIndex = rewindIndex - 1;
        prev = prev->prev;
      } //end of rewind button

    strcpy(returnedCommand, prev->command);
    } //end inner if
  } //end else if
}

void cleanComList(ComList * theList){

  if(theList == NULL) {
    exit(-99);
  }

  if(theList != NULL){
    if(theList->size > 2){
      ComNode *prev = theList->head->next;
      ComNode *cur = prev->next;
      ComNode *temp = NULL;
      theList->head->next = theList->head;
      theList->head->prev = theList->head;
      while(cur->next != theList->head){
        temp = prev;
        free(prev->command);
        prev = cur;
        free(temp);
        cur = cur->next;
      }
      //free two remaining nodes
      free(prev->command);
      free(cur->command);
      free(prev); 
      free(cur);
    }

    if(theList->size == 2){
      ComNode *prev = theList->head->next;
      ComNode *cur = prev->next;
      theList->head->next = theList->head;
      theList->head->prev = theList->head;
      free(prev->command);
      free(prev);
      free(cur->command);
      free(cur);
    }

    if(theList->size == 1){
      ComNode *prev = theList->head->next;
      theList->head->next = theList->head;
      theList->head->prev = theList->head;
      free(prev->command);
      free(prev);
    }
    free(theList->head);
    theList->size = 0;
    theList->max = 0;
  } else { 
    //only reach here if passed in list is NULL
    perror("List is NULL!\n");
  }

}

void printComList(ComList * theList, int historySize){

  if(theList == NULL) {
    exit(-99);
  }


  if(theList->size <= historySize){
    ComNode *cur = theList->head->next;
    while(cur != theList->head){
      printf("%d %s\n", cur->index, cur->command);
      cur = cur->next;
    } //end of while
  
  } else {
    ComNode *cur = theList->head->next;
    for(int u = 0; u < (theList->size - historySize); u++){
      cur = cur->next;
    }
    while(cur != theList->head){
      printf("%d %s\n", cur->index, cur->command);
      cur = cur->next;
    } //end of while
  }

}

void dumpComList(ComList * theList, int historyFileSize){

  if(theList == NULL) {
    exit(-99);
  }

  if(historyFileSize < 0){
    exit(-99);
  }

  FILE *p_msshrc_history;
  p_msshrc_history = fopen(".msshrc_history","w");

  if(p_msshrc_history == NULL){
    perror("No write permissions in present directory!\n");
    exit(-99);
  }

  if(theList->size <= historyFileSize){

    ComNode *cur = theList->head->next;

    while(cur != theList->head){
      fprintf(p_msshrc_history, "%-4d %s\n", cur->index, cur->command);
      cur = cur->next;
    } //end of while loop

  } else {
    //advance cur to the correct starting spot
    ComNode *cur = theList->head->next;
    for(int y = 0; y < (theList->size-historyFileSize); y++){
      cur = cur->next;
    }

    while(cur != theList->head){
      fprintf(p_msshrc_history, "%-4d %s\n", cur->index, cur->command);
      cur = cur->next;
    } //end of while loop

  }

  //close file if open
  int checkMSSHRC_H_File = fclose(p_msshrc_history);
  if(checkMSSHRC_H_File == 1){
   perror("Error closing history file!");
  }

}
