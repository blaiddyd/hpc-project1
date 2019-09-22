FLAGS = -Wall -Werror -pedantic

matrix: main.o io_ops.o matrix_ops.o header.h
	gcc -fopenmp -o matrix main.o io_ops.o matrix_ops.o -lm

main.o: main.c header.h
	gcc -c main.c

io_ops.o: io_ops.c header.h
	gcc -c io_ops.c

matrix_ops: matrix_ops.c header.h
	gcc -c matrix_ops.c

clean:
	rm -f *.o

clear:
	rm -f *.o
	rm matrix