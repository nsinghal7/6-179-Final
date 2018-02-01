#ifndef CONTAINER_MAP_H
#define CONTAINER_MAP_H

#include <iostream>
#include <vector>
#include "nutrient.h"
#include "organismType.h"
#include "mapSquare.h"

class Map {
    std::vector<std::vector<std::shared_ptr<MapSquare>>> map;
    std::vector<std::shared_ptr<OrganismType>> allTypes;
    std::vector<Nutrient> allNutrients;
    
public:
    Map(std::vector<Nutrient> &an, std::vector<std::shared_ptr<OrganismType>> at): allNutrients(an), allTypes(at) {};
    void readIn(std::istream &is, std::ostream &os);
    void readOutSquare(std::ostream &os, int row, int col);
    void readMapForNutrient(std::ostream &os, Nutrient nut);
    friend std::ostream &operator<<(std::ostream &os, const Map &m);

    void runRound();
};

#endif
