#ifndef ORGANISM_H
#define ORGANISM_H

#include "nutrient.h"

class MapSquare;

class OrganismType {
    std::string name;
    Nutrient primaryReqNutrient;
    Nutrient primaryProdNutrient;
    int createAmount;
    int drainAmount;
    int consumeAmount;
    int primaryProdAmount;
    int otherProdAmount;
public:
    OrganismType(){};
    double scoreCreationOnSquare(MapSquare &square);
    friend std::istream &operator>>(std::istream &is, OrganismType &ot);
    friend std::ostream &operator<<(std::ostream &os, const OrganismType &ot);
    friend Organism;
};

class Organism {
    std::shared_ptr<OrganismType> ot;
    Nutrient secondaryReqNutrient;
public:
    Organism(std::shared_ptr<OrganismType> ot, Nutrient &secondaryReqNutrient): ot(ot), secondaryReqNutrient(secondaryReqNutrient){};
};

#endif