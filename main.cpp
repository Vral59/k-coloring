#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <random>
#include <chrono>
#include "include/Node.h"
#include "include/Graph.h"


/**
 * @brief Lit un graphe à partir d'un fichier au format DIMACS et crée un objet Graph correspondant.
 *
 * Cette fonction prend en entrée un nom de fichier, extrait les informations du graphe au format DIMACS, puis crée et
 * renvoie un objet Graph représentant le graphe.
 *
 * @param filename Le nom du fichier contenant les données du graphe au format DIMACS.
 * @return Un objet Graph représentant le graphe extrait du fichier.
 * @throw std::invalid_argument si le fichier ne peut pas être ouvert ou s'il y a des erreurs dans le format du fichier.
 */
Graph readGraphFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::invalid_argument("Impossible d'ouvrir le fichier: " + filename);
    }

    int numNodes = 0;
    int numEdges = 0;

    std::string line; // Déclaration de la variable 'line'

    while (std::getline(file, line)) {
        if (line.find("p edge ") == 0) {
            std::istringstream iss(line);
            iss.ignore(7);
            if (iss >> numNodes >> numEdges) {
                std::cout << "Nombre de noeud: " << numNodes << ", Nombre d'arrete: " << numEdges << std::endl;
            } else {
                std::cerr << "Impossible d'extraire numNodes et numEdges." << std::endl;
            }
            break;
        } else {
            continue;
        }
    }
    // Creation du graphe
    Graph graph(numNodes);

    while (std::getline(file, line)) {
        int u, v;
        if (sscanf(line.c_str(), "e %d %d", &u, &v) == 2) {
            if (u >= 1 && v >= 1) {
                // Ajoutez les arêtes au graphe
                graph.addEdge(u - 1, v - 1);
            } else {
                throw std::invalid_argument("Les IDs des arêtes doivent être supérieurs ou égaux à 1.");
            }
        } else {
            throw std::invalid_argument("Impossible de lire les arêtes: " + filename);
        }
    }
    file.close();
    return graph;
}

/**
 * @brief Colorie le graphe avec un algorithme glouton en utilisant au plus k couleurs.
 *
 * @param graph Le graphe à colorier.
 * @param k Le nombre maximal de couleurs à utiliser.
 */
void greedyColoring(Graph& graph, int k) {

    std::vector<Node>& nodes = graph.getNodes();
    int numNodes = graph.getNumNodes();

    // Créez un vecteur d'indices pour trier les noeuds
    std::vector<int> nodeIndices(numNodes);
    for (int i = 0; i < numNodes; i++) {
        nodeIndices[i] = i;
    }

    // Triez les indices en fonction du degré des noeuds (nombre de voisins)
    std::sort(nodeIndices.begin(), nodeIndices.end(), [&nodes](int a, int b) {
        return nodes[a].getNeighbors().size() > nodes[b].getNeighbors().size();
    });


    std::vector<int> colorAssigned(numNodes, -1); // Initialise toutes les couleurs à -1 (non attribuées)

    for (int i : nodeIndices) {
        Node& node = nodes[i];
        std::vector<bool> neighborColors(k, false);

        // Parcourez les voisins et marquez leurs couleurs comme utilisées
        for (int neighborID : node.getNeighbors()) {
            int neighborColor = colorAssigned[neighborID];
            if (neighborColor >= 0) {
                neighborColors[neighborColor] = true;
            }
        }

        int chosenColor = -1;

        // Attribuez la première couleur non utilisée
        for (int color = 0; color < k; color++) {
            if (!neighborColors[color]) {
                chosenColor = color;
                break;
            }
        }

        if (chosenColor == -1) {
            // Si aucune couleur non utilisée n'est trouvée, attribuez la couleur la moins utilisée parmi les voisins
            std::vector<int> neighborColorCount(k, 0);
            for (int neighborID : node.getNeighbors()) {
                int neighborColor = colorAssigned[neighborID];
                if (neighborColor != -1){
                    neighborColorCount[neighborColor]++;
                }
            }

            int minNeighborCount = k + 1;
            for (int color = 0; color < k; color++) {
                if (neighborColorCount[color] < minNeighborCount) {
                    chosenColor = color;
                    minNeighborCount = neighborColorCount[color];
                }
            }
        }
        node.setColor(chosenColor); // Mettez à jour la couleur du noeud
        colorAssigned[i] = chosenColor;
    }
    graph.setConflictCount();
}

