#include "maxflow.hpp"

// Recursive internal
vector<edge> get_path(graph &residual, graph &capacities, const int &s, const int &t, set<int> &used);

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
        out.nodes.back().nodes_having_backwards_edges.clear();
    }

    for (int i = 0; i < num_links; i++)
    {
        strm >> from >> to >> weight;
        out.nodes[from].edges[to] = weight;
        out.nodes[to].nodes_having_backwards_edges.insert(from);
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

    strm << to_save.nodes.size() << ' ' << num_links << '\n';

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

// Return the net flow across a path
// Takes time proportional to the length of the path
int path_flow(const vector<edge> &path)
{
    if (path.size() == 0)
    {
        return 0;
    }

    int min = path[0].weight;

    for (int i = 1; i < path.size(); i++)
    {
        if (path[i].weight < min && path[i].weight > 0)
        {
            min = path[i].weight;
        }
    }

    return min;
}

// Returns a valid path from the source to the sink
vector<edge> get_path(graph &residual, graph &capacities, const int &s, const int &t)
{
    set<int> used = {s};
    return get_path(residual, capacities, s, t, used);
}

// Recursive internal
vector<edge> get_path(graph &residual, graph &capacities, const int &s, const int &t, set<int> &used)
{
    // Iterate over options at this point
    // cout << "On node " << s << " looking for " << t << '\n';

    used.insert(s);

    // Forward edges
    for (auto &item : residual.nodes[s].edges)
    {
        // cout << item.first << ' ' << item.second << '\n';

        if (item.second == 0)
        {
            continue;
        }

        else if (item.first == t)
        {
            // Begin complete path
            // Base case for success
            vector<edge> out = {edge{item.first, item.second}};
            return out;
        }

        else if (used.count(item.first) != 0)
        {
            continue;
        }

        else
        {
            // Non-complete path
            // cout << "Recursive call from " << s << " on link " << item.first << ' ' << item.second << '\n';

            vector<edge> result = get_path(residual, capacities, item.first, t, used);

            if (result.size() > 0)
            {
                // Got complete path; Return now
                result.insert(result.begin(), edge{item.first, item.second});
                return result;
            }

            // Failure case,
            // Continue iterating
        }
    }

    // Backwards edges
    for (auto &from : residual.nodes[s].nodes_having_backwards_edges)
    {
        int weight = residual.nodes[from].edges[s];

        // If empty, can't do it
        if (weight >= capacities.nodes[from].edges[s])
        {
            continue;
        }

        else if (used.count(from) != 0)
        {
            continue;
        }

        else
        {
            // Non-complete path
            vector<edge> result = get_path(residual, capacities, from, t, used);

            if (result.size() > 0)
            {
                // Got complete path; Return now
                result.insert(result.begin(), edge{from, 0});
                return result;
            }

            // Failure case,
            // Continue iterating
        }
    }

    // If found no valid paths, return empty
    // Base case for failure
    return vector<edge>();
}

// Get the shorted valid augmenting path using breadth first search
// of the residual graph. Not recursive.
vector<edge> get_path_bfs(graph &residual, graph &capacities, const int &s, const int &t)
{
    // source_index[i] is the node which led to i
    // If a node is not yet visited by bfs, it will have nothing
    map<int, int> source_index;
    queue<int> to_search;

    to_search.push(s);

    while (true)
    {
        if (to_search.empty())
        {
            // Failure case; Halt algorithm
            return vector<edge>{};
        }
        else if (to_search.front() == t)
        {
            // Success case; Break from loop
            break;
        }
        else
        {
            // Regular case; Iterate over links
            int cur = to_search.front();
            to_search.pop();

            // Forwards edges
            for (const auto &p : residual.nodes[cur].edges)
            {
                if (source_index.count(p.first) == 0 && residual.nodes[cur].edges[p.first] > 0)
                {
                    source_index[p.first] = cur;
                    to_search.push(p.first);
                }
            }

            // Backwards edges
            for (const auto &from : residual.nodes[cur].nodes_having_backwards_edges)
            {
                if (source_index.count(from) == 0 &&
                    residual.nodes[from].edges[cur] < capacities.nodes[from].edges[cur])
                {
                    source_index[from] = cur;
                    to_search.push(from);
                }
            }
        }
    }

    // Success case here
    // Reconstruct shortest path into vector
    vector<edge> out;
    int position = t, from;
    while (position != s)
    {
        from = source_index[position];

        // Forwards edge
        if (residual.nodes[from].edges.count(position) != 0)
        {
            out.insert(out.begin(), edge{position, residual.nodes[from].edges[position]});
        }

        // Backwards edge
        else
        {
            out.insert(out.begin(), edge{position, 0});
        }

        position = from;
    }

    return out;
}

// Returns a zero graph in the shape of the one passed
// Takes time proportional to the number of edges in the input
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
// Takes time proportional to the size of the path
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
        // Forwards edge
        if (flow.nodes[position].edges.count(p.to) != 0)
        {
            flow.nodes[position].edges[p.to] += net_flow;
        }

        // Backwards edge
        else
        {
            flow.nodes[p.to].edges[position] -= net_flow;
        }

        position = p.to;
    }
}

