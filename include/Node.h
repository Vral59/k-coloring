#ifndef NODE_H
#define NODE_H

#include <list>

class Node {
public:
    explicit Node(int id);
    [[nodiscard]] int getID() const;
    [[nodiscard]] int getColor() const;
    void setColor(int c);
    [[nodiscard]] const std::list<int>& getNeighbors() const;
    void addNeighbor(int neighborID);

private:
    int id;
    int color;
    std::list<int> neighbors;
};

#endif
