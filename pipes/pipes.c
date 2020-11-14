#include "pipes.h"
#include "../tokenize/makeArgs.h"

int containsPipe(char *s){
    
  int count = 0;

  for(int m = 0; s[m]; m++){
    if(s[m] == '|')
      count++;
  }

  return count;
}

char ** parsePrePipe(char *s, int * preCount){
  char **argv = NULL;
  char *save = NULL;
  char copy[MAX];
  strcpy(copy, s);
  char *token = strtok_r(copy, "|", &save);
  int total = makeargs(token, &argv);
  *preCount = total;
  return argv;
}

char ** parseMidPipe(char *s, int * midCount){
  char **argv = NULL;
  char *midSave = NULL;
  char midCopy[MAX];
  strcpy(midCopy, s);

  char *token = strtok_r(midCopy, "|", &midSave);
  token = strtok_r(NULL, "|", &midSave);
  token = strtok_r(NULL, "|", &midSave);
  int total = makeargs(token, &argv);

  *midCount = total;
  return argv;
}

char ** parsePostPipe(char *s, int * postCount){
  char **argv = NULL;
  char *save = NULL;
  char *token = strtok_r(s, "|", &save);
  token = strtok_r(NULL, "|", &save);
  int total = makeargs(token, &argv);
  *postCount = total;
  return argv;
}

void twoPipe(char *incomingCommand){
  int preCount = 0, midCount = 0, postCount = 0;
  char ** prePipe = NULL, ** midPipe = NULL, ** postPipe = NULL;
  prePipe = parsePrePipe(incomingCommand, &preCount);
  midPipe = parseMidPipe(incomingCommand, &midCount);
  postPipe = parsePostPipe(incomingCommand, &postCount);

  int status;
  pid_t pidMaster = fork();

  if(pidMaster != 0){ //shell process is here
    waitpid(pidMaster, &status, 0);
  } else {
    
    //start set up for child processes
    int firstPipe[2];
    int pipeResultPipe1;

    pipeResultPipe1 = pipe(firstPipe);
    if(pipeResultPipe1 < 0){
      perror("Pipe 1 failure!\n");
      exit(pipeResultPipe1);
    }


    //we now have 4 pipe ends
    // firstPipe[0] = read end of first pipe (read by middle command)
    // firstPipe[1] = write end of first pipe (written by first command)

    //start command forking
    pid_t pid1 = fork(); //fork from shell process

    if(pid1 == 0){ //grandchild

      dup2(firstPipe[1], 1);

      close(firstPipe[0]);
      close(firstPipe[1]);

      int exitGrandChild = execvp(prePipe[0], prePipe);
      exit(exitGrandChild);

    } else {

      int secondPipe[2];
      int pipeResultPipe2;
      pipeResultPipe2 = pipe(secondPipe);
      if(pipeResultPipe2 < 0){
        perror("Pipe 2 failure!\n");
        exit(pipeResultPipe2);
      }
      // secondPipe[0] = read end of second pipe (read by third command)
      // secondPipe[1] = write end of second pipe (written by middle command)

      pid_t pid2 = fork(); //fork child/parent

      if(pid2 == 0){ //child

        dup2(firstPipe[0], 0); //replace middle command stdin with read end of 1st pipe
        dup2(secondPipe[1], 1); //replace middle command stdout with write end of 2nd pipe

        close(firstPipe[0]);
        close(firstPipe[1]);
        close(secondPipe[0]);
        close(secondPipe[1]);

        int exitChild = execvp(midPipe[0], midPipe);
        exit(exitChild);

      } else { //parent

        dup2(secondPipe[0], 0);

        close(firstPipe[0]);
        close(firstPipe[1]);
        close(secondPipe[0]);
        close(secondPipe[1]);

        int exitParent = execvp(postPipe[0], postPipe);
        exit(exitParent);

      }
    }

  }//end of top level shell if/else

  clean(preCount, prePipe);
  clean(midCount, midPipe);
  clean(postCount, postPipe);

} //end of two pipe

void onePipe(char *incomingCommand){
  int preCount = 0, postCount = 0;
  char ** prePipe = NULL, ** postPipe = NULL;
  prePipe = parsePrePipe(incomingCommand, &preCount);
  postPipe = parsePostPipe(incomingCommand, &postCount);
  
  int status;
  pid_t pid = fork();
  if(pid != 0){ // the shell process is here

    waitpid(pid, &status, 0);

  } else {

    int fd[2], res, status;
    res = pipe(fd);

    if(res < 0) {
      printf("Pipe Failure\n");
      exit(-1);
    }// end if
 
    pid_t pid2 = fork();
 
    if(pid2 == 0){
      close(fd[0]);
      close(1);
      dup(fd[1]);
      close(fd[1]);
      int exitChild = execvp(prePipe[0], prePipe);
      exit(exitChild); 

    }// child side (ls -l)

    else {

      close(fd[1]);
      close(0);
      dup(fd[0]);
      close(fd[0]);
      int exitParent = execvp(postPipe[0], postPipe);
      exit(exitParent);

    }// parent side (wc -w)

  }// end top else

  clean(preCount, prePipe);
  clean(postCount, postPipe);

} //end of one pipe

void noPipe(char *incomingCommand){
  int argc;
  char **argv = NULL;
  argc = makeargs(incomingCommand, &argv);
  forkIt(argv);
  clean(argc, argv);
  argv = NULL;
}

void forkIt(char ** argv) {
  int status;
  pid_t pid = fork();
  if(pid != 0){
    waitpid(pid, &status, 0);
  } else {
    int res = execvp(argv[0], argv);
    exit(res);
  }

}

