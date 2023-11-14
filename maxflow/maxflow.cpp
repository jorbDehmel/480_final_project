#include "maxflow.hpp"

// Recursive internal
vector<edge> get_path(const graph &residual, const int &s, const int &t, set<int> &used);

////////////////////////////////////////////////////////////////
// IO operations
////////////////////////////////////////////////////////////////

// For debugging
ostream &
operator<<(ostream &strm, graph &what)
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

    strm << to_save.nodes.size() << ' '
         << num_links << '\n';

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
    set<int> used = {s};
    return get_path(residual, s, t, used);
}

// Recursive internal
vector<edge> get_path(const graph &residual, const int &s, const int &t, set<int> &used)
{
    // Iterate over options at this point
    // cout << "On node " << s << " looking for " << t << '\n';

    used.insert(s);
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

            vector<edge> result = get_path(residual, item.first, t, used);

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

    // If found no valid paths, return empty
    // Base case for failure
    return vector<edge>();
}

// Get the shorted valid augmenting path using breadth first search
// of the residual graph. Not recursive.
vector<edge> get_path_bfs(graph &residual, const int &s, const int &t)
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
            for (const auto &p : residual.nodes[cur].edges)
            {
                if (source_index.count(p.first) == 0 && residual.nodes[cur].edges[p.first] > 0)
                {
                    source_index[p.first] = cur;
                    to_search.push(p.first);
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
        out.insert(out.begin(), edge{position, residual.nodes[from].edges[position]});
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
        flow.nodes[position].edges[p.to] += net_flow;
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
        residuals.nodes[position].edges[p.to] -= net_flow;
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
int ford_fulkerson(graph &capacities, const int &s, const int &t)
{
    vector<edge> path;
    graph flow, residual;
    int out;

    flow = zero_graph(capacities); // O(e)
    residual = capacities;

    // While a augmenting path exists
    // int total_path_length = 0;
    do // do p times
    {
        // Get augmenting path
        path = get_path(residual, s, t); // O(len(path)), worst case e
        // total_path_length += path.size();

        // Add augmenting path to flow
        add_augmenting_path(path, flow, s); // O(len(path))

        // Recompute residual
        subtract_augmenting_path(path, residual, s); // O(len(path))

    } while (path.size() > 0);

    // Get total flow out of source node
    // (equal to total flow into destination node,
    // as well as total maximum network flow)
    // worst case e
    out = 0;
    for (const auto &p : flow.nodes[s].edges)
    {
        out += p.second;
    }

    // cout << "FF had total path length = " << total_path_length << '\n';

    return out;
}

// Returns the maxflow of a given graph
// using the Edmonds Karp algorithm
int edmonds_karp(graph &capacities, const int &s, const int &t)
{
    vector<edge> path;
    graph flow, residual;
    int out;

    flow = zero_graph(capacities); // O(e)
    residual = capacities;

    // While a augmenting path exists
    // int total_path_length = 0;
    do // do p times
    {
        // Get augmenting path via breadth first search
        path = get_path_bfs(residual, s, t); // O(len(path))
        // total_path_length += path.size();

        // Add augmenting path to flow
        add_augmenting_path(path, flow, s); // O(len(path))

        // Recompute residual
        subtract_augmenting_path(path, residual, s); // O(len(path))

    } while (path.size() > 0);

    // Get total flow out of source node
    // (equal to total flow into destination node,
    // as well as total maximum network flow)
    // worst case e
    out = 0;
    for (const auto &p : flow.nodes[s].edges)
    {
        out += p.second;
    }

    // cout << "EK had total path length = " << total_path_length << '\n';

    return out;
}

////////////////////////////////////////////////////////////////
