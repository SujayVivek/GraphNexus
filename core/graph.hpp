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

private:
    unordered_map<int, vector<int>> adj;
    vector<int> empty;
};
