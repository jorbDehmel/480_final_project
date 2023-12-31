/*
Max-flow implementation for final assignment in CSCI 480
Under Prof. Sherine Antoun, CMU
Jordan Dehmel, 2023
jdehmel@outlook.com
jedehmel@mavs.coloradomesa.edu
*/

#include "maxflow.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    string filepath;
    ifstream file;
    graph g;
    int s, t;
    // chrono::_V2::system_clock::time_point start, end;
    unsigned long long FF_elapsed_ns = 0, EK_elapsed_ns = 0;
    int FF_result, EK_result, FF_iterations, EK_iterations;
    double percentage_faster;

    // Get file to load from
    if (argc >= 2)
    {
        filepath = argv[1];
    }
    else
    {
        // Title
        cout << "Maxflow using Ford-Fulkerson and Edmonds-Karp\n"
             << "---------------------------------------------\n\n";

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

    // Close input file
    file.close();

    // Display info
    cout << "'" << filepath << "' contains " << g.nodes.size() << " nodes.\n";

    // Get s and t
    if (argc >= 3)
    {
        s = atoi(argv[2]);
    }
    else
    {
        cout << "Source (s) node index: ";
        cin >> s;
    }

    if (s < 0 || s >= g.nodes.size())
    {
        cerr << "Error: Invalid source node " << s << "\n";

        return 2;
    }

    if (argc >= 4)
    {
        t = atoi(argv[3]);
    }
    else
    {
        cout << "Sink (t) node index:   ";
        cin >> t;
    }

    cout << "\ns=" << s << "\n"
         << "t=" << t << "\n";

    if (t < 0 || t >= g.nodes.size() || t == s)
    {
        cerr << "Error: Invalid sink node " << t << "\n";

        return 3;
    }

    cout << '\n' << flush;

    // Perform FF and time
    {
        auto start = chrono::high_resolution_clock::now();
        FF_result = ford_fulkerson(g, s, t, FF_iterations);
        auto end = chrono::high_resolution_clock::now();
        FF_elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    // Perform EK and time
    {
        auto start = chrono::high_resolution_clock::now();
        EK_result = edmonds_karp(g, s, t, EK_iterations);
        auto end = chrono::high_resolution_clock::now();
        EK_elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    // Output results
    cout << "FF result: " << FF_result << '\n'
         << "FF ns:     " << FF_elapsed_ns << '\n'
         << "FF ms:     " << (FF_elapsed_ns) / (double)(1'000'000) << '\n'
         << "FF passes: " << FF_iterations << '\n'
         << "FF f*/p:   " << FF_result / (double)(FF_iterations) << "\n\n"
         << "EK result: " << EK_result << '\n'
         << "EK ns:     " << EK_elapsed_ns << '\n'
         << "EK ms:     " << (EK_elapsed_ns) / (double)(1'000'000) << '\n'
         << "EK passes: " << EK_iterations << '\n'
         << "EK f*/p:   " << EK_result / (double)(EK_iterations) << "\n\n";

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
