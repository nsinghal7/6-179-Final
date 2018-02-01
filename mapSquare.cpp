#include "mapSquare.h"

MapSquare::MapSquare(std::vector<Nutrient> nuts) {
    for(Nutrient n : nuts) {
        this->nutrients[n] = 0;
    }
}


void MapSquare::checkCreateLife(std::vector<std::shared_ptr<OrganismType>> &types) {
    if(this->org.isAlive()) { // only create life if doesn't already have an organism
        return;
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

        this->org.create(ot, secondary);
    }
}

void MapSquare::consumePrimary() {
    if(this->org.isAlive()) { // only consume if has an organism
        this->org.consumePrimary(this->nutrients);
    }
}

void MapSquare::requestDrain() {
    if(!this->org.isAlive()) { // only drain if has an organism
        return;
    }
    NutrientAndAmount stats = this->org.drainStats();

    for(std::pair<std::shared_ptr<MapSquare>, NutrientAndAmount> neighbor : this->neighbors) {
        // for each neighbor, set their stored value for me to this stats value
        neighbor->first->neighbors[this] = stats;
    }
}


void MapSquare::divideRequests() {
    // all squares divide requests
    std::map<Nutrient, int> totals;
    for(std::pair<std::shared_ptr<MapSquare>, NutrientAndAmount> neighbor : this->neighbors) {
        NutrientAndAmount &naa = neighbor->second;
        totals[naa->first] += naa->second;
    }
    for(std::pair<std::shared_ptr<MapSquare>, NutrientAndAmount> neighbor : this->neighbors) {
        NutrientAndAmount &naa = neighbor->second;
        int total = totals[naa->first];
        int possess = this->nutrients[naa->first];
        if(total > possess) { // total cannot be 0 since possess >= 0
            naa->second = possess * naa->second / total; // rounded down proportial amount of possessed amount
        }
    }
}

void MapSquare::drain() {
    if(!this->org.isAlive()) { // only drain if has an organism
        return;
    }
    for(std::pair<std::shared_ptr<MapSquare>, NutrientAndAmount> neighbor : this->neighbors) {
        NutrientAndAmount &got = neighbor->first->neighbors[std::shared_ptr(this)];
        this->nutrients[got->first] += got->second;
        neighbor->first->nutrients[got->first] -= got->second;
        got->second = 0; // prevent continued drainage if organism dies
    }
}

void MapSquare::consumeSecondary() {
    if(this->org.isAlive()) {//only consume if has an organism
        this->org.consumeSecondary(this->nutrients);
    }
}


void MapSquare::addNutrients(std::vector<NutrientAndAmount> &add) {
    for(NutrientAndAmount naa : add) {
        this->nutrients[naa->first] += naa->second;
    }
}


void MapSquare::checkDead() {
    if(this->org.isAlive() && !this->org.survivedRound()) {
        this->addNutrients(this->org.die());
    }
}


void MapSquare::produce(std::vector<Nutrient> &allNutrients) {
    if(!this->org.isAlive()) { // only produce if alive
        return;
    }
    std::vector<NutrientAndAmount> add = this->org.produce(allNutrients);
    int total = this->neighbors.size() + 1;
    // add portion to each neighbor's amounts
    for(std::pair<std::shared_ptr<MapSquare>, NutrientAndAmount> neighbor : this->neighbors) {
        for(NutrientAndAmount &naa : add) {
            neighbor->first->nutrients[naa->first] += naa->second / total;
        }
    }
    // add portion and leftovers to own amounts. roundoff must be added here so no arbitrary squares are favored
    for(NutrientAndAmount &naa : add) {
        this->nutrients[naa->first] += naa->second / total + naa->second % total;
    }
}


void MapSquare::readIn(std::istream &is, std::ostream &os) {
    os << "Number of types of nutrients on square: ";
    int n;
    is >> n;
    os << "Enter all nutrients as [name] [quantity]" << std::endl;
    for(int i = 0; i < n; i++) {
        Nutrient nut;
        int amount;
        is >> nut >> amount;
        this->nutrients[nut] = amount;
    }
}

void MapSquare::readOut(std::ostream &os, bool justReadOrg) {
    if(justReadOrg) {
        os << this->org << " ";
    } else {
        for(NutrientAndAmount n : this->nutrients) {
            os << n->first << ": " << n->second << std::endl;
        }
    }
}
