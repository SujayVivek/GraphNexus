// cli.cpp
#include "core/graph.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "json.hpp" // you need this header in your include path

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: graph_cli <mode> <edge_file> <args...>\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string edge_file = argv[2];
    Graph g;
    try {
      g.loadFromEdgeList(edge_file);
    } catch (const std::exception& e) {
      std::cerr << "Failed to load graph: " << e.what() << "\n";
      return 3;
    }

    if (mode == "neighbors") {
        int u = std::atoi(argv[3]);
        const auto& nbrs = g.neighbors(u);
        json j = {{"neighbors", nbrs}};
        std::cout << j.dump();
    }
    else if (mode == "bfs") {
        int u = std::atoi(argv[3]);
        auto order = g.bfs(u);
        json j = {{"order", order}};
        std::cout << j.dump();
    }
    else if (mode == "shortest") {
        if (argc != 5) { return 1; }
        int u = std::atoi(argv[3]);
        int v = std::atoi(argv[4]);
        int d = g.shortestDistance(u, v);    // implement this in Graph!
        json j = {{"distance", d}};
        std::cout << j.dump();
    }
    else if (mode == "dijkstra") {
        if (argc != 5) { return 1; }
        int u = std::atoi(argv[3]);
        int v = std::atoi(argv[4]);
        int d = g.dijkstra(u, v);            // implement this in Graph!
        json j = {{"distance", d}};
        std::cout << j.dump();
    }
    else {
        std::cerr << "Unknown mode\n";
        return 1;
    }
    return 0;
}
