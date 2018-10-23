/*
 * Copyright (c) 2018 George Wong
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "dispatch.h"

int nthreads = 12;

char **get_commands(const char *fname, int *ncommands);
void stripline(char *s);
void error(int n, const char *msg, int kill);

int main (int argc, char **argv) {

  int ncommands = 0;

  char **commands = get_commands(argv[1], &ncommands);
  if (commands == NULL) {
    error(-1, "! unable to read command file", 1);
  }

  int global_nodecount = 0;

  // create pool and start dispatchers
  pthread_t *threadpool = malloc(nthreads * sizeof(*threadpool));
  dispatch_args *threadargs = malloc(nthreads * sizeof(*threadargs));
  for (int i=0; i<nthreads; ++i) {
    threadargs[i].pid = i;
    threadargs[i].ncommands = ncommands;
    threadargs[i].commands = commands;
    threadargs[i].global_nodecount = &(global_nodecount);
    int ret = pthread_create(&threadpool[i], NULL, dispatch_func, &(threadargs[i]));
    if (ret != 0) error(ret, "create thread", 1);
  }

  // join threads after work is done
  for (int i=0; i<nthreads; ++i) {
    int ret = pthread_join(threadpool[i], NULL);
    if (ret != 0) error(ret, "join thread", 0);
  }

  // housekeeping
  if (commands != NULL) {
    for (int i=0; i<ncommands; ++i) {
      if (commands[i] != NULL) {
        free(commands[i]);
      }
    }
    free(commands);
  }

  free(threadargs);
  free(threadpool);

  return 0;
}

char **get_commands(const char *fname, int *ncommands) {

  FILE *fp = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t read = 0;

  fp = fopen(fname, "r");
  if ( fp == NULL ) return NULL;

  // get number of non-empty lines in file
  int nlines = 0;
  while ( (read = getline(&line, &len, fp)) != -1 ) {
    stripline(line);
    if (strlen(line) > 0) nlines++;
  }

  int count = 0;
  char **commands = malloc(nlines * sizeof(*commands));
  if (commands == NULL) return NULL;

  rewind(fp);
  while ( (read = getline(&line, &len, fp)) != -1 ) {
    stripline(line);
    if (strlen(line) > 0) {
      commands[count] = malloc(strlen(line)+1);
      if (commands[count] == NULL) return NULL;
      strncpy(commands[count], line, strlen(line));
      count++;
    }
  }

  *ncommands = nlines;

  free(line);

  fclose(fp);

  return commands;
}

void stripline(char *s) {
  int pos = 0, starti = -1, endi = -1;
  for (int i=0; i<strlen(s); ++i) {
    if (s[i] == '\t') s[i] = ' ';
    if (s[i] == '\n') s[i] = ' ';
    if (starti < 0 && s[i] != ' ') starti = i;
    if (s[i] != ' ') endi = i;
  }
  for (int i=0; i<strlen(s); ++i) {
    if (i<starti || i > endi) continue;
    if (i > 0 && s[i] == ' ' && s[i-1] == ' ') continue;
    s[pos++] = s[i];
  }
  s[pos] = '\0';
}

void error(int n, const char *msg, int kill) {
  fprintf(stderr, "error (%d): %s\n", n, msg);
  if (kill) exit(n);
}


