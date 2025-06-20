// cli.cpp
#include "core/graph.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "json.hpp" // you need this header in your include path

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    std::string mode = (argc > 1 ? argv[1] : "");
    // dump only needs 2 args: mode + edge_file
    if ((mode == "dump" && argc < 3) ||
        (mode != "dump" && argc < 4)) {
        std::cerr << "Usage:\n"
                  << "  graph_cli dump <edge_file>\n"
                  << "  graph_cli <mode> <edge_file> <args...>\n";
        return 1;
    }
    // std::string edge_file = argv[2];

    // std::string mode = argv[1];
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
    else if (mode == "dump") {
        std::vector<int> nodes;
        for (auto &kv : g.adjacency()) 
        nodes.push_back(kv.first);

        // Build link list
        std::vector<std::array<int,2>> links;
        for (auto &kv : g.adjacency()) {
        int u = kv.first;
        for (int v : kv.second)
            links.push_back({u, v});
        }

        json j;
        j["nodes"] = nodes;
        j["links"] = json::array();
        for (auto &e : links) {
        j["links"].push_back({{"source", e[0]}, {"target", e[1]}});
        }
        std::cout << j.dump();
    }
    else {
        std::cerr << "Unknown mode\n";
        return 1;
    }
    return 0;
}
