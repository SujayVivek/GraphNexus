#include <iostream>
#include "graph.hpp"

int main(int argc, char** argv) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0]
             << " edge_list.txt [neighbors|bfs] nodeID\n";
        return 1;
    }

    const string path   = argv[1];
    const string mode   = argv[2];
    const int    nodeID = stoi(argv[3]);

    Graph g;
    try {
        g.loadFromEdgeList(path);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    if (mode == "neighbors") {
        const vector<int>& nbrs = g.neighbors(nodeID);
        cout << "Neighbors of " << nodeID << ":\n";
        for (int v : nbrs) {
            cout << "  " << v << "\n";
        }

    } else if (mode == "bfs") {
        vector<int> order = g.bfs(nodeID);
        cout << "BFS order from " << nodeID << ":\n";
        for (int u : order) {
            cout << "  " << u << "\n";
        }

    } else {
        cerr << "Unknown mode '" << mode << "'. Use 'neighbors' or 'bfs'.\n";
        return 1;
    }

    return 0;
}
