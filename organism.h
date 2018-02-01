#ifndef ORGANISM_H
#define ORGANISM_H

#include <vector>
#include <map>
#include <iostream>
#include "nutrient.h"
#include "organismType.h"

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
    std::vector<NutrientAndAmount> die();
    NutrientAndAmount drainStats() const;
    std::vector<NutrientAndAmount> produce(std::vector<Nutrient> &nutrients);

    bool isAlive() const {
        return !this->isNull;
    }

    friend std::ostream &operator<<(std::ostream &os, const Organism &o) {
        os << *o.ot;
        return os;
    };
};

#endif
