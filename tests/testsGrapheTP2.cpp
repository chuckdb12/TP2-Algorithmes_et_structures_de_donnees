//
// Created by charl on 2024-03-06.
//


#include "gtest/gtest.h"
#include "Graphe.h"
#include "ReseauInterurbain.h"
#include <string>

using namespace TP2;


TEST(Graphe, constructeur) {
    EXPECT_NO_THROW(Graphe map(5));
    EXPECT_THROW(Graphe map(0), std::logic_error);
}

class GrapheTest : public ::testing::Test {
public:

    GrapheTest(): g1(), g2() {}

protected:

    Graphe g1;
    Graphe g2;
};

TEST_F(GrapheTest, resize)
{
    EXPECT_EQ(g1.getNombreSommets(),10);
    g1.resize(14);
    EXPECT_EQ(g1.getNombreSommets(),14);
}



TEST_F(GrapheTest, nom)
{

    std::string name = "Quebec";
    g1.nommer(9,name);
    std::cout << (g1.getNomSommet(9));
    EXPECT_EQ(g1.getNomSommet(9),name);
    EXPECT_THROW(g1.nommer(11,"Cape town"),std::logic_error);
}

TEST_F(GrapheTest, numeroDeLaVille)
{
    g1.resize(3);
    std::string nom = "Montreal";
    g1.nommer(2,nom);
    EXPECT_EQ(g1.getNumeroSommet("Montreal"),2);
}

TEST_F(GrapheTest, ajouterArc)
{
    g1.nommer(0, "Montreal");
    g1.nommer(1, "Quebec");
    g1.nommer(2, "Trois rivieres");
    g1.ajouterArc(g1.getNumeroSommet("Montreal"),g1.getNumeroSommet("Quebec"),2.00001,45);
    g1.ajouterArc(0,2,2,45);
    g1.ajouterArc(1,2,2,45);
    g1.ajouterArc(2,0,2,45);
    //std::cout << g1.to_string() << std::endl;
    std::cout << g1 << std::endl;
    EXPECT_TRUE(g1.arcExiste(1,2));
    EXPECT_EQ(g1.getNombreArcs(),4);
}

TEST_F(GrapheTest, getNumeroSommet)
{
    g1.nommer(1, "Montreal");
    g1.nommer(2, "Quebec");
    g1.nommer(3, "Trois rivieres");
    g1.ajouterArc(1,2,2,45);
    g1.ajouterArc(1,3,2,45);
    EXPECT_EQ(g1.getNumeroSommet("Trois rivieres"), 3);
    EXPECT_THROW(g1.getNumeroSommet("Sainte Foy"), std::logic_error);
}

TEST_F(GrapheTest, enleverArc)
{
    g1.nommer(1, "Montreal");
    g1.nommer(2, "Quebec");
    g1.nommer(3, "Trois rivieres");
    g1.ajouterArc(1,2,2,45);
    EXPECT_TRUE(g1.arcExiste(1,2));

    g1.enleverArc(1,2);
    EXPECT_FALSE(g1.arcExiste(1,2));
    g2.nommer(1, "Chicago");
    g2.nommer(2,"New-York");
    g2.ajouterArc(1,2,2,45);
    std::cout << g2 << std::endl;
    EXPECT_TRUE(g2.arcExiste(1,2));
    EXPECT_EQ(g2.getNombreArcs(),1);
    g2.enleverArc(1,2);
    std::cout << g2 << std::endl;
    EXPECT_EQ(g2.getNombreArcs(),0);
    EXPECT_FALSE(g2.arcExiste(1,2));
}

TEST_F(GrapheTest, listerSommets)
{
    g1.nommer(1, "Montreal");
    g1.nommer(2, "Quebec");
    g1.nommer(3, "Trois rivieres");
    g1.ajouterArc(3,1,2,45);
    g1.ajouterArc(3,2,2,45);
    std::vector<size_t > voisins {1,2};
    EXPECT_EQ(g1.listerSommetsAdjacents(3),voisins);
}

TEST_F(GrapheTest, sommetExiste)
{
    g1.nommer(1, "Montreal");
    g1.nommer(2, "Quebec");
    g1.nommer(3, "Trois rivieres");
    EXPECT_NO_THROW(g1.sommetExiste("Quebec"));
}

TEST_F(GrapheTest, getPonderation)
{
    g1.nommer(1, "Montreal");
    g1.nommer(2, "Quebec");
    g1.nommer(3, "Trois rivieres");
    g1.ajouterArc(3,2,2,45);
    Ponderations poids;
    poids.duree = 2;
    poids.cout = 45;
    EXPECT_EQ(g1.getPonderationsArc(3,2).cout, poids.cout);
    EXPECT_EQ(g1.getPonderationsArc(3,2).duree, poids.duree);
    EXPECT_FALSE(g1.getPonderationsArc(3,2).cout == 50);

}