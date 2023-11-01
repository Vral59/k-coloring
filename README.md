# Projet de Résolution du Problème de Coloration de Graphes

Ce projet implémente une résolution du problème de coloration de graphes en utilisant un algorithme glouton. L'objectif est d'attribuer des couleurs aux nœuds d'un graphe de manière à minimiser les conflits, c'est-à-dire à minimiser le nombre de nœuds voisins ayant la même couleur.

## Fonctionnalités

- Lecture d'un graphe au format DIMACS depuis un fichier.
- Coloration du graphe en utilisant un algorithme glouton.
- Affichage du graphe coloré.
- Calcul du nombre de conflits dans le graphe coloré.

## Structure du Projet

Le projet est composé des fichiers suivants :

- `Graph.h` : Définition de la classe `Graph` représentant un graphe avec des nœuds et des arêtes.
- `Node.h` : Définition de la classe `Node` représentant un nœud dans un graphe.
- `main.cpp` : Le programme principal qui lit le graphe depuis un fichier, effectue la coloration, et affiche les résultats.
- D'autres fichiers d'en-tête et de code peuvent être inclus en fonction des besoins.

## Compilation

Pour compiler le projet, assurez-vous d'avoir un compilateur C++ prenant en charge la version C++11 ou supérieure. Vous pouvez utiliser les commandes suivantes pour compiler le programme :

```sh
g++ -std=c++11 -o nom_du_programme main.cpp src/Node.cpp src/Graph.cpp
```

## Compilation avec CMake

Vous pouvez utiliser CMake pour configurer et construire le projet. Assurez-vous d'avoir CMake installé sur votre système.

1. Créez un dossier de construction (par exemple, `build`) à la racine du projet :
```bash
mkdir build
cd build
```


2. Exécutez CMake pour générer les fichiers de construction :
```bash
cmake ..
```


3. Compilez le projet en exécutant `make` :
```
make
```

## Utilisation

Pour exécuter le programme, utilisez la commande suivante :
```bash
./nom_du_programme --file nom_du_fichier_dimacs --k nombre_de_couleur [--thread nombre_de_threads]
```

- `nom_du_programme` est le nom de l'exécutable généré après compilation.
- `nom_du_fichier_dimacs` est le fichier contenant les données du graphe au format DIMACS.
- `nombre_de_couleur` est la valeur de k, le nombre de couleurs maximum qui peut être utilisé.
- `nombre_de_threads` (optionnel) est le nombre de threads à utiliser. Si non spécifié, le programme utilisera le nombre maximum de threads disponibles.

Exemple sans spécifier le nombre de threads :
```bash
./coloration_graphes --file example.graph --k 5
```

Exemple en spécifiant le nombre de threads :
```bash
./coloration_graphes --file example.graph --k 5 --thread 4
```

## Format du Fichier DIMACS

Le programme prend en entrée un graphe au format DIMACS. Le fichier doit respecter la syntaxe suivante :

```
c <comment>
p edge <numNodes> <numEdges>
e <nodeID> <nodeID>
e <nodeID> <nodeID>
...
```


- `<numNodes>` : Le nombre de nœuds dans le graphe.
- `<numEdges>` : Le nombre d'arêtes dans le graphe.
- `<nodeID>` : L'ID du nœud.
- Les lignes commençant par "e" définissent les arêtes du graphe.

Exemple :

```
c Commentaire 
c Inutile
p edge 6 7
e 1 2
e 1 3
e 2 3
e 2 4
e 3 4
e 4 5
e 5 6
```
