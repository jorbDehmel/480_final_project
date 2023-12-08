CC := clang++
CFLAGS := -pedantic -Wall -g -c -O3
LFLAGS := -pedantic -Wall -g -O3

all : rbt.out

run : rbt.out
	./rbt.out
	rm rbt.out

rbt.out:	red-black-tree.cpp
	$(CC) $(LFLAGS) -o $@ $^


clean:
	rm *.o *.out
