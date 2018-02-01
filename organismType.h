#ifndef ORGANISMTYPE_H
#define ORGANISMTYPE_H

#include <map>
#include <iostream>
#include "nutrient.h"

class Organism; // forward declare so it can be a friend

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
    double scoreCreationOnSquare(std::map<Nutrient, int> &nutrients) const;
    Nutrient primary() const {
        return this->primaryReqNutrient;
    }
    friend std::istream &operator>>(std::istream &is, OrganismType &ot);
    friend std::ostream &operator<<(std::ostream &os, const OrganismType &ot);
    friend class Organism;
};

#endif