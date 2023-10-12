/**
 * @file Graph.cpp
 * @brief Implémente les méthodes de la classe Graph pour représenter un graphe avec des noeuds et des arêtes.
 */

#include <iostream>
#include "../include/Graph.h"

/**
 * @brief Constructeur de la classe Graph.
 * @param numNodes Le nombre de noeuds dans le graphe.
 * @throw std::invalid_argument si numNodes est négatif.
 */
Graph::Graph(int numNodes) : numNodes(numNodes) {
    if (numNodes < 0) {
        throw std::invalid_argument("Le nombre de noeuds ne peut pas être négatif.");
    }

    for (int i = 0; i < numNodes; i++) {
        nodes.emplace_back(i);
    }
}

/**
 * @brief Ajoute une arête entre deux noeuds du graphe.
 * @param u L'ID du premier noeud.
 * @param v L'ID du deuxième noeud.
 * @throw std::invalid_argument si u ou v sont en dehors de la plage [0, numNodes-1].
 */
void Graph::addEdge(int u, int v) {
    if (u < 0 || u >= numNodes || v < 0 || v >= numNodes) {
        throw std::invalid_argument("Les ID de noeuds sont en dehors de la plage valide.");
    }

    nodes[u].addNeighbor(v);
    nodes[v].addNeighbor(u);
}

/**
 * @return Le nombre de noeuds dans le graphe.
 */
[[nodiscard]] int Graph::getNumNodes() const {
    return numNodes;
}

/**
 * @brief Obtient un noeud spécifique du graphe.
 * @param id L'ID du noeud à récupérer.
 * @return Une référence vers le noeud.
 * @throw std::out_of_range si id est en dehors de la plage [0, numNodes-1].
 */
[[nodiscard]] Node& Graph::getNode(int id) {
    if (id < 0 || id >= numNodes) {
        throw std::out_of_range("ID de noeud en dehors de la plage valide.");
    }

    return nodes[id];
}

/**
 * @brief Obtient un vecteur de noeuds du graphe.
 * @return Une référence vers le vecteur de noeuds.
 */
std::vector<Node>& Graph::getNodes() {
    return nodes;
}

/**
 * @brief Obtient un vecteur de noeuds du graphe (constante).
 * @return Une référence constante vers le vecteur de noeuds.
 */
[[nodiscard]] const std::vector<Node>& Graph::getNodes() const {
    return nodes;
}

/**
 * @brief Affiche le contenu du graphe, y compris les noeuds, leurs couleurs et leurs voisins.
 */
void Graph::displayGraph() const {
    for (int i = 0; i < numNodes; i++) {
        const Node& node = nodes[i];
        std::cout << "Node " << i << " - Color: " << node.getColor() << " - Neighbors: ";
        for (int neighbor : node.getNeighbors()) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
}
