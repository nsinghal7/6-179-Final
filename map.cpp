#include "map.h"

MapSquare::MapSquare(std::vector<Nutrient> nuts): nutrients(nuts.size()) {
    int i = 0;
    for(Nutrient n : nuts) {
        this->nutrients[i] = make_pair(n, 0)
        i++;
    }
}


void MapSquare::checkCreateLife(std::vector<std::shared_ptr<OrganismType>> &types) {
    if(this->org.isAlive()) {
        return; // can't have two living things in one place
    }
    std::shared_ptr<OrganismType> bestType;
    double bestScore = -1;

    for(auto ot : types) {
        double score = ot->scoreCreationOnSquare(this->nutrients);
        if(score > bestScore) {
            bestType = ot;
            bestScore = score;
        }
    }

    if(bestScore > 0) {
        // Actually possible to create life
        Nutrient primary = ot->primary();
        Nutrient secondary;
        int largestAmount = -1;
        for(NutrientAndAmount n : this->nutrients) {
            if(n->second > largestAmount && n->first != primary) {
                secondary = n->first;
                largestAmount = n->second;
            }
        }

        this->org = new Organism(ot, secondary);
    }
}

void MapSquare::consumePrimary() {
    this->org.consumePrimary(this->nutrients);
}

void MapSquare::requestDrain() {
    NutrientAndAmount stats = this->org.drainStats();

    for(Neighbor neighbor : this->neighbors) {
        // for each neighbor, set their stored value for me to this stats value
        for(Neighbor n : neighbor->first->neighbors) {
            if(n->first.get() == this) {
                n->second = stats;
                break;
            }
        }
    }
}




