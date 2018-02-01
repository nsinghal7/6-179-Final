#ifndef MAPSQUARE_H
#define MAPSQUARE_H

#include <iostream>
#include <vector>
#include <map>
#include "nutrient.h"
#include "organismType.h"
#include "organism.h"

class MapSquare {
    std::map<Nutrient, int> nutrients;
    Organism org;
    std::map<std::shared_ptr<MapSquare>, NutrientAndAmount> neighbors;
public:
    MapSquare(std::vector<Nutrient> &nuts);

    // called every round
    void checkCreateLife(std::vector<std::shared_ptr<OrganismType>> &types);
    void consumePrimary();

    // drain sequence
    void requestDrain(std::shared_ptr<MapSquare> self);
    void divideRequests();
    void drain(std::shared_ptr<MapSquare> self);

    // finish round
    void consumeSecondary();
    void checkDead();
    void produce(std::vector<Nutrient> &allNutrients);
    void readIn(std::istream &is, std::ostream &os);
    void readOut(std::ostream &os, bool justReadOrg);


    friend class Map;
};

#endif
