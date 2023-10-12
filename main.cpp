#include <iostream>
#include "include/Node.h"
#include "include/Graph.h"

int main() {
    // Creez un graphe avec 6 noeuds
    Graph graph(6);

    // Ajoutez des arêtes au graphe (dans cet exemple, un graphe non oriente)
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);
    graph.addEdge(5, 0);

    // Attribuez des couleurs aux noeuds (1, 2, 3 de maniere cyclique)
    int color = 1;
    for (int i = 0; i < graph.getNumNodes(); i++) {
        Node& node = graph.getNode(i); // Obtenez une référence à l'objet Node dans le vecteur
        node.setColor(color);
        color = (color % 3) + 1;
    }


    // Affichez les voisins de chaque noeud avec leur couleur
    for (int i = 0; i < graph.getNumNodes(); i++) {
        const Node& node = graph.getNode(i);
        std::cout << "Node " << i << " - Color: " << node.getColor() << " - Neighbors: ";
        for (int neighbor : node.getNeighbors()) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
