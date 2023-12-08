/*
Generates random large graphs
Not necessarily acyclic
*/

#include "maxflow.hpp"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

const static int W_MIN = 1;
const static int W_MAX = 1000;

int main(int argc, char *argv[])
{
    graph g;
    unsigned long long n = 0, e = 0;
    string file;

    if (argc != 4)
    {
        cout << "File: ";
        cin >> file;
        cout << "Number of nodes: ";
        cin >> n;
        cout << "Number of edges: ";
        cin >> e;
    }
    else
    {
        file = argv[1];
        n = atoi(argv[2]);
        e = atoi(argv[3]);
    }

    if (n < 0 || e < 0)
    {
        cout << "Invalid combination n = " << n << ", e = " << e << "\n";
        return 1;
    }

    srand(time(NULL));

    // Create nodes
    for (int i = 0; i < n; i++)
    {
        g.nodes.push_back(graph_node{});
    }

    // Create edges
    for (int i = 0; i < e; i++)
    {
        int from, to, weight;
        from = rand() % n;

        do
        {
            to = rand() % n;
        } while (from == to);

        weight = W_MIN + (rand() % (W_MAX - W_MIN));

        g.nodes[from].edges[to] = weight;
    }

    // Save to file
    ofstream f(file);
    if (!f.is_open())
    {
        cout << "Failed to open '" << file << "'\n";
        return 2;
    }
    save_graph(g, f);
    f.close();

    return 0;
}
