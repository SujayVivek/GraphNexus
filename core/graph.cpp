#include "graph.hpp"
#include <stdexcept>
#include <queue>
#include <limits>
void Graph::loadFromEdgeList(const string& path) {
    ifstream in(path);
    if (!in) {
        throw runtime_error("Cannot open file " + path);
    }
    string line;
    // Read line by line
    while (getline(in, line)) {
        istringstream iss(line);
        int u, v;
        if (!(iss >> u >> v)) continue;
        adj[u].push_back(v);   
    }
}

const vector<int>& Graph::neighbors(int u) const {
    auto it = adj.find(u);
    if (it == adj.end()) return empty;   
    return it->second;
}

vector<int> Graph::bfs(int start) const {
    vector<int> order;
    unordered_set<int> visited;
    queue<int> q;
    visited.insert(start);
    q.push(start);

    while (!q.empty()) {
        int u = q.front(); 
        q.pop();
        order.push_back(u);
        auto it = adj.find(u);
        if (it == adj.end()) continue;

        for (int v : it->second) {
            if (visited.insert(v).second) {
                q.push(v);
            }
        }
    }
    return order;
}

size_t Graph::size() const {
  return adj.size();
}

int Graph::shortestDistance(int u, int v) const {
    if (u == v) return 0;
    unordered_set<int> visited;
    queue<pair<int,int>> q;   // pair<node,dist>
    visited.insert(u);
    q.push({u, 0});

    while (!q.empty()) {
      auto p = q.front(); q.pop();
      int node = p.first;
      int dist = p.second;
        auto it = adj.find(node);
        if (it == adj.end()) continue;
        for (int w : it->second) {
            if (w == v) return dist + 1;
            if (visited.insert(w).second) {
                q.push({w, dist + 1});
            }
        }
    }
    return -1;  // no path found
}

// === dijkstra ===
// Simple Dijkstra: weight=1 per edge. You can extend to real weights later.
int Graph::dijkstra(int u, int v) const {
    if (u == v) return 0;
    const int INF = std::numeric_limits<int>::max() / 2;
    // distance map
    unordered_map<int,int> dist;
    for (auto& kv : adj) dist[kv.first] = INF;
    dist[u] = 0;

    // min‑heap of (distance, node)
    using P = pair<int,int>;
    priority_queue<P, vector<P>, greater<P>> pq;
    pq.push({0, u});

    while (!pq.empty()) {
        auto p = pq.top(); pq.pop();
      int d    = p.first;
        int node = p.second;
        if (d > dist[node]) continue;
        if (node == v) return d;
        auto it = adj.find(node);
        if (it == adj.end()) continue;
        for (int w : it->second) {
            int nd = d + 1;  // edge weight = 1
            if (nd < dist[w]) {
                dist[w] = nd;
                pq.push({nd, w});
            }
        }
    }
    return -1;  // unreachable
}