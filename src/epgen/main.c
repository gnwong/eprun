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
#include <string.h>

int main (int argc, char **argv) {

 	// epgen python myscript.py -g glob/arguments 

	int globidx = -1;
	for (int i=0; i<argc; ++i) {
		if ( strncmp("-g", argv[i], 2) == 0 ) {
			globidx = i;
			break;
		}
	}

	FILE *fp = fopen("run", "w");

	for (int i=globidx+1; i<argc; ++i) {
		for (int j=1; j<globidx; ++j) {
			fprintf(fp, "%s ", argv[j]);
		}	
		fprintf(fp, "%s\n", argv[i]);
	}

  fprintf(fp, "\n");

	fclose(fp);

  return 0;
}


