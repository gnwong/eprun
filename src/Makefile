
all: eprun

eprun:
	gcc main.c dispatch.c -lpthread -o eprun

clean:
	-rm eprun

test: eprun
	./eprun run