/**
 * @brief Algorithme du récuit simulé.
 * @param graph Graphe à colorier.
 * @param k Le nombre de couleur utilisable.
 * @param initTemp La température initiale.
 * @param coolingRate Le coefficient de refroidissement.
 * @param maxIter Le nombre maximum d'itération.
 * @param nb_changes Nombre de changement de couleur dans le voisinage.
 * @param maxExecutionTimeInSeconds Temps execution maximum de la fonction.
 * @return Le graphe colorié à la fin de l'algorithme.
 */
Graph simulatedAnnealing(Graph& graph, int k, double initTemp, double coolingRate, int maxIter, int nb_changes, int maxExecutionTimeInSeconds) {

    //initialisation des paramètres et de la solution courante
    Graph currentSol = graph.clone();
    Graph best_sol_encountered = graph.clone();
    double currentCost = currentSol.countConflicts();
    double best_value_encountered = best_sol_encountered.countConflicts();
    double temperature = initTemp;
    int index_best_sol = 0;
    int index_last_change = 0;
    long long bestSolTime = 0;
    // Creation d'une seed aléatoire différente dans chaque thread
    unsigned seed = static_cast<unsigned>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // Récupérer le moment où l'algorithme a commencé à s'exécuter
    auto startTime = std::chrono::high_resolution_clock::now();

    //itérations de l'algo de recuit simulé
    for (int i = 0; i < maxIter; i++) {
        //génération du voisin aléatoire
        Graph newSol = currentSol.clone();
        newSol.recolorAllNodes(nb_changes, k, rng);
        double newCost = newSol.countConflicts();

        // choix de garder ou non ce voisin
        if (newCost < currentCost) {
            currentSol = newSol.clone();
            currentCost = newCost;
            index_best_sol = i;
            index_last_change = i;
            auto currentTime = std::chrono::high_resolution_clock::now();
            bestSolTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

        }
        else {
            double acceptanceProbability = exp((currentCost - newCost) / temperature);
            double randomRate = distribution(rng);

            if (randomRate < acceptanceProbability) {
                currentSol = newSol.clone();
                currentCost = newCost;
                index_last_change = i;
            }
        }
        if (newCost < best_value_encountered) {
            best_sol_encountered = newSol.clone();
            best_value_encountered = newCost;
        }
        if ((i % 10) == 0) {
            temperature *= coolingRate;
        }
        // Vérifier si le temps d'exécution dépasse la limite
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTimeInSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

        if (elapsedTimeInSeconds >= maxExecutionTimeInSeconds) {
            // Arrêter l'exécution et retourner la meilleure solution rencontrée jusqu'à présent
            std::cout << "Nombre d'iterations pour meilleure solution: " << index_best_sol << std::endl;
            std::cout << "Temps pour meilleure solution: " << bestSolTime << std::endl;
            std::cout << "Nombre d'iterations au dernier changement de solution courante: " << index_last_change << std::endl;
            std::cout << "Temperature finale: " << temperature << std::endl;
            return best_sol_encountered;
        }
    }
    std::cout << "Nombre d'iterations pour meilleure solution: " << index_best_sol << std::endl;
    std::cout << "Temps pour meilleure solution: " << bestSolTime << std::endl;
    std::cout << "Nombre d'iterations au dernier changement de solution courante: " << index_last_change << std::endl;
    std::cout << "Temperature finale: " << temperature << std::endl;

    return best_sol_encountered;
}

/**
 * @brief Algorithme de recherche local, choisi à chaque tour un noeud aléatoirement et lui donne la couleur qui
 * minimise les conflits.
 * @param graph Graphe à colorier.
 * @param k Le nombre de couleur utilisable.
 * @param maxIter Le nombre maximum d'itération.
 */
