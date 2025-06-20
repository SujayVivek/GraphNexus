#include "../core/graph.hpp"
#include "httplib.h"
#include "json.hpp"


using namespace httplib;
using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <edge_list.txt> <bind_address> <port>\n", argv[0]);
        return 1;
    }
    const char* edge_file = argv[1];
    const char* bind_addr = argv[2];      // e.g. "0.0.0.0"
    int port             = atoi(argv[3]); // e.g. 8080

    // 1) Load graph into memory
    Graph g;
    try {
        g.loadFromEdgeList(edge_file);
    } catch (const std::exception& e) {
        fprintf(stderr, "Error loading graph: %s\n", e.what());
        return 1;
    }
    printf("Loaded graph with %zu nodes\n", g.size()); 

    // 2) Start HTTP server
    Server svr;

    // 2a) /neighbors?node=<id>
    svr.Get("/neighbors", [&](const Request& req, Response& res) {
        if (!req.has_param("node")) {
            res.status = 400;
            res.set_content("Missing ?node=<id>", "text/plain");
            return;
        }
        int u = stoi(req.get_param_value("node"));
        const auto& nbrs = g.neighbors(u);

        json j;
        j["neighbors"] = nbrs;  // vector<int> is serialized automatically
        res.set_content(j.dump(), "application/json");
    });

    // 2b) /bfs?start=<id>
    svr.Get("/bfs", [&](const Request& req, Response& res) {
        if (!req.has_param("start")) {
            res.status = 400;
            res.set_content("Missing ?start=<id>", "text/plain");
            return;
        }
        int s = stoi(req.get_param_value("start"));
        auto order = g.bfs(s);

        json j;
        j["order"] = order;
        res.set_content(j.dump(), "application/json");
    });

    printf("Listening on %s:%d …\n", bind_addr, port);
    svr.listen(bind_addr, port);
    return 0;
}
