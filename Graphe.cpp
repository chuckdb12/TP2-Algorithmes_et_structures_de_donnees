/**
 * \file Graphe.cpp
 * \brief Implémentation d'un graphe orienté.
 * \author Charles Dion-Buteau
 * \version 0.1
 * \date mars 2024
 *
 *  Travail pratique numéro 2
 *
 */

#include "Graphe.h"
#include <algorithm>
#include <sstream>
#include <cassert>
#include <list>
#include <unordered_set>
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{
	//Ajoutez l'implémentation de vos méthodes ici.
    /**
    * \fn             Graphe(size_t nbSommets)
    * \brief          Constructeur de la classe Graphe
    * \param[in]      size_t nbSommets
    * \pre
    * \post
    * \exception      std::logic_error si le nombre de sommet est plus petit que 1
   */
    Graphe::Graphe(size_t p_nbSommets):nbSommets(p_nbSommets),listesAdj(),nbArcs(0)
    {
        if(p_nbSommets<1) throw std::logic_error("Le nombre de sommets ne peut pas être inf à 1");
        resize(p_nbSommets);
        assert(invariant());
    }

    /**
    * \fn             Graphe(size_t nbSommets)
    * \brief          Destructeur de la classe Graphe
    * \param[in]      size_t nbSommets
    * \pre
    * \post
    * \exception
   */
    Graphe::~Graphe() = default; //À confirmer!!

    /**
    * \fn             resize(size_t nouvelleTaille)
    * \brief          Change la taille du graphe en utilisant un nombre de sommet = nouvelleTaille
    * \param[in]      size_t nouvelleTaille
    * \pre
    * \post
    * \exception      std::logic_error si la nouvelle taille est plus petite que 1
   */
    void Graphe::resize(size_t nouvelleTaille)
    {
        nbSommets = nouvelleTaille;
        noms.resize(nouvelleTaille);
        listesAdj.resize(nouvelleTaille);

        assert(invariant());

    }

    /**
    * \fn             nommer(size_t sommet, const std::string& nom)
    * \brief          nomme le sommet specifie avec le nom specifie
    * \param[in]      size_t nouvelleTaille
    * \exception      std::logic_error si le sommet n'existe pas
   */
    void Graphe::nommer(size_t sommet, const std::string& nom)
    {
        if(sommet > nbSommets) throw std::logic_error("Le sommet n'existe pas");
        noms[sommet] = nom;
        assert(invariant());
    }

    /**
    * \fn             ajouterArc(size_t source, size_t destination, float duree, float cout)
    * \brief          Ajoute un arc au graphe
    * \param[in]      size_t source
    * \param[in]      size_t destination
    * \param[in]      float duree
    * \param[in]      float cout
    * \pre
    * \post
    * \exception      std::logic_error si la source et/ou la destination n'existe pas
    * \exception      std::logic_error si l'arc existe deja dans le graphe
   */
    void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout)
    {

        if(source > nbSommets || destination > nbSommets) throw std::logic_error("La source ou la destionation n'existe pas");

        if(arcExiste(source,destination)) throw std::logic_error("L'arc existe deja");

        Ponderations nouv_poids{};
        nouv_poids.duree = duree;
        nouv_poids.cout = cout;

        //On ajoute un objet Arc (créé dasn le emplace_back) à l'endroit de la source
        //source-1 pour transformer le num du sommet pour l'indice de celui-ci
        listesAdj.at(source).emplace_back(destination,nouv_poids);

        ++nbArcs;

        assert(invariant());


    }
    /**
    * \fn             enleverrArc(size_t source, size_t destination, float duree, float cout)
    * \brief          enleve un arc au graphe
    * \param[in]      size_t source
    * \param[in]      size_t destination
    * \pre
    * \post
    * \exception      std::logic_error si la source et/ou la destination n'existe pas
    * \exception      std::logic_error si l'arc n'existe dans le graphe
   */
    void Graphe::enleverArc(size_t source, size_t destination){

        if(source > nbSommets || destination > nbSommets) throw std::logic_error("La source ou la destionation n'existe pas");

        if(!arcExiste(source,destination)) throw std::logic_error("L'arc n'existe pas");

        //On itere sur les arc de la source specifiee, puis on cherche s'il y a un arc avec la
        //destination specifiee
        for(auto it = (listesAdj.at(source)).begin(); it != (listesAdj.at(source)).end(); ++it)
        {
            if(it->destination == destination)
            {
                //Quand on trouve la destionnation on efface l'arc
                (listesAdj.at(source)).erase(it);
            }
        }

        nbArcs--;


    }

    /**
    * \fn             arcExiste(size_t source, size_t destination) const
    * \brief          verifie si l'arc en question existe
    * \param[in]      size_t source
    * \param[in]      size_t destination
    * \pre
    * \post
    * \exception      std::logic_error si la source et/ou la destination n'existe pas
   */
    bool Graphe::arcExiste(size_t source, size_t destination) const{

        if(source > nbSommets || destination > nbSommets) throw std::logic_error("La source ou la destionation n'existe pas");

        for(auto dest: listesAdj.at(source))
        {
            if(dest.destination == destination) return true;
        }

        return false;
    }
    /**
    * \fn             listerSommetsAdjacents(size_t sommet) const
    * \brief          permet de retourner un vector contenant les sommets adjacents au sommet specife
    * \param[in]      size_t source
    * \pre
    * \post
    * \exception      std::logic_error si le sommet n'existe pas
   */
    std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const
    {

        if(sommet > nbSommets) throw std::logic_error("Le sommet n'existe pas");

        std::vector<size_t> listeSommets;

        for(auto it = (listesAdj.at(sommet).begin()); it != (listesAdj.at(sommet).end()); ++it)
        {
            //On ajoute le numero de l'indice+1 = numero du sommet dans la liste
            listeSommets.push_back(it->destination);
        }

        return listeSommets;
    }

    /**
    * \fn             getNomSommet(size_t sommet) const
    * \brief          Permet de trouver le nom du sommet à l'aide de son numero
    * \param[in]      const std::string & sommet
    * \pre
    * \post
    * \exception      std::logic_error si le sommet n'existe pas
   */
    std::string Graphe::getNomSommet(size_t sommet) const
    {
        if(sommet > nbSommets) throw std::logic_error("Le sommet n'existe pas");
        return noms[sommet];
    }

    /**
    * \fn             getNumeroSommet(const std::string & nom) const
    * \brief          Permet de trouver le numéro associé au nom de la ville
    * \param[in]      const std::string & nom
    * \pre
    * \post
    * \exception      std::logic_eror si le nom n'existe pas
   */
    size_t Graphe::getNumeroSommet(const std::string& nom) const
    {

        if(!sommetExiste(nom)) throw std::logic_error("Le nom n'existe pas");

        auto it = std::find(noms.begin(),noms.end(),nom);
        //ici on fait +1 pour recevoir le numéro du sommet et non son indice dans le vecteur
        return static_cast<size_t>(it - noms.begin());
    }

    /**
    * \fn             getNombreSommets() const
    * \brief          retourne le nombre de sommets dans le graphe
    * \pre
    * \post
    * \exception
   */
    int Graphe::getNombreSommets() const
    {
        return static_cast<int>(nbSommets);
    }

    /**
    * \fn             getNombreArcs() const
    * \brief          retourne le nombre d'arc dans le graphe
    * \pre
    * \post
    * \exception
   */
    int Graphe::getNombreArcs() const
    {
        return static_cast<int>(nbArcs);
    }


    /**
    * \fn             getPonderationsArc(size_t source, size_t destination) const
    * \brief          retourne la ponderation de l'arc specifie
    * \param[in]      size_t source
    * \param[in]      size_t destination
    * \pre
    * \post
    * \exception      std::logic_error si la source et/ou la destination n'existe pas
    * \exception      std::logic_error si l'arc n'existe dans le graphe
   */
    Ponderations Graphe::getPonderationsArc(size_t source, size_t destination) const
    {

        if(source > nbSommets || destination > nbSommets) throw std::logic_error("La source ou la destionation n'existe pas");

        if(!arcExiste(source,destination)) throw std::logic_error("L'arc n'existe pas");

        Ponderations poids;

        for(auto it : listesAdj.at(source))
        {
            if(it.destination == destination)
            {
                poids.cout = it.poids.cout;
                poids.duree = it.poids.duree;
                break;
            }
        }
        return poids;
    }

    /**
    * \fn             sommetExiste(std::string const & ville) const
    * \brief          Vérifie si le sommet existe
    * \param[in]      size_t ville
    * \pre
    * \post
    * \exception
   */
    bool Graphe::sommetExiste(std::string const & ville) const
    {
        auto it = std::find(noms.begin(),noms.end(), ville);
        if(it == noms.end())
        {
            return false;
        }
        else
        {
            return true;
        }

    }

    /**
   * \fn             invariant() const
   * \brief          On vérifie que les invariants sont respectés. À utiliser dans les méthodes privée
  */
    bool Graphe::invariant() const {

        if(noms.size() != listesAdj.size())
        {

            std::cout << "1" << std::endl;
            return false;
        }

        if(noms.size() != nbSommets)
        {
            return false;
        }

        size_t nombreArc = 0;
        for(const auto & itListe : listesAdj)
        {
            for(auto itArc: itListe) {
                nombreArc++;
            }
        }

        if(nombreArc != nbArcs) return false;

        return true;
    }




}//Fin du namespace
