CFLAGS = -Wall -Werror -O

test_ass4 : test_ass4.o MyGraph.o MyBinomialHeaps.o MyList.o
	gcc -o test_ass4 test_ass4.o MyGraph.o MyBinomialHeaps.o MyList.o

test_ass4.o : test_ass4.c MyGraph.h MyBinomialHeaps.h
	gcc $(CFLAGS) -c test_ass4.c

MyGraph.o : MyGraph.c MyGraph.h
	gcc $(CFLAGS) -c MyGraph.c

MyBinomialHeaps.o : MyBinomialHeaps.c MyBinomialHeaps.h
	gcc $(CFLAGS) -c MyBinomialHeaps.c

MyList.o : MyList.c MyList.h
	gcc $(CFLAGS) -c MyList.c

clean :
	rm test_ass4 test_ass4.o MyGraph.o MyBinomialHeaps.o MyList.o