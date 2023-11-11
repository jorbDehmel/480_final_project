#include "maxflow.hpp"

////////////////////////////////////////////////////////////////
// IO operations
////////////////////////////////////////////////////////////////

// For debugging
ostream &operator<<(ostream &strm, graph &what)
{
    int i = 0;
    strm << "Graph\n{\n";
    for (const auto &node : what.nodes)
    {
        for (const auto &edge : node.edges)
        {
            strm << '\t' << i << " -(" << edge.second << ")-> " << edge.first << '\n';
        }
        i++;
    }
    strm << "}\n";

    return strm;
}

// Loads a graph from an input stream (cin or file)
graph load_graph(istream &strm)
{
    /*
    num_nodes
    num_links
    from to weight
    ...
    */

    unsigned long num_nodes, num_links;
    int from, to, weight;
    graph out;

    out.nodes.clear();

    strm >> num_nodes >> num_links;

    out.nodes.reserve(num_nodes);
    for (int i = 0; i < num_nodes; i++)
    {
        out.nodes.push_back(graph_node{});
        out.nodes.back().edges.clear();
    }

    for (int i = 0; i < num_links; i++)
    {
        strm >> from >> to >> weight;
        out.nodes[from].edges[to] = weight;
    }

    return out;
}

// Saves or outputs a graph to an output stream
void save_graph(graph &to_save, ostream &strm)
{
    /*
    num_nodes
    num_links
    from to weight
    ...
    */

    size_t num_links = 0;
    for (auto item : to_save.nodes)
    {
        num_links += item.edges.size();
    }

    strm << to_save.nodes.size() << '\n'
         << num_links;

    int i = 0;
    for (auto node : to_save.nodes)
    {
        for (auto edge : node.edges)
        {
            strm << i << ' ' << edge.first << ' ' << edge.second << '\n';
        }

        i++;
    }

    return;
}

////////////////////////////////////////////////////////////////
// Utilities
////////////////////////////////////////////////////////////////

// Returns the residual graph given some capacity and flow
// graphs
graph residual_graph(const graph &capacity, graph &flow)
{
    graph out = capacity;

    for (int node = 0; node < capacity.nodes.size(); node++)
    {
        for (auto edge_item : flow.nodes[node].edges)
        {
            out.nodes[node].edges[edge_item.first] -= edge_item.second;

            // If residual is 0, erase this edge
            // if (out.nodes[node].edges[edge] == 0)
            // {
            //      out.nodes[node].edges.erase(edge);
            // }

            // Error checking
            if (out.nodes[node].edges[edge_item.first] < 0)
            {
                throw runtime_error("Negative residual cannot exist!");
            }
        }
    }

    return out;
}

// Return the net flow across a path
int path_flow(const vector<edge> &path)
{
    int min = path[0].weight;

    for (int i = 1; i < path.size(); i++)
    {
        if (path[i].weight < min)
        {
            min = path[i].weight;
        }
    }

    return min;
}

// Returns a valid path from the source to the sink
vector<edge> get_path(const graph &residual, const int &s, const int &t)
{
    int position = s;

    // The set of all transitions already taken
    set<int> taken = {s};

    // The path taken
    vector<edge> path;

    // Iterate until reaches end (or no options)
    while (position != t)
    {
        // Choose next edge
        bool has_taken_path = false;
        for (auto edge_item : residual.nodes[position].edges)
        {
            // Is an option and is not already in path; Take this edge
            if (edge_item.second > 0 && taken.count(edge_item.first) == 0)
            {
                taken.insert(edge_item.first);
                path.push_back(edge{edge_item.first, edge_item.second});
                position = edge_item.first;
                has_taken_path = true;
                break;
            }
        }

        // Termination case for larger program
        if (!has_taken_path)
        {
            return vector<edge>{};
        }
    }

    return path;
}

// Returns a zero graph in the shape of the one passed
graph zero_graph(const graph &capacities)
{
    graph out = capacities;

    for (int node = 0; node < capacities.nodes.size(); node++)
    {
        for (auto edge_item : capacities.nodes[node].edges)
        {
            out.nodes[node].edges[edge_item.first] = 0;
        }
    }

    return out;
}

// Adds an augmenting path into a flow graph
void add_augmenting_path(const vector<edge> &path, graph &flow, const int &s)
{
    if (path.size() == 0)
    {
        return;
    }

    int net_flow = path_flow(path);
    int position = s;

    for (const auto &p : path)
    {
        flow.nodes[position].edges[p.to] += net_flow;
        position = p.to;
    }
}

////////////////////////////////////////////////////////////////
// Actual routines
////////////////////////////////////////////////////////////////

// Returns the maxflow of a given graph
// using the Ford-Fulkerson algorithm
int ford_fulkerson(graph &capacities, const int &s, const int &t)
{
    vector<edge> path;

    graph flow, residual;
    flow = zero_graph(capacities);
    residual = capacities;

    // While a augmenting path exists
    do
    {
        /// Get augmenting path
        path = get_path(residual, s, t);

        // Add augmenting path to flow
        add_augmenting_path(path, flow, s);

        // Recompute residual
        residual = residual_graph(capacities, flow);

    } while (path.size() > 0);

    // Get total flow out of source node
    // (equal to total flow into destination node,
    // as well as total maximum network flow)
    int out = 0;
    for (const auto &p : flow.nodes[0].edges)
    {
        out += p.second;
    }

    return out;
}

// Returns the maxflow of a given graph
// using the Edmonds Karp algorithm
int edmonds_karp(graph &on, const int &s, const int &t)
{
    return 0;
}

////////////////////////////////////////////////////////////////
