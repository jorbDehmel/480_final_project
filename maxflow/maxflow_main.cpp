/*
Max-flow implementation for final assignment in CSCI 480
Under Prof. Sherine Antoun, CMU
Jordan Dehmel, 2023
jdehmel@outlook.com
jedehmel@mavs.coloradomesa.edu
*/

#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include "maxflow.hpp"
using namespace std;

int main(int argc, char *argv[])
{
    string filepath;
    ifstream file;
    graph g;
    int s, t;
    chrono::_V2::system_clock::time_point start, end;
    unsigned long long FF_elapsed_ns, EK_elapsed_ns;
    int FF_result, EK_result;
    double percentage_faster;

    // Title
    cout << "Maxflow using Ford-Fulkerson and Edmonds-Karp\n"
         << "---------------------------------------------\n\n";

    // Get file to load from
    if (argc == 2)
    {
        filepath = argv[1];
    }
    else
    {
        cout << "Enter filepath: ";
        cin >> filepath;
    }

    file.open(filepath);
    if (!file.is_open())
    {
        cerr << "Error: Failed to open file '" << filepath << "'\n";

        return 1;
    }

    // Load graph
    g = load_graph(file);

    cout << "'" << filepath << "' contains " << g.nodes.size() << " nodes.\n\n";

    // Close input file
    file.close();

    // Get s and t
    cout << "Source (s) node index: ";
    cin >> s;

    if (s < 0 || s >= g.nodes.size())
    {
        cerr << "Error: Invalid source node " << s << "\n";

        return 2;
    }

    cout << "Sink (t) node index:   ";
    cin >> t;

    if (t < 0 || t >= g.nodes.size() || t == s)
    {
        cerr << "Error: Invalid sink node " << t << "\n";

        return 3;
    }

    cout << '\n';

    // Perform FF and time
    start = chrono::high_resolution_clock::now();
    FF_result = ford_fulkerson(g, s, t);
    end = chrono::high_resolution_clock::now();
    FF_elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    // Perform EK and time
    start = chrono::high_resolution_clock::now();
    EK_result = edmonds_karp(g, s, t);
    end = chrono::high_resolution_clock::now();
    EK_elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    // Output results
    cout << "FF result: " << FF_result << '\n'
         << "FF ns:     " << FF_elapsed_ns << '\n'
         << "FF ms:     " << (FF_elapsed_ns) / (double)(1'000'000) << "\n\n"
         << "EK result: " << EK_result << '\n'
         << "EK ns:     " << EK_elapsed_ns << '\n'
         << "EK ms:     " << (EK_elapsed_ns) / (double)(1'000'000) << "\n\n";

    // Show speed comparison
    percentage_faster = (((FF_elapsed_ns) / (double)(EK_elapsed_ns)) - 1.0) * 100.0;
    cout << "EK is " << percentage_faster << "% faster than FF.\n\n";

    // Error checking for result match
    if (FF_result != EK_result)
    {
        cerr << "Error: FF result does not match EK result!\n\n";

        return 4;
    }

    // Exit
    return 0;
}
