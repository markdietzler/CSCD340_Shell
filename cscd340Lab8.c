#include "./pipes/pipes.h"
#include "./utils/myUtils.h"
#include "./tokenize/makeArgs.h"
#include "./comlist/comlist.h"
#include "./aliaslist/aliaslist.h"

int main(){

  /*
  *  Start init
  */

  FILE *p_msshrc;
  FILE *p_msshrc_history;
  int HISTCOUNT = 100;  //default value
  int HISTFILECOUNT = 1000;  //default value
  int commandHistoryIndexCounter = 0;
  char buff[MAX];
  char s[MAX];
  int read_MSSC_Int = 0;

  ComList * commandList = comlist();
  AList * aliasList = alistlinkedlist();

  //check for msshrc, and if found read in its values
  p_msshrc = fopen(".msshrc", "r");
  
  if(p_msshrc != NULL) {
    
    //if file exists
    
    //read first line
    fscanf(p_msshrc, "%s",buff);
    char check_me[MAX];
    char int_value[MAX];
    //int equals_spot = 0;

    msscProc(buff, check_me, int_value);
    read_MSSC_Int = atoi(int_value);
    
    //read in HISTCOUNT or HISTFILECOUNT
    if(check_me[4] == 'C'){
      HISTCOUNT = read_MSSC_Int;
    }
    if(check_me[4] == 'F'){
      HISTFILECOUNT = read_MSSC_Int;
    }

    //read next line
    fscanf(p_msshrc, "%s",buff);

    msscProc(buff, check_me, int_value);
    read_MSSC_Int = atoi(int_value);
    //read in HISTCOUNT or HISTFILECOUNT
    if(check_me[4] == 'C'){
      HISTCOUNT = read_MSSC_Int;
    }

    if(check_me[4] == 'F'){
      HISTFILECOUNT = read_MSSC_Int;
    }

    memset(buff, 0, MAX); //zero out buff so we can use it later
    //scan the rest of the file looking for alias and path entries

    //NOTE: this bit of code is predicated on alias commands following
    //bash standard alias format: alias dir='ls -l'
    //anything else WILL seg fault

    while(fgets(buff, MAX, p_msshrc) != NULL){

      strip(buff); //strip off carriage return
      char check[6];
      strncpy(check, buff, 5);

      if(strcmp(check, "alias") == 0){

        char a_alias[MAX];
        char a_comm[MAX];

        procAlias(buff, a_alias, a_comm);

        pushAlias(aliasList, a_comm, a_alias);

      }

    } //end of while loop
    
    //close the .msshrc file because it existed
    int checkMSSHRC_file = fclose(p_msshrc);
    if(checkMSSHRC_file == 1){
      perror(".msshrc file not closed!\n");
    }

  } //end of msshrc if statement

    memset(buff, 0, MAX); //zero out buff so we can use it later

  //end of msshrc processing  

  //open msshrc_history and read it in if it exists

  p_msshrc_history = fopen(".msshrc_history", "r");

  if(p_msshrc_history != NULL){
    int readIndex = 0;
    char IndexBuff[6];
    char histBuff[MAX];

    //start reading file
    while(fgets(buff, MAX, p_msshrc_history) != NULL){
      strip(buff);
      strncpy(IndexBuff, buff, 5);
      strcpy(histBuff, buff+5);
      readIndex = atoi(IndexBuff);

      

      pushCom(commandList, readIndex, histBuff);
    }

    //sets the floating history file command index to the last read in index from the file
    commandHistoryIndexCounter = readIndex;

    //close history file
    int checkMSSHRC_hist = fclose(p_msshrc_history);
    if(checkMSSHRC_hist == 1) {
      perror(".msshrc_history not closed!\n");
    }

  } //end of p_msshrc_history NULL check
  
  //end of msshrc history file processing

  memset(buff, 0, MAX); //zero out buff so we can use it later  

  /*
  *  end init 
  */


  
  printf("command?: ");
  fgets(s, MAX, stdin);
  strip(s);

  while(strcmp(s, "exit") != 0){

    //handle bang stuff NOTE: NOT SAVED IN COMMAND HISTORY
    if(s[0] == '!'){
      int rewind = 1;
      char retrieved[MAX];
      //start bang handling
      if(isdigit(s[1]) != 0){
        rewind = atoi(s+1);
        bangCom(commandList, retrieved, rewind);
      }
      if(s[1] == '-'){
        rewind = atoi(s+2);
        bangCom(commandList, retrieved, rewind);
      }
      if(s[1] == '!'){
        bangCom(commandList, retrieved, rewind);
      }
      if(rewind <= HISTCOUNT){
        strcpy(s, retrieved);
      }

    } else { //end of bang handling

      //increment history index and push command to the history list
      commandHistoryIndexCounter = commandHistoryIndexCounter + 1;
      pushCom(commandList, commandHistoryIndexCounter, s);

    }

    //execute existing aliases
    char compare_me[MAX] = "ABCD";
    peekCommand(aliasList, s, compare_me);
    if(strcmp(compare_me, "ABCD") == 0){
      //do nothing
    } else {
      strcpy(s, compare_me);
    }
    memset(compare_me, 0, MAX);  //zero out compare_me

    int pipeCount = containsPipe(s);

    //handle cd %some_directory and cd ~
    if(s[0] == 'c' && s[1] == 'd'){
      if(s[3] == '~'){
        char * ptr_path;
        ptr_path = getenv("HOME");
        chdir (ptr_path);
      } else {
        chdir (s+3);
      }
    }

    //handle print history
    if(strcmp(s, "history") == 0){

      printComList(commandList, HISTFILECOUNT);
    }

    //handle print alias and setting a new one
    if(strcmp(s, "alias") == 0){
        printList(aliasList);
    }
    strncpy(buff, s, 6);
    if(strcmp(buff, "alias ") == 0){
    int nd = isalpha(s[6]);
      if(nd != 0){
        char new_alias[MAX];
        char new_command[MAX];
        procAlias(s, new_alias, new_command);
      } else {
        printList(aliasList);
      }
    }
    //unalias
    if(strcmp(buff, "unalia") == 0){
      char workingString[MAX];
      strcpy(workingString, s+8);
      unAlias(aliasList, workingString);
      memset(workingString, 0, MAX);
    }

    memset(buff, 0, MAX); //zero out buff so we can use it later  
    
    if(pipeCount == 2){

      twoPipe(s);

    }

    else if(pipeCount == 1){

      onePipe(s);

    } else {
    
      noPipe(s);

    }

    printf("command?: ");
    fgets(s, MAX, stdin);
    strip(s);
  }// end while

  //start clean up

  dumpComList(commandList, HISTFILECOUNT);

  cleanComList(commandList);
  cleanList(aliasList);
  
  free(commandList);
  free(aliasList);

  return 0;

}// end main




