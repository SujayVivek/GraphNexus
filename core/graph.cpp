#include "graph.hpp"
#include <stdexcept>
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