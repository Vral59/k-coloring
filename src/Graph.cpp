/**
 * @file Graph.cpp
 * @brief Implémente les méthodes de la classe Graph pour représenter un graphe avec des noeuds et des arêtes.
 */

#include <iostream>
#include "../include/Graph.h"
#include <random>
#include <ctime>
#include <algorithm>
#include <chrono>

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
[[nodiscard]] const Node& Graph::getNode(int id) const {
    if (id < 0 || id >= numNodes) {
        throw std::out_of_range("ID de noeud hors de la plage valide.");
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
 * @brief Calcule le nombre de conflits dans le graphe. Un conflit est défini comme un sommet ayant
 * des voisins partageant la même couleur.
 * @return Le nombre de conflits (sommets voisins ayant la même couleur).
 */
int Graph::countConflicts() const {
    int conflictCount = 0;

    for (const Node& node : nodes) {
        conflictCount += node.countConflict(*this);
    }

    return conflictCount/2;
}

/**
 * @brief Ajoute un noeud au graphe.
 * @param n le noeud à ajouter.
 */
void Graph::addNode(const Node& n){
    numNodes++;
    nodes.push_back(n);
}

/**
 * @brief Crée une copie en profondeur du graphe.
 * @return Une nouvelle instance de Graph copiée en profondeur.
 */
Graph Graph::clone(){
    Graph clonedGraph(0);
    // Copie en profondeur des nœuds.
    for (Node& originalNode : nodes) {
        clonedGraph.addNode(originalNode.clone());
    }
    return clonedGraph;
}

/**
 * @brief Affiche le contenu du graphe, y compris les noeuds, leurs couleurs et leurs voisins.
 */
void Graph::displayGraph() const {
    for (int i = 0; i < numNodes; i++) {
        const Node& node = nodes[i];
        std::cout << "Node " << i << " - Color: " << node.getColor() << " - Neighbors: ";
        for (int neighborID : node.getNeighbors()) {
            std::cout << neighborID << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Change aléatoirement la couleur de plusieurs noeuds différents.
 * @param numChange Nombre de noeud à changer.
 * @param k Le nombre de couleur différents.
 */
void Graph::recolorNodes(int numChange, int k) {
    // Vérifie si X est valide
    if (numChange < 0) {
        std::cerr << "Le nombre de noeuds à recolorier ne peut pas être negatif." << std::endl;
        return;
    }
    // Initialise le générateur de nombres aléatoires avec une seed basée sur le temps actuel
    unsigned seed = static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> colorDist(0, k - 1);

    // Crée un vecteur de nœuds ayant au moins un conflit
    std::vector<Node*> nodesWithConflict;
    for (Node& node : nodes) {
        if (node.countConflict(*this) > 0) {
            nodesWithConflict.push_back(&node);
        }
    }

    // Vérifie si le nombre maximum de noeud possible à recolorier est plus petit que numChange
    if (nodesWithConflict.size() < numChange) {
        std::cerr << "Le nombre de noeuds à recolorier est trop eleve." << std::endl;
        return;
    }

    // Mélange le vecteur pour choisir des noeuds aléatoirement
    std::shuffle(nodesWithConflict.begin(), nodesWithConflict.end(), rng);

    // Recolorie les X premiers nœuds
    for (int i = 0; i < numChange; ++i) {
        Node* node = nodesWithConflict[i];
        int currentColor = node->getColor();
        int newColor = colorDist(rng);

        // Assurez-vous que la nouvelle couleur est différente de la couleur actuelle
        while (newColor == currentColor) {
            newColor = colorDist(rng);
        }

        node->setColor(newColor);
    }
}
