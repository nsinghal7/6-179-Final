#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <map>
#include "nutrient.h"
#include "organism.h"

class MapSquare;

class MapSquare {
    std::map<Nutrient, int> nutrients;
    Organism org;
    std::map<std::shared_ptr<MapSquare>, NutrientAndAmount> neighbors;
    void addNutrients(std::vector<NutrientAndAmount> &add);
public:
    MapSquare(std::vector<Nutrient> nuts);

    // called every round
    void checkCreateLife(std::vector<std::shared_ptr<OrganismType>> &types);
    void consumePrimary();

    // drain sequence
    void requestDrain();
    void divideRequests();
    void drain();

    // finish round
    void consumeSecondary();
    void checkDead();
    void produce(std::vector<Nutrient> &allNutrients);
    void readIn(std::istream &is, std::ostream &os);
    void readOut(std::ostream &os, bool readOrg);


    friend class Map;
};

class Map {
    std::vector<std::vector<std::shared_ptr<MapSquare>>> map;
    std::vector<std::shared_ptr<OrganismType>> allTypes;
    std::vector<Nutrient> allNutrients;
    
public:
    Map(std::vector<Nutrient> &an, std::vector<std::shared_ptr<OrganismType>> at): allNutrients(an), allTypes(at) {};
    void readIn(std::istream &is, std::ostream &os);
    friend std::ostream &operator<<(std::ostream &os, const Map &m);

    void runRound();
};

#endif
