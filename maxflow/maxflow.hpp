/**
 * @file maxflow.hpp
 *
 * @brief Several maxflow algorithms for the final project in
 *        CSCI 480 at CMU under Prof. Sherine Antoun.
 *
 * Jordan Dehmel, 2023
 * jdehmel@outlook.com
 * jedehmel@mavs.coloradomesa.edu
 *
 * Resources:
 * https://www.geeksforgeeks.org/ford-fulkerson-algorithm-for-maximum-flow-problem/
 * https://brilliant.org/wiki/ford-fulkerson-algorithm/
 * https://brilliant.org/wiki/edmonds-karp-algorithm/
 */

#ifndef MAXFLOW_HPP
#define MAXFLOW_HPP

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;

/**
 * @struct edge
 * @brief A struct representing a weighted edge in a graph
 *
 * @var edge::to
 * The index of the node that the edge points to
 *
 * @var edge::weight
 * The weight of the edge
 */
struct edge
{
    int to;
    int weight;
};

/**
 * @struct graph_node
 * @brief A struct representing a node in a weighted graph
 *
 * @var graph_node::edges
 * A map of the edges contained within. The key is the index of
 * the linked node, and the value is the weight of that edge.
 * @var graph_node::nodes_having_backwards_edges
 * A set of all nodes which contain links to this node.
 */
struct graph_node
{
    map<int, int> edges;
    set<int> nodes_having_backwards_edges;
};

// A graph of graph_nodes
/**
 * @struct graph
 * @brief A struct representing a weighted graph
 *
 * @var graph::nodes
 * All the nodes in the graph.
 */
struct graph
{
    // vector of all nodes
    vector<graph_node> nodes;
};

/**
 * @brief Debugging output for graph objects
 *
 * @param strm The stream to write debugging output to
 * @param what The graph to output to the stream
 */
ostream &operator<<(ostream &strm, graph &what);

/**
 * @brief Loads a graph from an input stream
 *
 * @param strm The stream to load from
 *
 * @return The loaded graph
 */
graph load_graph(istream &strm);

// Saves (or outputs) a graph to an output stream (cout, cerr,
// or file)
/**
 * @brief Outputs a graph in a way such that it can be loaded
 *        by `load_graph`.
 *
 * @param to_save The graph to save
 * @param strm The stream to output `to_save` to
 */
void save_graph(graph &to_save, ostream &strm);

/**
 * @brief Creates a zero graph in the shape of the passed graph
 *
 * @param capacities The capacity graph to copy
 * @return A graph of all zeros in the shape of the passed graph
 */
graph zero_graph(const graph &capacities);

/**
 * @brief Adds an augmenting path into a flow graph
 *
 * @param path The augmenting path to add
 * @param flow The flow graph to modify
 * @param s The index of the starting node
 */
void add_augmenting_path(const vector<edge> &path, graph &flow, const int &s);

/**
 * @brief Subtracts an augmenting path from a residual graph
 *
 * @param path The augmenting path to subtract
 * @param residuals The residual graph to modify
 * @param s The index of the starting node
 */
void subtract_augmenting_path(const vector<edge> &path, graph &residuals, const int &s);

/**
 * @brief Returns the first valid augmenting path from the
 *        source to the sink. Recursive.
 *
 * @param residual The residual (remaining unused flow) graph
 * @param capacities The capacity graph
 * @param s The index of the starting node
 * @param t The index of the ending node
 *
 * @return A vector of edges representing a valid augmenting
 *         path
 */
vector<edge> get_path(graph &residual, graph &capacities, const int &s, const int &t);

/**
 * @brief Returns the shorted valid augmenting path from the
 *        source to the sink, using breadth first search.
 *
 * @param residual The residual (remaining unused flow) graph
 * @param capacities The capacity graph
 * @param s The index of the starting node
 * @param t The index of the ending node
 *
 * @return A vector of edges representing the shortest valid
 *         augmenting path
 */
vector<edge> get_path_bfs(const graph &residual, graph &capacities, const int &s, const int &t);

/**
 * @brief Gets the net flow along an augmenting path
 *
 * @param path The path in question
 *
 * @return The minimal (net) flow across the path
 */
int path_flow(const vector<edge> &path);

/**
 * @brief Returns the maxflow of a given graph using the
 *        Ford-Fulkerson algorithm
 *
 * @param on The graph to operate on
 * @param s The index of the starting node
 * @param t The index of the ending node
 * @param iterations Replaced by the number of iterations
 *
 * @return The max flow across the graph from `s` to `t`
 */
int ford_fulkerson(graph &on, const int &s, const int &t, int &iterations);

/**
 * @brief Returns the maxflow of a given graph the Edmonds Karp
 *        algorithm
 *
 * @param on The graph to operate on
 * @param s The index of the starting node
 * @param t The index of the ending node
 * @param iterations Replaced by the number of iterations
 *
 * @return The max flow across the graph from `s` to `t`
 */
int edmonds_karp(graph &on, const int &s, const int &t, int &iterations);

#endif
