#include <iostream>  
#include <vector>  
#include <queue>  
#include <set>  
#include <omp.h>  
#include <chrono>  
using namespace std;  
using namespace std::chrono;  
struct Graph  
{  
int V;  
vector<vector<int>> adj;  
Graph(int V) : V(V), adj(V) {}  
void addEdge(int u, int v)  
{  
adj[u].push_back(v);  
adj[v].push_back(u);  
}  
};  
void BFS_Normal(Graph& g, int start)  
{  
set<int> visited;  
queue<int> q;  
visited.insert(start);  
q.push(start);  
while (!q.empty())  
{  
int u = q.front();  
q.pop();  
cout << "[" << u << "] ";  
for (int v : g.adj[u])  
{  
if (visited.find(v) == visited.end())  
{  
visited.insert(v);  
q.push(v);  
}  
}  
}  
cout << endl;  
}  
void BFS_Parallel(Graph& g, int start)  
{  
set<int> visited;  
queue<int> q;  
visited.insert(start);  
q.push(start);  
while (!q.empty())  
{  
   int u = q.front();  
   q.pop();  
   cout << "[" << u << "] ";  
 #pragma omp parallel for  
   for (size_t i = 0; i < g.adj[u].size(); i++)  
{  
    int v = g.adj[u][i];  
    if (visited.find(v) == visited.end())  
{  
 #pragma omp critical  
     {  
      visited.insert(v);  
      q.push(v);  
     }  
    }  
   }  
  }  
  cout << endl;  
 }  
 void DFSUtil(Graph& g, int u, vector<bool>& visited)  
{  
  visited[u] = true;  
  cout << "{" << u << "} ";  
  for (int v : g.adj[u])  
{  
   if (!visited[v])  
{  
    DFSUtil(g, v, visited);  
   }  
  }  
 }  
 void DFS_Normal(Graph& g, int start)  
{  
  vector<bool> visited(g.V, false);  
  DFSUtil(g, start, visited);  
  cout << endl;  
 }  
 void DFS_Parallel(Graph& g, int start)  
{  
  vector<bool> visited(g.V, false);  
 #pragma omp parallel  
  {  
 #pragma omp single nowait  
   {  
DFSUtil(g, start, visited);  
}  
}  
cout << endl;  
}  
int main()  
{  
int V, edgeCount, startNode;  
cout << "Input vertices and edges: ";  
cin >> V >> edgeCount;  
Graph g(V);  
cout << "Edges (format: u v):\n";  
for (int i = 0; i < edgeCount; i++)  
{  
int u, v;  
cin >> u >> v;  
g.addEdge(u, v);  
}  
cout << "Enter start node for traversal: ";  
cin >> startNode;  
auto start = high_resolution_clock::now();  
cout << "\nNormal BFS: ";  
BFS_Normal(g, startNode);  
auto stop = high_resolution_clock::now();  
cout << "BFS (Normal) Time: " << 
duration_cast<microseconds>(stop - start).count() <<  
"µs\n";  
// Measure BFS Parallel Time  
start = high_resolution_clock::now();  
cout << "\nParallel BFS: ";  
BFS_Parallel(g, startNode);  
stop = high_resolution_clock::now();  
cout << "BFS (Parallel) Time: " << 
duration_cast<microseconds>(stop - start).count() <<  
"µs\n";  
// Measure DFS Normal Time  
start = high_resolution_clock::now();  
cout << "\nNormal DFS: ";  
DFS_Normal(g, startNode);  
stop = high_resolution_clock::now();  
cout << "DFS (Normal) Time: " << 
duration_cast<microseconds>(stop - start).count() <<  
"µs\n";  
// Measure DFS Parallel Time  
start = high_resolution_clock::now();  
cout << "\nParallel DFS: ";  
DFS_Parallel(g, startNode);  
stop = high_resolution_clock::now();  
cout << "DFS (Parallel) Time: " << 
duration_cast<microseconds>(stop - start).count() <<  
"µs\n";  
return 0;  
}





//commands : g++ -fopenmp hpc1.cpp -o hpc1
// ./hpc1
