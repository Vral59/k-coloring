#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "include/Node.h"
#include "include/Graph.h"
#include <random>

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
                std::cout << "numNodes: " << numNodes << ", numEdges: " << numEdges << std::endl;
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
}

Graph simulatedAnnealing(Graph& graph, int k, double initTemp, double coolingRate, int maxIter, int nb_changes) {

//initialisation des paramètres et de la solution courante
Graph currentSol = graph.clone();
Graph best_sol_encountered = graph.clone();
double currentCost = currentSol.countConflicts();
double best_value_encountered = best_sol_encountered.countConflicts();
double temperature = initTemp;
double random_rate = 0;

//itérations de l'algo de recuit simulé
for (int i = 0; i < maxIter; i++) {

//génération du voisin aléatoire
Graph newSol = currentSol.clone(); 
newSol.recolorNodes(nb_changes,k);
double newCost = newSol.countConflicts();

// choix de garder ou non ce voisin
if (newCost < currentCost) {
currentSol = newSol.clone();
currentCost = newCost;
} else {
double acceptanceProbability = exp((currentCost - newCost) / temperature);
srand(time(NULL));
random_rate = (double)rand() / (double)RAND_MAX;
if (random_rate < acceptanceProbability) {
currentSol = newSol.clone();
currentCost = newCost;
}
}
if (newCost < best_value_encountered) {
    best_sol_encountered = newSol.clone();
    best_value_encountered = newCost;
}
temperature *= coolingRate;
}

return best_sol_encountered;
}



int main(int argc, char* argv[]) {

    try {
        std::string filename;
        int k = -1;  // Ne pas initialiser avec une valeur par défaut

        // Recherche de l'argument --file
        for (int i = 1; i < argc; ++i) {
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

        // Colorier le graphe avec la valeur de k
        greedyColoring(graph, k);

        // Affichez le graphe
        graph.displayGraph();

        // Exemple à faire du graph1.col du fonctionnement de la deepcopy d'un graphe et de sa recolorisation
        std::cout << "Je vais recolorier un noeud aleatoirement" << std::endl;
        Graph newGraph = graph.clone();
        newGraph.recolorNodes(1, k);
        newGraph.displayGraph();

        // Calcul du conflit dans le graphe colorié
        std::cout << "Dans le graphe il y a : " << graph.countConflicts() << " conflit(s)" << std::endl;
        std::cout << "Dans le graphe recolorier il y a : " << newGraph.countConflicts() << " conflit(s)" << std::endl;
        
        // test recuit simulé
        Graph test_annealing = simulatedAnnealing(newGraph, k, 10000, 0.9, 5000, 1);
        std::cout << "Dans le graphe après recuit il y a : " << test_annealing.countConflicts() << " conflit(s)" << std::endl;



    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

