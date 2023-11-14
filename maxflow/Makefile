CC := clang++
CFLAGS := -pedantic -Wall -g -c -O3
LFLAGS := -pedantic -Wall -g -O3

all:	maxflow_main.out graph_generator.out

maxflow_main.out:	maxflow_main.o maxflow.o
	$(CC) $(LFLAGS) -o $@ $^

graph_generator.out:	graph_generator.o maxflow.o
	$(CC) $(LFLAGS) -o $@ $^

%.o:	%.cpp %.hpp
	$(CC) $(CFLAGS) -o $@ $<

%.o:	%.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm *.o *.out
