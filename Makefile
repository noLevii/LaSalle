#
# Makefile
# Practica4
#
#

CC = gcc
CPPFLAGS = -Wall -fopenmp

Practica4: Main.o ParallelMatrix.o
	$(CC) $(CPPFLAGS) -o $@ $^

%o:%.c
	$(CC) $(CPPFLAGS) -c $<

clean:
	rm *.o
	rm Practica4
