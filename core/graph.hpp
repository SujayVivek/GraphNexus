#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <unordered_set>
using namespace std;
class Graph {
public:
    void loadFromEdgeList(const string& path);
    const vector<int>& neighbors(int u) const;
    vector<int> bfs(int start) const;
    size_t size() const;
    int dijkstra(int u, int v) const;
    int shortestDistance(int u, int v) const;
    const unordered_map<int, vector<int>>& adjacency() const {
        return adj;
    }

private:
    unordered_map<int, vector<int>> adj;
    vector<int> empty;
};
