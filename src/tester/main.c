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
#include <sys/utsname.h>
#include <omp.h>

size_t collatz_next (size_t n) {
  if (n % 2) return 3*n + 1;
  return n / 2;
}

size_t collatz (size_t n) {
  while (n != 1) {
    n = collatz_next(n);
  }
}

int main (int argc, char **argv) {

  // run as
  // $ ./tester [target_runtime_in_seconds]

  double target_runtime_seconds = 10;

  if (argc > 1) {
    target_runtime_seconds = atoi(argv[1]);
  }

  double quit_time = omp_get_wtime() + target_runtime_seconds;

  struct utsname buffer;
  if (uname(&buffer)) {
    fprintf(stderr, "! unable to get system info\n");
    exit(-1);
  }

  fprintf(stderr, "system name = %s\nnode name   = %s\nrelease     = %s\nversion     = %s\nmachine     = %s\n", 
      buffer.sysname, buffer.nodename, buffer.release, buffer.version, buffer.machine);

  int n_openmp = 1;
#pragma omp parallel
  if (omp_get_thread_num() == 0) {
    n_openmp = omp_get_num_threads();
  }

  size_t n_to_run = 100000;

  while (omp_get_wtime() < quit_time) {
    double t_initial = omp_get_wtime();
#pragma omp parallel for schedule(dynamic,2)
    for (size_t n=1; n<n_to_run; ++n) {
      collatz(n);
    }
    double t_final = omp_get_wtime();
    double t_elapsed = t_final - t_initial;
    fprintf(stderr, "ran %ld (with nproc=%d) in %g seconds on %s\n", n_to_run, n_openmp, t_elapsed, buffer.nodename);
    if (t_elapsed < target_runtime_seconds/10) n_to_run *= 10;
  }
   
}

