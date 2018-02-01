#ifndef ORGANISMTYPE_H
#define ORGANISMTYPE_H

#include <map>
#include <algorithm>
#include <vector>
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
    void readIn(std::istream &is, std::ostream &os, std::vector<Nutrient> &allNutrients);
    friend std::ostream &operator<<(std::ostream &os, const OrganismType &ot);
    friend class Organism;
};

#endif
