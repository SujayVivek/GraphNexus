#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "graph_service.grpc.pb.h"
using namespace std;
#include "../core/graph.hpp"  // adjust path as needed

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using graphnexus::GraphService;
using graphnexus::NeighborRequest;
using graphnexus::NeighborResponse;

class GraphServiceImpl final : public GraphService::Service {
public:
    // Constructor: load graph once
    GraphServiceImpl(const string& edge_list_path) {
        graph_.loadFromEdgeList(edge_list_path);
        cout << "[Server] Loaded graph from " << edge_list_path << "\n";
    }

    // RPC method implementation
    Status GetNeighbors(ServerContext* /*ctx*/,
                        const NeighborRequest* request,
                        NeighborResponse* reply) override {
        int node = request->node_id();
        const auto& nbrs = graph_.neighbors(node);
        for (int v : nbrs) {
            reply->add_neighbors(v);
        }
        return Status::OK;
    }

private:
    Graph graph_;
};

void RunServer(const string& address, const string& edge_list_path) {
    GraphServiceImpl service(edge_list_path);

    ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "[Server] Listening on " << address << "\n";

    server->Wait();
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0]
                  << " <bind_address:port> <edge_list.txt>\n";
        return 1;
    }
    string address = argv[1];        // e.g. "0.0.0.0:50051"
    string edge_list_path = argv[2];

    RunServer(address, edge_list_path);
    return 0;
}
