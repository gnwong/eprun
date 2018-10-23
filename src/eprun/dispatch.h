#ifndef DISPATCH_H
#define DISPATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct {
  int *global_nodecount;
  int pid;
  int ncommands;
  char **commands;
} dispatch_args;

void *dispatch_func(void *ptr);

#endif // DISPATCH_H
