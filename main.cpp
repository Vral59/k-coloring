#include <algorithm>
#include "include/Node.h"
#include "include/Graph.h"

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
                neighborColorCount[neighborColor]++;
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

int main() {
    // Creez un graphe avec 6 noeuds
    Graph graph(6);

    // Ajoutez des arêtes au graphe (dans cet exemple, un graphe non orienté)
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);
    graph.addEdge(5, 0);

    // Colorier le graphe
    greedyColoring(graph, 3);

    // Affichez le graphe
    graph.displayGraph();

    return 0;
}
