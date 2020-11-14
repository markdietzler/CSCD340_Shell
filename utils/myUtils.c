#include "myUtils.h"

void strip(char *array)
{
	if(array == NULL)
	{
		perror("array is null");
		exit(-99);
	}// end if

	int len = strlen(array), x = 0;
   
	while(array[x] != '\0' && x < len)
	{
	  if(array[x] == '\r')
		 array[x] = '\0';

	  else if(array[x] == '\n')
		 array[x] = '\0';

	  x++;

}// end while
   
}// end strip

int menu()
{
   char temp[MAX];
   int choice;
   printf("1) Print the list\n");
   printf("2) Add First\n");
   printf("3) Add Last\n");
   printf("4) Sort\n");
   printf("5) Remove Item\n"); 
   printf("6) Quit\n"); 
   printf("Choice --> ");
   fgets(temp, MAX, stdin);
   choice = atoi(temp);

   
   while(choice < 1 || choice > 6)
   {
      printf("1) Print the list\n");
      printf("2) Add First\n");
      printf("3) Add Last\n");
      printf("4) Sort\n");
      printf("5) Remove Item\n"); 
      printf("6) Quit\n"); 
      printf("Choice --> ");
      fgets(temp, MAX, stdin);
      choice = atoi(temp);
   
   }// end while

   return choice;
   
}// end menu

void procAlias(const char *incoming, char *returnedAlias, char *returnedCommand){
  if(incoming == NULL){
    perror("String to check was NULL!\n");
    exit(-99);
  }
  if(returnedAlias == NULL){
    perror("Alias string to return is NULL!\n");
    exit(-99);
  }
  if(returnedCommand == NULL){
    perror("Command string to return is NULL!\n");
    exit(-99);
  }
  
  int checkEqualsSign = 0;
  int checkSingleQuote = 0;
  char blah = 39;

  for(int a = 0; a < strlen(incoming); a++){
    if(incoming[a] == '='){
      checkEqualsSign++;
    }
    if(incoming[a] == blah){
      checkSingleQuote++;
    }
  } //end of validation loop

  if(checkEqualsSign < 1 || checkSingleQuote < 2){
    printf("Malformed alias command!\n");
  } else {
    char proc_temp[MAX];
    char copyAlias[MAX];
    char * token;
    char * save;
    strcpy(copyAlias, incoming);
    //printf("%s\n", copyAlias);
    token = strtok(copyAlias, "'");
    strcpy(proc_temp, token);
    token = strtok(NULL, "'");
    strcpy(returnedCommand, token);
  
    token = strtok_r(proc_temp, " ", &save);
    token = strtok_r(NULL, " ", &save);
    int stripIt = strlen(token) -1;
    token[stripIt] = '\0';
    strcpy(returnedAlias, token);
    memset(proc_temp, 0, MAX); //zero out string
    memset(copyAlias, 0, MAX); //zero out string
  }

}

void msscProc(const char *incomingString, char *returnedSearch, char *returnedInt){
  int equals_spot = 0;
  for(int z = 0; z < strlen(incomingString); z++){
    if(incomingString[z] == '='){
      equals_spot = z;
    }
  }
  memset(returnedSearch, 0, MAX); //zero out buff so we can use it later
  strcpy(returnedInt, incomingString + (equals_spot +1));
  strncpy(returnedSearch, incomingString, (equals_spot));
}
