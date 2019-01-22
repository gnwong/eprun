
#include "dispatch.h"

void *dispatch_func(void *ptr) {

  int cidx = -1;
  dispatch_args *args = (dispatch_args *)ptr;
  pid_t parent = getpid();

  while (1) {

    // get index of next command to run
    int buf[1];
    MPI_Status status;
    MPI_Send(buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(buf, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    cidx = buf[0];

#ifdef VERBOSE
      fprintf(stderr, "%d (%d) is about to start job %d\n", parent, args->pid, cidx);
#endif // VERBOSE

    if (cidx < 0) break;

    pid_t pid = fork();

    // error
    if (pid < 0) {

      perror("fork new process");
      break;

    } 
    
    // child
    else if (pid == 0) {

      char *command = args->commands[cidx];

      // get number of tokens
      int ntokens = 1;
      for (int i=0; i<strlen(command); ++i) {
        if (command[i] == ' ') ntokens++;
      }

      // set up arguments
      int tokenn = 0;
      char **argv = malloc((ntokens+1) * sizeof(*argv));
      argv[tokenn] = strtok(command, " "); 
      while (argv[tokenn] != NULL) argv[++tokenn] = strtok(NULL, " ");
      argv[++tokenn] = malloc(1);
      if (argv[tokenn] == NULL) exit(-1);
      argv[tokenn] = NULL;
    
      // run and handle issues
      execvp(argv[0], argv);
      perror("execlp fail");
      exit(-1);
    } 
    
    // parent
    else {
      
      int status;
      waitpid(pid, &status, 0);
#ifdef VERBOSE
      fprintf(stderr, "%d/%d (%d) is done with code %d\n", parent, pid, args->pid, status);
#endif // VERBOSE

    }

  }

}

