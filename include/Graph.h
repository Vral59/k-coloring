/**
 * @file Graph.h
 * @brief Définit la classe Graph qui représente un graphe avec des noeuds et des arêtes.
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include <vector>
#include <stdexcept>

/**
 * @class Graph
 * @brief Représente un graphe composé de noeuds et d'arêtes.
 */
class Graph {
public:
    /**
     * @brief Constructeur de la classe Graph.
     * @param numNodes Le nombre de noeuds dans le graphe.
     * @throw std::invalid_argument si numNodes est négatif.
     */
    explicit Graph(int numNodes);

    /**
     * @brief Ajoute une arête entre deux noeuds du graphe.
     * @param u L'ID du premier noeud.
     * @param v L'ID du deuxième noeud.
     * @throw std::invalid_argument si u ou v sont en dehors de la plage [0, numNodes-1].
     */
    void addEdge(int u, int v);

    /**
     * @return Le nombre de noeuds dans le graphe.
     */
    [[nodiscard]] int getNumNodes() const;

    /**
     * @brief Obtient un noeud spécifique du graphe.
     * @param id L'ID du noeud à récupérer.
     * @return Une référence vers le noeud.
     * @throw std::out_of_range si id est en dehors de la plage [0, numNodes-1].
     */
    [[nodiscard]] const Node& getNode(int id) const;

    /**
     * @brief Obtient un vecteur de noeuds du graphe.
     * @return Une référence vers le vecteur de noeuds.
     */
    std::vector<Node>& getNodes();

    /**
     * @brief Obtient un vecteur de noeuds du graphe (constante).
     * @return Une référence constante vers le vecteur de noeuds.
     */
    [[nodiscard]] const std::vector<Node>& getNodes() const;

    /**
     * @brief Calcule le nombre de conflits dans le graphe.
     * @return Le nombre de conflits (sommets voisins ayant la même couleur).
     */
    [[nodiscard]] int countConflicts() const;

    /**
     * @brief Affiche le contenu du graphe, y compris les noeuds, leurs couleurs et leurs voisins.
     */
    void displayGraph() const;

private:
    int numNodes; // Le nombre de noeuds dans le graphe.
    std::vector<Node> nodes; // Le vecteur de noeuds du graphe.
};

#endif
