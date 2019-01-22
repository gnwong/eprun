#ifndef DISPATCH_H
#define DISPATCH_H

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct {
  int pid;
  char **commands;
} dispatch_args;

void *dispatch_func(void *ptr);

#endif // DISPATCH_H
