#ifndef ORGANISM_H
#define ORGANISM_H

#include <vector>
#include "nutrient.h"

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
    double scoreCreationOnSquare(std::vector<NutrientAndAmount> nutrients);
    friend std::istream &operator>>(std::istream &is, OrganismType &ot);
    friend std::ostream &operator<<(std::ostream &os, const OrganismType &ot);
    friend class Organism;
};

class Organism {
    std::shared_ptr<OrganismType> ot;
    Nutrient secondaryReqNutrient;
public:
    Organism(); // TODO, must clearly be null
    Organism(std::shared_ptr<OrganismType> ot, Nutrient &secondaryReqNutrient): ot(ot), secondaryReqNutrient(secondaryReqNutrient){};
};

#endif