// Like add_augmenting_path, but for a residual graph
// Runs in time proportional to the length of the path
void subtract_augmenting_path(const vector<edge> &path, graph &residuals, const int &s)
{
    if (path.size() == 0)
    {
        return;
    }

    int net_flow = path_flow(path);
    int position = s;

    for (const auto &p : path)
    {
        // Forwards edge
        if (residuals.nodes[position].edges.count(p.to))
        {
            residuals.nodes[position].edges[p.to] -= net_flow;
        }

        // Backwards edge
        else
        {
            residuals.nodes[p.to].edges[position] += net_flow;
        }

        position = p.to;
    }
}

////////////////////////////////////////////////////////////////
// Actual routines
////////////////////////////////////////////////////////////////

// Returns the maxflow of a given graph
// using the Ford-Fulkerson algorithm
// n := number NODES, e := number EDGES, f := max flow,
// p := number augmenting paths
// O(e + 3*p*len(path) + e) = O(2e + 3pl) ~ O(pe)
int ford_fulkerson(graph &capacities, const int &s, const int &t, int &iterations)
{
    vector<edge> path;
    graph flow, residual;
    int out = 0;

    iterations = 0;
    flow = zero_graph(capacities); // O(e)
    residual = capacities;

    // While a augmenting path exists
    // int total_path_length = 0;
    do // do p times
    {
        // Get augmenting path
        path = get_path(residual, capacities, s, t); // O(len(path)), worst case e
        out += path_flow(path);

        // Add augmenting path to flow
        add_augmenting_path(path, flow, s); // O(len(path))

        // Recompute residual
        subtract_augmenting_path(path, residual, s); // O(len(path))
        iterations++;

    } while (path.size() > 0);

    return out;
}

// Returns the maxflow of a given graph
// using the Edmonds Karp algorithm
int edmonds_karp(graph &capacities, const int &s, const int &t, int &iterations)
{
    vector<edge> path;
    graph flow, residual;
    int out = 0;

    iterations = 0;
    flow = zero_graph(capacities); // O(e)
    residual = capacities;

    // While a augmenting path exists
    // int total_path_length = 0;
    do // do p times
    {
        // Get augmenting path via breadth first search
        path = get_path_bfs(residual, capacities, s, t); // O(len(path))
        out += path_flow(path);

        // Add augmenting path to flow
        add_augmenting_path(path, flow, s); // O(len(path))

        // Recompute residual
        subtract_augmenting_path(path, residual, s); // O(len(path))
        iterations++;

    } while (path.size() > 0);

    return out;
}

////////////////////////////////////////////////////////////////
