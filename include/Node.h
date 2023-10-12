/**
 * @file Node.h
 * @brief Définit la classe Node qui représente un noeud dans un graphe.
 */

#ifndef NODE_H
#define NODE_H

#include <list>
#include <stdexcept> // Pour les exceptions

/**
 * @class Node
 * @brief Représente un noeud dans un graphe.
 */
class Node {
public:
    /**
     * @brief Constructeur de la classe Node.
     * @param id L'ID du noeud.
     */
    explicit Node(int id);

    /**
     * @brief Obtient l'ID du noeud.
     * @return L'ID du noeud.
     */
    [[nodiscard]] int getID() const;

    /**
     * @brief Obtient la couleur du noeud.
     * @return La couleur du noeud. -1 signifie que la couleur n'a pas été attribuée.
     */
    [[nodiscard]] int getColor() const;

    /**
     * @brief Définit la couleur du noeud.
     * @param c La couleur à attribuer.
     * @throw std::invalid_argument si la couleur c est négative.
     */
    void setColor(int c);

    /**
     * @brief Obtient la liste des ID des noeuds voisins.
     * @return Une référence constante vers la liste des ID des noeuds voisins.
     */
    [[nodiscard]] const std::list<int>& getNeighbors() const;

    /**
     * @brief Ajoute un noeud voisin.
     * @param neighborID L'ID du noeud voisin à ajouter.
     */
    void addNeighbor(int neighborID);

private:
    int id; // L'ID du noeud.
    int color; // La couleur du noeud, -1 signifie que la couleur n'a pas été attribuée.
    std::list<int> neighbors; // La liste des ID des noeuds voisins.
};

#endif
