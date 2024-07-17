//
// Created by charl on 2024-03-16.
//
#include "gtest/gtest.h"
#include "Graphe.h"
#include "ReseauInterurbain.h"
#include <string>

using namespace TP2;


TEST(ReseauInterurbain, constructeur) {
    EXPECT_NO_THROW(ReseauInterurbain reseau("Resion mtl",5));
}

class ReseauTest : public ::testing::Test {
public:

    ReseauTest(): r1("Grand Montreal", 10), r2("Grand Montreal", 10) {}

protected:

    ReseauInterurbain r1;
    ReseauInterurbain r2;

    void SetUp() override {


    }

};

TEST_F(ReseauTest,nouvelleTaille)
{
    r1.resize(11);
}

TEST_F(ReseauTest, rechercheCheminDijkstra)
{
     r1.rechercheCheminDijkstra("","",true);
}