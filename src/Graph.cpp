/**
 * @file Graph.cpp
 * @brief Implémente les méthodes de la classe Graph pour représenter un graphe avec des noeuds et des arêtes.
 */

#include <iostream>
#include "../include/Graph.h"
#include <random>
#include <algorithm>

/**
 * @brief Constructeur de la classe Graph.
 * @param numNodes Le nombre de noeuds dans le graphe.
 * @throw std::invalid_argument si numNodes est négatif.
 */
Graph::Graph(int numNodes) : numNodes(numNodes) {
    if (numNodes < 0) {
        throw std::invalid_argument("Le nombre de noeuds ne peut pas être négatif.");
    }

    // Initialise conflictCount avec des zéros en fonction de la taille de numNodes
    conflictCount.resize(numNodes, 0);
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
 * @brief Obtient un vecteur de conflit pour chaque noeud.
 * @return Une référence vers le vecteur de conflits.
 */
std::vector<int>& Graph::getConflictCount() {
    return conflictCount;
}

/**
 * @brief Obtient un vecteur de noeuds du graphe (constante).
 * @return Une référence constante vers le vecteur de noeuds.
 */
[[nodiscard]] const std::vector<Node>& Graph::getNodes() const {
    return nodes;
}

/**
 * @brief Met à jour le tableau conflictCount en entier avec la colorisation actuelle du graphe.
 * Un conflit est défini comme un sommet ayant des voisins partageant la même couleur.
 */
void Graph::setConflictCount() {
    for (const Node& node : nodes) {
        conflictCount[node.getID()] = node.countConflict(*this);
    }
}


/**
 * @brief Met à jour le tableau conflictCount en entier avec le vecteur donné en paramètre.
 * @param conflictVector vecteur de conflit à mettre dans notre graphe
 */
void Graph::setConflictCount(std::vector<int> conflictVector) {
    for (const Node& node : nodes) {
        int id = node.getID();
        conflictCount[id] = conflictVector[id];
    }
}

/**
 * @brief Calcule le nombre de conflits dans le graphe.
 * @return Le nombre de conflits (sommets voisins ayant la même couleur).
 */
int Graph::countConflicts() const {
    int conflictCpt = 0;
    for (int i = 0; i < nodes.size(); ++i) {
        conflictCpt += conflictCount[i];
    }
    return conflictCpt / 2;
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
    // Initialise le vecteur conflictCount de la copie avec la même taille que le vecteur original
    clonedGraph.conflictCount.resize(numNodes, 0);
    // Met à jour le vecteur conflictCount de la copie
    clonedGraph.setConflictCount(conflictCount);
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

    std::cout << "Contenu de conflit : ";
    for (int i = 0; i < numNodes; i++) {
        std::cout << conflictCount[i] << " ";
    }
    std::cout << std::endl;
}

/**
 * @brief Change aléatoirement la couleur de plusieurs noeuds différents.
 * @param numChange Nombre de noeud à changer.
 * @param k Le nombre de couleur différents.
 * @param rng Générateur de nombres aléatoires.
 */
void Graph::recolorNodes(int numChange, int k, std::mt19937 rng) {
    // Vérifie si X est valide
    if (numChange < 0) {
        std::cerr << "Le nombre de noeuds à recolorier ne peut pas être negatif." << std::endl;
        return;
    }

    // Création d'une liste des couleurs
    std::vector<int> colorList;
    colorList.reserve(k);
    for (int i = 0; i < k; ++i) {
        colorList.push_back(i);
    }
    // Crée un vecteur de nœuds ayant au moins un conflit
    std::vector<Node*> nodesWithConflict;
    for (Node& node : nodes) {
        if (conflictCount[node.getID()] > 0) {
            nodesWithConflict.push_back(&node);
        }
    }

    // Si numChange est plus grand que le nombre de noeud en conflit, on change numChange
    if (nodesWithConflict.size() < static_cast<std::vector<Node*>::size_type>(numChange)) {
        numChange = static_cast<int>(nodesWithConflict.size());
    }

    // Mélange le vecteur pour choisir des noeuds aléatoirement
    std::shuffle(nodesWithConflict.begin(), nodesWithConflict.end(), rng);

    // Recolorie les X premiers nœuds
    for (int i = 0; i < numChange; ++i) {
        Node* node = nodesWithConflict[i];
        int currentColor = node->getColor();
        // On mélange aléatoirement la liste des couleurs
        std::shuffle(colorList.begin(), colorList.end(), rng);
        // La nouvelle couleur du noeud est la première couleur de la liste
        int newColor = colorList[0];
        // Si cette couleur était la même que l'ancienne, on prend la couleur suivante
        if (newColor == currentColor){
            newColor = colorList[1];
        }

        // Mettez à jour conflictCount en conséquence
        for (int neighborID : node->getNeighbors()) {
            const Node& neighbor = getNode(neighborID);
            if (neighbor.getColor() == currentColor) {
                conflictCount[neighborID]--; // Réduction du conflit
                conflictCount[node->getID()]--;
            }
            if (neighbor.getColor() == newColor) {
                conflictCount[neighborID]++; // Augmentation du conflit
                conflictCount[node->getID()]++;
            }
        }

        node->setColor(newColor);
    }
}

void Graph::recolorAllNodes(int numChange, int k, std::mt19937 rng){
    if (numChange < 0) {
        std::cerr << "Le nombre de noeuds à recolorier ne peut pas être negatif." << std::endl;
        return;
    }

    std::uniform_int_distribution<int> distribution(0, numNodes-1);

    // Création d'une liste des couleurs
    std::vector<int> colorList;
    colorList.reserve(k);
    for (int i = 0; i < k; ++i) {
        colorList.push_back(i);
    }

    for (int i = 0; i < numChange; i++){
        Node& node = nodes[distribution(rng)];
        int currentColor = node.getColor();

        // On mélange aléatoirement la liste des couleurs
        std::shuffle(colorList.begin(), colorList.end(), rng);
        // La nouvelle couleur du noeud est la première couleur de la liste
        int newColor = colorList[0];
        // Si cette couleur était la même que l'ancienne, on prend la couleur suivante
        if (newColor == currentColor){
            newColor = colorList[1];
        }

        // Mettez à jour conflictCount en conséquence
        for (int neighborID : node.getNeighbors()) {
            const Node& neighbor = getNode(neighborID);
            if (neighbor.getColor() == currentColor) {
                conflictCount[neighborID]--; // Réduction du conflit
                conflictCount[node.getID()]--;
            }
            if (neighbor.getColor() == newColor) {
                conflictCount[neighborID]++; // Augmentation du conflit
                conflictCount[node.getID()]++;
            }
        }
        node.setColor(newColor);
    }

}