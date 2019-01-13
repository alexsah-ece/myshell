cc = gcc
objects = myshell.o parse.o execution.o

myshell: $(objects)
	$(cc) -o myshell $(objects)

parse.o: parse.h
execution.o: execution.h

clean:
	rm -f *.o 