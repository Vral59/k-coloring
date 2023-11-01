/**
 * @file Graph.h
 * @brief Définit la classe Graph qui représente un graphe avec des noeuds et des arêtes.
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include <vector>
#include <stdexcept>
#include <chrono>
#include <random>

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
     * @brief Constructeur par défaut de la classe Graph.
     * Initialise le graphe avec un nombre de nœuds égal à zéro.
     */
    Graph() : numNodes(0) {}

    /**
     * @brief Met à jour le tableau conflictCount en entier avec la colorisation actuelle du graphe.
     * Un conflit est défini comme un sommet ayant des voisins partageant la même couleur.
     */
    void setConflictCount();

    /**
     * @brief Met à jour le tableau conflictCount en entier avec le vecteur donné en paramètre.
     * @param conflictVector vecteur de conflit à mettre dans notre graphe
     */
    void setConflictCount(std::vector<int> conflictVector);

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
     * @brief Obtient un vecteur de conflit pour chaque noeud.
     * @return Une référence vers le vecteur de conflits.
     */
    std::vector<int>& getConflictCount();

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
     * @brief Ajoute un noeud au graphe.
     * @param n le noeud à ajouter.
     */
    void addNode(const Node& n);

    /**
     * @brief Crée une copie en profondeur du graphe.
     * @return Une nouvelle instance de Graph copiée en profondeur.
     */
    [[nodiscard]] Graph clone();

    /**
     * @brief Affiche le contenu du graphe, y compris les noeuds, leurs couleurs et leurs voisins.
     */
    void displayGraph() const;

    /**
     * @brief Change aléatoirement la couleur de plusieurs noeuds différents.
     * @param numChange Nombre de noeud à changer.
     * @param k Le nombre de couleur différents.
     * @param rng Générateur de nombres aléatoires (avec une valeur par défaut).
     */
    void recolorNodes(int numChange, int k, std::mt19937 rng);

private:
    int numNodes; // Le nombre de noeuds dans le graphe.
    std::vector<Node> nodes; // Le vecteur de noeuds du graphe.
    std::vector<int> conflictCount; // Le vecteur qui compte les conflits du graphe.
};

#endif
