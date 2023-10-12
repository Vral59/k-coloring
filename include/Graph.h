#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include <vector>

class Graph {
public:
    explicit Graph(int numNodes);
    void addEdge(int u, int v);
    [[nodiscard]] int getNumNodes() const;
    [[nodiscard]] Node& getNode(int id);

private:
    int numNodes;
    std::vector<Node> nodes;
};

#endif
