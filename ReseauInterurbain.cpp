/**
 * \file ReseauInterurbain.cpp
 * \brief Implémentattion de la classe ReseauInterurbain.
 * \author Charles Dion-Buteau
 * \version 0.1
 * \date 24 mars 2024
 *
 *  Travail pratique numéro 2
 *
 */
#include <sstream>
#include <fstream>
#include "ReseauInterurbain.h"
#include <algorithm>
#include <stack>
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2 {
    ReseauInterurbain::ReseauInterurbain(std::string nomReseau, size_t nbVilles) : nomReseau(nomReseau),
                                                                                   unReseau(nbVilles) {};

    //a voir
    ReseauInterurbain::~ReseauInterurbain() = default;

    void ReseauInterurbain::resize(size_t nouvelleTaille) {

        unReseau.resize(nouvelleTaille);

    }

    // Méthode fournie
    void ReseauInterurbain::chargerReseau(std::ifstream &fichierEntree) {
        if (!fichierEntree.is_open())
            throw std::logic_error("ReseauInterurbain::chargerReseau: Le fichier n'est pas ouvert !");

        std::string buff;

        getline(fichierEntree, nomReseau);
        nomReseau.erase(0, 20); // Enlève: Reseau Interurbain:

        int nbVilles;

        fichierEntree >> nbVilles;
        getline(fichierEntree, buff); //villes

        unReseau.resize(nbVilles);

        getline(fichierEntree, buff); //Liste des villes

        size_t i = 0;

        getline(fichierEntree, buff); //Premiere ville

        while (buff != "Liste des trajets:") {
            unReseau.nommer(i, buff);
            getline(fichierEntree, buff);
            i++;
        }

        while (!fichierEntree.eof()) {
            getline(fichierEntree, buff);
            std::string source = buff;
            getline(fichierEntree, buff);
            std::string destination = buff;

            getline(fichierEntree, buff);
            std::istringstream iss(buff);

            float duree;
            iss >> duree;

            float cout;
            iss >> cout;

            unReseau.ajouterArc(unReseau.getNumeroSommet(source), unReseau.getNumeroSommet(destination), duree, cout);
            //unReseau.ajouterArc(2, 3, duree, cout);

        }
    }

    /**
    * \fn             rechercheCheminDijkstra(const std::string &source, const std::string &destination,
                                                      bool dureeCout) const
    * \brief          Permet de trouver le plus court chemin entre une source et une destination dans un reseau de ville
    * \param[in]      const std::string &source
    * \param[in]      const std::string &destination
    * \param[in]      bool dureeCout
   */
    Chemin ReseauInterurbain::rechercheCheminDijkstra(const std::string &source, const std::string &destination,
                                                      bool dureeCout) const {
        Chemin cheminTrouve;
        //Initialisation des attributs de l'objet cheminTrouve
        cheminTrouve.coutTotal = 0;
        cheminTrouve.dureeTotale = 0;
        cheminTrouve.reussi = false;

        size_t noeudSource = unReseau.getNumeroSommet(source); //Numero du noeud source
        size_t noeudDestionation = unReseau.getNumeroSommet(destination); //Numero du noeud destination

        std::vector<size_t> noeudNonSol; //Q Vecteur des noeuds non solutionnés, initialement de longueur nbSommets
        std::vector<size_t> noeudSol; //T Vecteur des noeuds solutionnés, initialement vide
        std::vector<float> ponderationsNoeud(unReseau.getNombreSommets(), 1e6);; //vecteur de pondération
        std::vector<size_t> predecesseurs(unReseau.getNombreSommets(), -1); //vecteur de prédecesseurs
        std::ostringstream osQ;
        ponderationsNoeud[noeudSource] = 0; //Pondération de source à source est initialisée à 0
        size_t noeudActuel = noeudSource; //noeud atcuellement analysé
        noeudSol.push_back(noeudActuel);

        for (size_t i = 0; i != unReseau.getNombreSommets(); ++i) {
            noeudNonSol.push_back(i);
            //osQ << i << ", ";
        }

        //std::cout << osQ.str();

        //Je veux itérer sur les arc du noeud actuel et rouver le plus petit poids
        //Pour topus les noeuds dans mon graphes
        while (!noeudNonSol.empty()) {
            //for (int noeuds = 0; noeuds <= unReseau.getNombreSommets(); ++noeuds) {
            //Je cherche le prochain noeud a solutionner
            //itérer sur les arcs puis choisir celui avec le poids le plus faible
            //Exploration du noeud noeud actuel
            for (auto arcDest: unReseau.listerSommetsAdjacents(noeudActuel)) {
                //On choisie avec quel type de pondération on travaille
                float poidsTrajet = getPoids(noeudActuel, arcDest, dureeCout);
                //Si la pondération du noeud de destionation est plus grande que celle du noeudActuel + poidsTrajet
                if (ponderationsNoeud[arcDest] > ponderationsNoeud[noeudActuel] + poidsTrajet) {
                    ponderationsNoeud[arcDest] = ponderationsNoeud[noeudActuel] + poidsTrajet;
                    predecesseurs[arcDest] = noeudActuel;
                }
            }//fin exploration

            //On ajoute le noeud tout juste solutionné à la liste des noeuds solutionnés
            noeudSol.push_back(noeudActuel);
            //On enleve le noeud solutionné au vector de noeud non solutionnés
            for (auto itNonsol = noeudNonSol.begin(); itNonsol != noeudNonSol.end(); ++itNonsol) {
                if (*itNonsol == noeudActuel) {
                    noeudNonSol.erase(itNonsol);
                    break;
                }
            }

            //Je regarde dans tous mes noeuds non solutionné, celui qui a les poids le plus petit
            float poidsAbattre = 1e6;

            for (auto itNonsol = noeudNonSol.begin(); itNonsol != noeudNonSol.end(); ++itNonsol) {

                //on verifie la ponderation de chaque noeud non solutionnés
                //on veut trouver la plus basse pour ensuite solutionner le noeud en question
                if (ponderationsNoeud[*itNonsol] < poidsAbattre) {
                    poidsAbattre = ponderationsNoeud[*itNonsol];
                    noeudActuel = *itNonsol;

                }
            }

            //on verifie ensuite si on est arrivé a destination

            if (noeudActuel == noeudDestionation) {
                //On est arrivé c'est le temps de faire notre trajet inverse dans un nouveau vecteur
                std::vector<size_t> cheminInverse;
                cheminInverse.push_back(noeudDestionation);
                while (noeudActuel != noeudSource) {
                    cheminInverse.push_back(predecesseurs[noeudActuel]);
                    noeudActuel = predecesseurs[noeudActuel];
                }

                for (int i = static_cast<int>(cheminInverse.size()) - 1; i >= 0; i--) {
                    //On ajoute d'abord la ville actuelle au chemin
                    cheminTrouve.listeVilles.push_back(unReseau.getNomSommet(cheminInverse[i]));
                    //On ajoute le cout au cout total
                    if (i > 0) {
                        cheminTrouve.coutTotal += getPoids(cheminInverse[i], cheminInverse[i - 1], false);
                        //On ajoute la duree à la duree total
                        cheminTrouve.dureeTotale += getPoids(cheminInverse[i], cheminInverse[i - 1], true);
                    }
                }
                cheminTrouve.reussi = true;
                return cheminTrouve;
            }

        }
        return cheminTrouve;

    }

    /**
   * \fn             algorithmeKosaraju()
   * \brief          Permet de trouver les composantes fortements connexes du reseau actuel
  */
    std::vector<std::vector<std::string> > ReseauInterurbain::algorithmeKosaraju() {

        //On commance par se créer un graphe inverse de notre reseau
        Graphe grapheInverse = inverserGraphe(unReseau);

        //On fait ensuite le parcours en profondeur de ce graphe inverse en stockant les noeuds abandonnés dans une pile
        std::stack<size_t> pileNoeudsAbandonnes = explorerDFS(grapheInverse);

        //Création du vecteur des composantes fortement connexes
        std::vector<std::vector<std::string>> composantes;

        //Noeud visités
        std::vector<size_t> noeudsVisites;

        std::unordered_set<size_t> sommetsParcourus;


        while (!pileNoeudsAbandonnes.empty()) {
            if (std::find(noeudsVisites.begin(), noeudsVisites.end(), pileNoeudsAbandonnes.top()) ==
                noeudsVisites.end()) {
                ResultDFS InfoDFS(unReseau);

                InfoDFS.sommetsParcourus = sommetsParcourus;

                auxExplorerDFS(InfoDFS, pileNoeudsAbandonnes.top());

                std::vector<std::string> composante;
                std::stack<size_t> pilePourIteration = InfoDFS.pileAbandonnes;
                std::vector<size_t> composanteFortCon;
                size_t premierNoeud;
                for (size_t i = 0; i < pilePourIteration.size(); ++i) {
                    composanteFortCon.push_back(InfoDFS.pileAbandonnes.top());
                    noeudsVisites.push_back(InfoDFS.pileAbandonnes.top());
                    InfoDFS.pileAbandonnes.pop();

                }
                std::sort(composanteFortCon.begin(), composanteFortCon.end());
                for (size_t i = 0; i < pilePourIteration.size(); ++i) {
                    composante.push_back(unReseau.getNomSommet(composanteFortCon[i]));
                }
                composantes.push_back(composante);
                pileNoeudsAbandonnes.pop();
                sommetsParcourus = InfoDFS.sommetsParcourus;

            } else pileNoeudsAbandonnes.pop();
        }
        return composantes;
    }

    float ReseauInterurbain::getPoids(const size_t &source, const size_t &destination, bool dureeCout) const {

        if (dureeCout) return unReseau.getPonderationsArc(source, destination).duree;
        return unReseau.getPonderationsArc(source, destination).cout;
    }

    /**
  * \fn             inverserGraphe(const Graphe &graphe_a_inverser)
  * \brief          Permet l'inversion d'un graphe
  * \param[in]      const Graphe &graphe_a_inverser
 */
    Graphe ReseauInterurbain::inverserGraphe(const Graphe &graphe_a_inverser) {

        //il faut changer les sources pour les destinations et les distinations pour les sources

        size_t nbSommets = graphe_a_inverser.getNombreSommets();

        //creation du nouveau graphe
        Graphe grapheInverse(nbSommets);

        //on itere sur chaque sommet
        for (size_t src = 0; src < nbSommets; ++src) {
            for (size_t dest: unReseau.listerSommetsAdjacents(src)) {
                //On cree une arc en inversance source et dest, on ne se préocupe pas des poids
                grapheInverse.ajouterArc(dest, src, getPoids(src, dest, true), getPoids(src, dest, false));
            }
        }

        return grapheInverse;
    }

    /**
   * \fn             explorerDFS(const Graphe &graphe_a_explorer)
   * \brief          Méthode pour l'exploration en profondeur d'un graphe
   * \param[in]      const Graphe &graphe_a_explorer
   */
    std::stack<size_t> ReseauInterurbain::explorerDFS(const Graphe &graphe_a_explorer) {
        ResultDFS InfoDFS(graphe_a_explorer);

        //On boucle sur tous les sommets du graphe
        for (auto i = 0; i < graphe_a_explorer.getNombreSommets(); ++i) {
            //Si on a pas encore visiter le sommet départ, ...
            if (InfoDFS.sommetsParcourus.find(i) == InfoDFS.sommetsParcourus.end()) {
                auxExplorerDFS(InfoDFS, i);
            }
        }
        return InfoDFS.pileAbandonnes;

    }

    /**
   * \fn             auxExplorerDFS(ResultDFS &InfoDFS, const size_t &depart)
   * \brief          Méthode auxilière récursive pour l'exploration en profondeur d'un graphe
   * \param[in]      ResultDFS &InfoDFS
   * \param[in]      const size_t &depart
   */
    void ReseauInterurbain::auxExplorerDFS(ResultDFS &InfoDFS, const size_t &depart) {

        //Si le sommet de départ est déjà explorer, on ne fait la le DFS et l'exploration est terminée
        if (InfoDFS.sommetsParcourus.find(depart) != InfoDFS.sommetsParcourus.end()) return;

        //On ajoute le sommet qu'on va visiter à notre set de sommet parcourus
        InfoDFS.sommetsParcourus.insert(depart);
        for (auto voisin: InfoDFS.grapheVisite.listerSommetsAdjacents(depart)) {
            auxExplorerDFS(InfoDFS, voisin);
        }
        //On abandonne le noeud départ en l'empilant
        InfoDFS.pileAbandonnes.push(depart);

    }

    //À compléter au besoin par d'autres méthodes

}//Fin du namespace
