#ifndef ORGANISM_H
#define ORGANISM_H

#include <vector>
#include <map>
#include <iostream>
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
    double scoreCreationOnSquare(std::map<Nutrient, int> &nutrients) const;
    Nutrient primary() const {
        return this->primaryReqNutrient;
    }
    friend std::istream &operator>>(std::istream &is, OrganismType &ot);
    friend std::ostream &operator<<(std::ostream &os, const OrganismType &ot);
    friend class Organism;
};

class Organism {
    std::shared_ptr<OrganismType> ot;
    Nutrient secondaryReqNutrient;
    bool isNull;
    int consumed; // amount consumed on current round
    void consume(Nutrient &nutrient, std::map<Nutrient, int> &nutrients);
public:
    Organism(): isNull(true) {};
    void create(std::shared_ptr<OrganismType> ot, Nutrient &secondaryReqNutrient);
    void consumeForCreation(std::map<Nutrient, int> &nutrients);
    void consumePrimary(std::map<Nutrient, int> &nutrients);
    void consumeSecondary(std::map<Nutrient, int> &nutrients);
    bool survivedRound();
    std::vector<NutrientAndAmount> &die();
    NutrientAndAmount drainStats() const;
    std::vector<NutrientAndAmount> &produce(std::vector<Nutrient> &nutrients);

    bool isAlive() const {
        return !this->isNull;
    }

    friend std::ostream &operator<<(std::ostream &os, const Organism &o) {
        return os << o.ot;
    };
};

#endif
