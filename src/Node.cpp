/**
 * @file Node.cpp
 * @brief Implémente les méthodes de la classe Node pour représenter un noeud dans un graphe.
 */

#include "../include/Node.h"

/**
 * @brief Constructeur de la classe Node.
 * @param id L'ID du noeud.
 */
Node::Node(int id) : id(id), color(-1) {}

/**
 * @brief Obtient l'ID du noeud.
 * @return L'ID du noeud.
 */
[[nodiscard]] int Node::getID() const {
    return id;
}

/**
 * @brief Obtient la couleur du noeud.
 * @return La couleur du noeud. -1 signifie que la couleur n'a pas été attribuée.
 */
[[nodiscard]] int Node::getColor() const {
    return color;
}

/**
 * @brief Définit la couleur du noeud.
 * @param c La couleur à attribuer.
 * @throw std::invalid_argument si la couleur c est négative.
 */
void Node::setColor(int c) {
    if (c < 0) {
        throw std::invalid_argument("La couleur ne peut pas être négative.");
    }
    color = c;
}

/**
 * @brief Obtient la liste des ID des noeuds voisins.
 * @return Une référence constante vers la liste des ID des noeuds voisins.
 */
[[nodiscard]] const std::list<int>& Node::getNeighbors() const {
    return neighbors;
}

/**
 * @brief Ajoute un noeud voisin.
 * @param neighborID L'ID du noeud voisin à ajouter.
 */
void Node::addNeighbor(int neighborID) {
    neighbors.push_back(neighborID);
}
