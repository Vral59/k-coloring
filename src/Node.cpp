#include "../include/Node.h"

Node::Node(int id) : id(id), color(-1) {}

int Node::getID() const {
    return id;
}

int Node::getColor() const {
    return color;
}

void Node::setColor(int c) {
    color = c;
}

const std::list<int>& Node::getNeighbors() const {
    return neighbors;
}

void Node::addNeighbor(int neighborID) {
    neighbors.push_back(neighborID);
}
