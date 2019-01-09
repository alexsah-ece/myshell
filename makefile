cc = gcc
objects = myshell.o input.o execution.o

myshell: $(objects)
	$(cc) -o myshell $(objects)

input.o: input.h
execution.o: execution.h

clean:
	rm -f *.o 