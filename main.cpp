#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <random>
#include <thread>
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
 * @return Le graphe colorié à la fin de l'algorithme.
 */
Graph simulatedAnnealing(Graph& graph, int k, double initTemp, double coolingRate, int maxIter, int nb_changes) {

    //initialisation des paramètres et de la solution courante
    Graph currentSol = graph.clone();
    Graph best_sol_encountered = graph.clone();
    double currentCost = currentSol.countConflicts();
    double best_value_encountered = best_sol_encountered.countConflicts();
    double temperature = initTemp;

    // Creation d'une seed aléatoire différente dans chaque thread
    unsigned seed = static_cast<unsigned>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count() +
            std::hash<std::thread::id>()(std::this_thread::get_id()));
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    //itérations de l'algo de recuit simulé
    for (int i = 0; i < maxIter; i++) {
        //génération du voisin aléatoire
        Graph newSol = currentSol.clone();
        newSol.recolorNodes(nb_changes, k, rng);
        double newCost = newSol.countConflicts();

        // choix de garder ou non ce voisin
        if (newCost < currentCost) {
            currentSol = newSol.clone();
            currentCost = newCost;
        }
        else {
            double acceptanceProbability = exp((currentCost - newCost) / temperature);
            double randomRate = distribution(rng);

            if (randomRate < acceptanceProbability) {
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
        unsigned int numThreads = std::thread::hardware_concurrency();  // Par défaut, utilisez le nombre maximum de threads

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
            // Recherche de l'argument --thread
            if (std::string(argv[i]) == "--thread" && i + 1 < argc) {
                int threadValue = std::stoi(argv[i + 1]);
                if (threadValue > 0 && threadValue < numThreads) {
                    numThreads = threadValue;
                }
            }
        }

        std::cout << "Vous utilisez " << numThreads << " thread(s)" << std::endl;

        if (filename.empty() || k == -1) {
            std::cerr << "Utilisation : " << argv[0] << " --file <nom_du_fichier> --k <valeur_de_k>" << std::endl;
            return 1;
        }

        // Lecture du graphe depuis le fichier spécifié
        Graph graph = readGraphFromFile(filename);

        // Utilisation de l'heuristique
        // Colorier le graphe avec la valeur de k
        greedyColoring(graph, k);

        // Calcul du conflit dans le graphe colorié
        std::cout << "Dans le graphe il y a : " << graph.countConflicts() << " conflit(s) en utilisant l'heuristique" << std::endl;
        
        // Utilisation du recuit simulé

        // Recuit simulée sans multi-threading
        // Enregistrez l'heure actuelle avant d'appeler la fonction
        auto start_time = std::chrono::high_resolution_clock::now();
        Graph test_annealing = simulatedAnnealing(graph, k, 10000, 0.9, 5000, 1);
        // Enregistrez l'heure actuelle après l'exécution de la fonction
        auto end_time = std::chrono::high_resolution_clock::now();

        // Calculez la durée d'exécution en microsecondes (ou autre unité au choix)
        std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);        // Affichez le temps d'exécution
        std::cout << "Temps d'execution un recuit simulee : " << duration.count() << " secondes" << std::endl;

        std::cout << "Dans le graphe apres recuit il y a : " << test_annealing.countConflicts() << " conflit(s)" << std::endl;

        // Recuit simulée avec multi-threading
        // Stocke les différents résultats de chaque graphs
        std::vector<Graph> results(numThreads);

        // Fonction pour exécuter les recuit simulées
        auto runSimulatedAnnealing = [&](int threadIndex, int initTemp, float coolingRate, int maxIter, int nb_changes) {
            results[threadIndex] = simulatedAnnealing(graph, k, initTemp, coolingRate, maxIter, nb_changes);
        };

        std::vector<std::thread> threads;

        // Lancer les différents threads
        threads.reserve(numThreads);
        auto start_time_thread = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(runSimulatedAnnealing, i, 10000, 0.9, 5000, 1);
        }

        // Joindre tous les threads et attendre qu'ils finissent
        for (auto& thread : threads) {
            thread.join();
        }

        auto end_time_thread = std::chrono::high_resolution_clock::now();

        // Calculez la durée d'exécution en microsecondes (ou autre unité au choix)
        std::chrono::duration<double> duration_thread = std::chrono::duration_cast<std::chrono::duration<double>>(end_time_thread - start_time_thread);
        // Affichez le temps d'exécution
        std::cout << "Temps d'execution pour " << numThreads << " : " << duration_thread.count() << " secondes" << std::endl;


        for (int i = 0; i < numThreads; ++i) {
            std::cout << "Resultat du thread " << i << ": ";
            std::cout << "Conflits: " << results[i].countConflicts() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