void localResearch(Graph& graph, int k, int maxIter) {
    unsigned seed = static_cast<unsigned>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> distribution(0, graph.getNumNodes() - 1);

    for (int i = 0; i < maxIter; i++) {
        int valueRng = distribution(rng);
        Node& node = graph.getNodes()[valueRng];
        std::vector<int> neighborColorCount(k, 0);
        int oldColor = graph.getNodes()[valueRng].getColor();

        // Compte les conflits du à chaque couleurs
        for (int neighborID : node.getNeighbors()) {
            int neighborColor = graph.getNodes()[neighborID].getColor();
            if (neighborColor != -1) {
                neighborColorCount[neighborColor]++;
            }
        }

        // Récupère le nombre minimum de conflits
        int minValue = *std::min_element(neighborColorCount.begin(), neighborColorCount.end());
        std::vector<int> minColors;

        // Crée la liste des couleurs possibles
        for (int color = 0; color < k; color++) {
            if (neighborColorCount[color] == minValue) {
                minColors.push_back(color);
            }
        }

        // Sélection aléatoire parmi les couleurs minimales
        std::uniform_int_distribution<int> distributionMinColors(0, static_cast<int>(minColors.size()) - 1);
        int idxMin = minColors[distributionMinColors(rng)];
        node.setColor(idxMin);

        // Mettez à jour conflictCount en conséquence
        for (int neighborID : node.getNeighbors()) {
            const Node& neighbor = graph.getNode(neighborID);
            if (neighbor.getColor() == oldColor) {
                graph.getConflictCount()[neighborID]--; // Réduction du conflit
                graph.getConflictCount()[node.getID()]--;
            }
            if (neighbor.getColor() == idxMin) {
                graph.getConflictCount()[neighborID]++; // Augmentation du conflit
                graph.getConflictCount()[node.getID()]++;
            }
        }
    }
}


int main(int argc, char* argv[]) {
    try {
        std::string filename;
        int k = -1;  // Ne pas initialiser avec une valeur par défaut
        // Recherche de l'argument --file
        for (int i = 1; i < argc; ++i) {
            // Recherche de l'argument --file
            if (std::string(argv[i]) == "--file" && i + 1 < argc) {
                filename = argv[i + 1];
            }
            // Recherche de l'argument --k
            if (std::string(argv[i]) == "--k" && i + 1 < argc) {
                k = std::stoi(argv[i + 1]);
            }
        }

        if (filename.empty() || k == -1) {
            std::cerr << "Utilisation : " << argv[0] << " --file <nom_du_fichier> --k <valeur_de_k>" << std::endl;
            return 1;
        }

        // Lecture du graphe depuis le fichier spécifié
        Graph graph = readGraphFromFile(filename);

        // Utilisation de l'heuristique et colorier le graphe avec k couleur
        greedyColoring(graph, k);

        // Calcul du conflit dans le graphe colorié
        std::cout << "Dans le graphe il y a : " << graph.countConflicts() << " conflit(s) en utilisant l'heuristique"
                  << std::endl;

        // Utilisation du recuit simulé
        auto start_time = std::chrono::high_resolution_clock::now();
        Graph annealing = simulatedAnnealing(graph, k, 1000, 0.995, 75000, 1, 60);
        auto end_time = std::chrono::high_resolution_clock::now();

        // Calculez la durée d'exécution en secondes
        std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(
                end_time - start_time);
        std::cout << "Temps d'execution un recuit simulee : " << duration.count() << " secondes" << std::endl;

        std::cout << "Dans le graphe apres recuit il y a : " << annealing.countConflicts() << " conflit(s)"
                  << std::endl;

        // Utilisation de la recherche local
        auto start_time_local = std::chrono::high_resolution_clock::now();
        localResearch(annealing, k, 500000);
        auto end_time_local = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_local = std::chrono::duration_cast<std::chrono::duration<double>>(
                end_time_local - start_time_local);
        std::cout << "Temps d'execution de la recherche local : " << duration_local.count() << " secondes" << std::endl;

        std::cout << "Dans le graphe apres recuit et recherche local il y a : " << annealing.countConflicts() << " conflit(s)"
                  << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }
    return 0;
}