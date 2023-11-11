CC := clang++
CFLAGS := -pedantic -Wall -g -c
LFLAGS := -pedantic -Wall -g

maxflow_main.out:	maxflow_main.o maxflow.o
	$(CC) $(LFLAGS) -o $@ $^

%.o:	%.cpp %.hpp
	$(CC) $(CFLAGS) -o $@ $<

%.o:	%.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm *.o *.out
