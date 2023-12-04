CC := clang++
CFLAGS := -pedantic -Wall -g -c -O3
LFLAGS := -pedantic -Wall -g -O3

all:	rbt

run : rbt
	./rbt

rbt:	red-black-tree.cpp
	$(CC) $(LFLAGS) -o $@ $^


clean:
	rm *.o *.out
