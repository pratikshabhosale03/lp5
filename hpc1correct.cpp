#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Graph {
    int V;
    vector<vector<int>> adj;

    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
};

vector<int> BFS_Normal(Graph& g, int start) {
    set<int> visited;
    queue<int> q;
    vector<int> traversalOrder;

    visited.insert(start);
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        traversalOrder.push_back(u);

        for (int v : g.adj[u]) {
            if (visited.find(v) == visited.end()) {
                visited.insert(v);
                q.push(v);
            }
        }
    }
    return traversalOrder;
}

vector<int> BFS_Parallel(Graph& g, int start) {
    set<int> visited;
    queue<int> q;
    vector<int> traversalOrder;

    visited.insert(start);
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        traversalOrder.push_back(u);

        #pragma omp parallel for
        for (int i = 0; i < g.adj[u].size(); i++) {
            int v = g.adj[u][i];
            if (visited.find(v) == visited.end()) {
                #pragma omp critical
                {
                    if (visited.find(v) == visited.end()) {
                        visited.insert(v);
                        q.push(v);
                    }
                }
            }
        }
    }
    return traversalOrder;
}

void DFSUtil(Graph& g, int u, vector<bool>& visited, vector<int>& traversalOrder) {
    visited[u] = true;
    traversalOrder.push_back(u);

    for (int v : g.adj[u]) {
        if (!visited[v]) {
            DFSUtil(g, v, visited, traversalOrder);
        }
    }
}

vector<int> DFS_Normal(Graph& g, int start) {
    vector<bool> visited(g.V, false);
    vector<int> traversalOrder;
    DFSUtil(g, start, visited, traversalOrder);
    return traversalOrder;
}

vector<int> DFS_Parallel(Graph& g, int start) {
    vector<bool> visited(g.V, false);
    vector<int> traversalOrder;

    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            DFSUtil(g, start, visited, traversalOrder);
        }
    }
    return traversalOrder;
}

void printArray(const vector<int>& arr) {
    cout << "[ ";
    for (int val : arr) {
        cout << val << " ";
    }
    cout << "]" << endl;
}

int main() {
    int V, edgeCount, startNode;
    cout << "Input vertices and edges: ";
    cin >> V >> edgeCount;

    Graph g(V);
    cout << "Edges (format: u v):\n";
    for (int i = 0; i < edgeCount; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    cout << "Enter start node for traversal: ";
    cin >> startNode;

    auto start = high_resolution_clock::now();
    vector<int> bfsNormal = BFS_Normal(g, startNode);
    auto stop = high_resolution_clock::now();
    cout << "\nNormal BFS Traversal Order: ";
    printArray(bfsNormal);
    cout << "BFS (Normal) Time: " << duration_cast<microseconds>(stop - start).count() << "microseconds\n";

    start = high_resolution_clock::now();
    vector<int> bfsParallel = BFS_Parallel(g, startNode);
    stop = high_resolution_clock::now();
    cout << "\nParallel BFS Traversal Order: ";
    printArray(bfsParallel);
    cout << "BFS (Parallel) Time: " << duration_cast<microseconds>(stop - start).count() << "microseconds\n";

    start = high_resolution_clock::now();
    vector<int> dfsNormal = DFS_Normal(g, startNode);
    stop = high_resolution_clock::now();
    cout << "\nNormal DFS Traversal Order: ";
    printArray(dfsNormal);
    cout << "DFS (Normal) Time: " << duration_cast<microseconds>(stop - start).count() << "microseconds\n";

    start = high_resolution_clock::now();
    vector<int> dfsParallel = DFS_Parallel(g, startNode);
    stop = high_resolution_clock::now();
    cout << "\nParallel DFS Traversal Order: ";
    printArray(dfsParallel);
    cout << "DFS (Parallel) Time: " << duration_cast<microseconds>(stop - start).count() << "microseconds\n";

    return 0;
}
