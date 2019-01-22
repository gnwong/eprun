
.PHONY: meprun clean

all: meprun

meprun:
	mpicc main.c dispatch.c -lpthread -o meprun

clean:
	-rm meprun

test: meprun
	mpiexec ./meprun run


