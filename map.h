#ifndef MAP_H
#define MAP_H

#include <vector>
#include "nutrient.h"
#include "organism.h"

class MapSquare;
typedef std::pair<MapSquare, NutrientAndAmount> Neighbor;

class MapSquare {
    std::vector<NutrientAndAmount> nutrients;
    Organism org;
    std::vector<Neighbor> neighbors; // list of pairs of neighboring MapSquares and their requested NutrientAndAmounts
public:
    MapSquare(std::vector<Nutrient> nuts);
    friend class Map;
};

class Map {
    std::vector<std::vector<MapSquare>> map;
    std::vector<std::shared_ptr<OrganismType>> allTypes;
    std::vector<Nutrient> allNutrients;
    // add public methods and constructor
};

#endif
