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
    double scoreCreationOnSquare(std::vector<NutrientAndAmount> &nutrients) const;
    friend std::istream &operator>>(std::istream &is, OrganismType &ot);
    friend std::ostream &operator<<(std::ostream &os, const OrganismType &ot);
    friend class Organism;
};

class Organism {
    std::shared_ptr<OrganismType> ot;
    Nutrient secondaryReqNutrient;
    bool isNull;
    int consumed; // amount consumed on current round
    void consume(Nutrient &nutrient, std::vector<NutrientAndAmount> &nutrients);
public:
    Organism(): isNull(true);
    Organism(std::shared_ptr<OrganismType> ot, Nutrient &secondaryReqNutrient): ot(ot), secondaryReqNutrient(secondaryReqNutrient),
                                                                                isNull(false), consumed(0) {};
    void consumeForCreation(std::vector<NutrientAndAmount> &nutrients);
    void consumePrimary(std::vector<NutrientAndAmount> &nutrients);
    void consumeSecondary(std::vector<NutrientAndAmount> &nutrients);
    bool survivedRound(std::vector<NutrientAndAmount> &nutrients);
    NutrientAndAmount drainStats() const;
    std::vector<NutrientAndAmount> &produce();
};

#endif