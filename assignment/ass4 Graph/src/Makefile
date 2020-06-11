CFLAGS = -Wall -Werror -O

test_ass4 : test_ass4.o MyList.o MyHeap.o MyGraph.o
	gcc -o test_ass4 test_ass4.o MyList.o MyHeap.o MyGraph.o

test_ass4.o : test_ass4.c MyHeap.h MyGraph.h
	gcc $(CFLAGS) -c test_ass4.c

MyList.o : MyList.c MyList.h
	gcc $(CFLAGS) -c MyList.c

MyHeap.o : MyHeap.c MyHeap.h
	gcc $(CFLAGS) -c MyHeap.c

MyGraph.o : MyGraph.c MyGraph.h
	gcc $(CFLAGS) -c MyGraph.c

clean :
	rm test_ass4 test_ass4.o MyGraph.o MyList.o MyHeap.o