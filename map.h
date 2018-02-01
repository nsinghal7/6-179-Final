#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <map>
#include "nutrient.h"
#include "organism.h"

class MapSquare;
typedef std::shared_ptr<MapSquare> Neighbor;

class MapSquare {
    std::map<Nutrient, int> nutrients;
    Organism org;
    std::map<Neighbor, NutrientAndAmount> neighbors; // list of pairs of neighboring MapSquares and their requested NutrientAndAmount
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

    friend std::istream &operator>>(std::istream &is, MapSquare &m);
    friend std::ostream &operator<<(std::ostream &os, MapSquare &m);
    friend class Map;
};

class Map {
    std::vector<std::vector<MapSquare>> map;
    std::vector<std::shared_ptr<OrganismType>> allTypes;
    std::vector<Nutrient> allNutrients;
    
public:
    Map(std::vector<Nutrient> &an): allNutrients(an) {};
    friend std::istream &operator>>(std::istream &is, Map &m);
    friend std::ostream &operator<<(std::ostream &os, const Map &m);

    void runRound();
};

#endif
