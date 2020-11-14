#include "aliaslist.h"

AList * alistlinkedlist(){
  
  AList* newList = (AList*)calloc(1,sizeof(AList));

  ANode* nn = (ANode*)calloc(1,sizeof(ANode));

  newList->head = nn;
  newList->head->next = newList->head;
  newList->head->prev = newList->head;
  newList->size = 0;

  return newList;
}

void pushAlias(AList * theList, char* incomingCommand, char* incomingAlias){

  if(theList == NULL) {
    exit(-99);
  }

  if(incomingCommand == NULL){
    exit(-99);
  }

  if(incomingAlias == NULL){
    exit(-99);
  }

  //sets up the new node

  ANode* nn = (ANode*)calloc(1, sizeof(ANode));
  nn->command = (char*)calloc(strlen(incomingCommand) + 1, sizeof(char));
  strcpy(nn->command, incomingCommand);
  nn->alias = (char*)calloc(strlen(incomingAlias) + 1, sizeof(char));
  strcpy(nn->alias, incomingAlias);



  //now we start the add process
  if(theList->size == 0){
    nn->prev = theList->head;
    nn->next = theList->head;
    theList->head->next = nn;
    theList->head->prev = nn;
    theList->size = theList->size + 1;
  } else {
    ANode *prev = theList->head->prev;
    nn->next = theList->head;
    nn->prev = prev;
    prev->next = nn;
    theList->head->prev = nn;
    theList->size = theList->size + 1;
  }


}

void updateAlias(AList * theList, char* incomingCommand, char* incomingAlias){

  if(theList == NULL) {
    exit(-99);
  }

  ANode* cur = theList->head->next;
  while(cur != theList->head){
    if(strcmp(cur->alias, incomingAlias) == 0) {
      free(cur->command);
      cur->command=(char*)calloc(sizeof(incomingCommand) + 1,sizeof(char));
      strcpy(cur->command, incomingCommand);
      break;
    } //end of if
    cur = cur->next;
  } //end of while
  cur = NULL;

}

/*
*  Given an alias, returns the associated command from the alias linkedlist
*  if it exists in the list
*/
void peekCommand(AList * theList, char* incomingAlias, char *returnedCommand){

  if(theList == NULL) {
    exit(-99);
  }
  
  ANode* cur = theList->head->next;
  while(cur != theList->head){
    if(strcmp(cur->alias, incomingAlias) == 0) {
      strcpy(returnedCommand, cur->command);
      break;
    }
    cur = cur->next;
  } //end of while
  cur = NULL;
}

/**
*  Given an alias, it removes the associated entry in the alias
*   linked list if found
*/
void unAlias(AList * theList, const char *incomingAlias){
  int foundFlag = 0;
  ANode* cur = theList->head->next;
  while(cur != theList->head){
    if(strcmp(cur->alias, incomingAlias) == 0) {
      foundFlag = 1;
      break;
    }
    cur = cur->next;
  } //end of while

  if(foundFlag == 1){
    ANode* prev = cur->prev;
    ANode* temp = cur;
    cur = cur->next;
    prev->next = cur;
    cur->prev = prev;
    temp->next = NULL;
    temp->prev = NULL;
    theList->size = theList->size - 1;
    free(temp->command);
    free(temp->alias);
    free(temp);
    prev = NULL;
  } else {
    printf("alias '%s' not found\n", incomingAlias);
  }

  cur = NULL; //reset cur  
}

void cleanList(AList * theList){

  if(theList == NULL) {
    exit(-99);
  }

  if(theList != NULL){
    if(theList->size > 2){
      ANode *prev = theList->head->next;
      ANode *cur = prev->next;
      ANode *temp = NULL;
      theList->head->next = theList->head;
      theList->head->prev = theList->head;
      while(cur->next != theList->head){
        temp = prev;
        free(prev->command);
        free(prev->alias);
        prev = cur;
        free(temp);
        cur = cur->next;
      }
      //free two remaining nodes
      free(prev->command);
      free(prev->alias);
      free(cur->command);
      free(cur->alias);
      free(prev); 
      free(cur);
      //free(temp);
    } //end of list is bigger than 2

    if(theList->size == 2){
      ANode *prev = theList->head->next;
      ANode *cur = prev->next;
      theList->head->next = theList->head;
      theList->head->prev = theList->head;
      free(prev->command);
      free(prev->alias);
      free(prev);
      free(cur->command);
      free(cur->alias);
      free(cur);
    }

    if(theList->size == 1){
      ANode *prev = theList->head->next;
      theList->head->next = theList->head;
      theList->head->prev = theList->head;
      free(prev->command);
      free(prev->alias);
      free(prev);
    }

    free(theList->head);
    theList->size = 0;
  } else { 
    //only reach here if passed in list is NULL
    perror("List is NULL!\n");

  } //and of if list is not null
}

void printList(AList * theList){

  if(theList == NULL) {
    exit(-99);
  }
if(theList->size >0){
  ANode* cur = theList->head->next;
  while(cur != theList->head){
    printf("alias %s='%s'\n", cur->alias, cur->command);
    cur = cur->next;
  }
}
}

