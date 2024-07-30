#include <bits/stdc++.h>
using namespace std;

const int INF = INT_MAX;
const int BANDS = 50;

struct Edge
{
    int to;
    int weight;
};

struct Path
{
    vector<int> nodes;
    int totalWeight;
};

bool operator<(const Path &a, const Path &b)
{
    return a.totalWeight > b.totalWeight;
}

vector<vector<Edge>> graph;
map<pair<int, int>, vector<int>> spectrum;
map<pair<int, int>, vector<int>> allocatedBands;
map<pair<int, int>, Path> allocatedRoutes;

// Dijkstra's Algorithm to find the shortest path from source to target
Path dijkstra(int source, int target)
{
    vector<int> dist(graph.size(), INF);
    vector<int> prev(graph.size(), -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        if (u == target)
            break;

        for (const Edge &edge : graph[u])
        {
            int v = edge.to;
            int weight = edge.weight;
            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    Path path;
    path.totalWeight = dist[target];
    for (int at = target; at != -1; at = prev[at])
    {
        path.nodes.push_back(at);
    }
    reverse(path.nodes.begin(), path.nodes.end());
    return path;
}

// Yen's Algorithm to find the k-shortest paths from source to target
vector<Path> yenKShortestPaths(int source, int target, int k)
{
    vector<Path> shortestPaths;
    priority_queue<Path> candidates;

    Path initialPath = dijkstra(source, target);
    if (initialPath.totalWeight == INF)
        return shortestPaths;
    shortestPaths.push_back(initialPath);

    for (int i = 1; i < k; ++i)
    {
        for (int j = 0; j < shortestPaths[i - 1].nodes.size() - 1; ++j)
        {
            int spurNode = shortestPaths[i - 1].nodes[j];
            vector<int> rootPath(shortestPaths[i - 1].nodes.begin(), shortestPaths[i - 1].nodes.begin() + j + 1);

            vector<Edge> removedEdges;
            for (const Path &path : shortestPaths)
            {
                if (path.nodes.size() > j && equal(rootPath.begin(), rootPath.end(), path.nodes.begin()))
                {
                    int u = path.nodes[j];
                    int v = path.nodes[j + 1];
                    auto it = remove_if(graph[u].begin(), graph[u].end(), [v](const Edge &e)
                                        { return e.to == v; });
                    if (it != graph[u].end())
                    {
                        removedEdges.push_back(*it);
                        graph[u].erase(it);
                    }
                }
            }

            Path spurPath = dijkstra(spurNode, target);
            if (spurPath.totalWeight != INF)
            {
                Path totalPath;
                totalPath.nodes = rootPath;
                totalPath.nodes.insert(totalPath.nodes.end(), spurPath.nodes.begin() + 1, spurPath.nodes.end());
                totalPath.totalWeight = spurPath.totalWeight + dijkstra(source, spurNode).totalWeight;
                candidates.push(totalPath);
            }

            for (const Edge &edge : removedEdges)
            {
                graph[edge.to].push_back(edge);
            }
        }

        if (candidates.empty())
            break;
        shortestPaths.push_back(candidates.top());
        candidates.pop();
    }

    return shortestPaths;
}

// Function to allocate bands for a connection
bool allocateBands(int u, int v, int bands, const Path &path)
{
    // Check if the path can accommodate the bands
    for (size_t i = 0; i < path.nodes.size() - 1; ++i)
    {
        int from = path.nodes[i];
        int to = path.nodes[i + 1];
        if (spectrum[{from, to}][bands] > 0 || spectrum[{to, from}][bands] > 0)
        {
            return false;
        }
    }

    // Allocate the bands
    for (size_t i = 0; i < path.nodes.size() - 1; ++i)
    {
        int from = path.nodes[i];
        int to = path.nodes[i + 1];
        spectrum[{from, to}][bands]++;
        spectrum[{to, from}][bands]++;
        allocatedBands[{from, to}].push_back(bands);
        allocatedBands[{to, from}].push_back(bands);
    }

    allocatedRoutes[{u, v}] = path;
    return true;
}

int main()
{
    // Initialize the graph with the given weights
    graph.resize(6);
    vector<vector<int>> weights = {
        {0, 400, 100, 200, INF, INF},
        {400, 0, 200, INF, INF, 200},
        {100, 200, 0, 300, INF, 600},
        {200, INF, 300, 0, 400, INF},
        {INF, INF, INF, 400, 0, 500},
        {INF, 200, 600, INF, 500, 0}};

    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            if (weights[i][j] != INF && i != j)
            {
                graph[i].push_back({j, weights[i][j]});
            }
        }
    }

    // Initialize the spectrum map with 0s
    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            if (i != j)
            {
                spectrum[{i, j}].resize(BANDS, 0);
            }
        }
    }

    // Generate connections and allocate bands
    vector<pair<int, int>> blockedConnections;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 10);

    for (int u = 0; u < 6; ++u)
    {
        for (int v = 0; v < 6; ++v)
        {
            if (u != v)
            {
                vector<Path> kShortestPaths = yenKShortestPaths(u, v, 3);
                int bandsToAllocate = dist(gen);

                bool allocated = false;
                for (const Path &path : kShortestPaths)
                {
                    if (allocateBands(u, v, bandsToAllocate, path))
                    {
                        allocated = true;
                        break;
                    }
                }

                if (!allocated)
                {
                    blockedConnections.push_back({u, v});
                }
                else
                {
                    // Ensure the route is added to allocatedRoutes even if the allocation fails
                    if (allocatedRoutes.find({u, v}) == allocatedRoutes.end() && !kShortestPaths.empty())
                    {
                        allocatedRoutes[{u, v}] = kShortestPaths[0];
                    }
                }
            }
        }
    }

    // Output the blocked connections
    cout << "Blocked Connections:" << endl;
    for (const auto &conn : blockedConnections)
    {
        cout << conn.first << "-" << conn.second << endl;
    }

    // Output the allocated bands and routes
    cout << "\nAllocated Bands and Routes:" << endl;
    for (const auto &entry : allocatedBands)
    {
        int u = entry.first.first;
        int v = entry.first.second;
        cout << u << "-" << v << ": Bands: ";
        for (int band : entry.second)
        {
            cout << band << " ";
        }
        cout << "Route: ";
        if (allocatedRoutes.find({u, v}) != allocatedRoutes.end())
        {
            for (int node : allocatedRoutes[{u, v}].nodes)
            {
                cout << node << " ";
            }
        }
        cout << endl;
    }

    return 0;
}
