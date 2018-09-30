# example spinner. 
# completely self-contained simple collatz program

import sys

def next(n):
  return 3*n+1 if n%2 else n/2

if len(sys.argv) > 1: N = int(sys.argv[1])
else: N = 10000

for i in range(1,N):
  n = i
  while n > 1: n = next(n)

