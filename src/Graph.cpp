#include "../include/Graph.h"

Graph::Graph(int numNodes) : numNodes(numNodes) {
    for (int i = 0; i < numNodes; i++) {
        nodes.emplace_back(i);
    }
}

void Graph::addEdge(int u, int v) {
    nodes[u].addNeighbor(v);
    nodes[v].addNeighbor(u);
}

int Graph::getNumNodes() const {
    return numNodes;
}

Node& Graph::getNode(int id) {
    return nodes[id];
}
