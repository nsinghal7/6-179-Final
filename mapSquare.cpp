#include "mapSquare.h"

MapSquare::MapSquare(std::vector<Nutrient> &nuts) {
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
        Nutrient primary = bestType->primary();
        Nutrient secondary;
        int largestAmount = -1;
        for(NutrientAndAmount n : this->nutrients) {
            if(n.second > largestAmount && n.first != primary) {
                secondary = n.first;
                largestAmount = n.second;
            }
        }

        this->org.create(bestType, secondary);
        this->org.consumeForCreation(this->nutrients);
    }
}

void MapSquare::consumePrimary() {
    if(this->org.isAlive()) { // only consume if has an organism
        this->org.consumePrimary(this->nutrients);
    }
}

void MapSquare::requestDrain(std::shared_ptr<MapSquare> self) {
    if(!this->org.isAlive()) { // only drain if has an organism
        return;
    }
    NutrientAndAmount stats = this->org.drainStats();

    for(std::pair<std::shared_ptr<MapSquare>, NutrientAndAmount> neighbor : this->neighbors) {
        // for each neighbor, set their stored value for me to this stats value
        neighbor.first->neighbors[self] = stats;
    }
}


void MapSquare::divideRequests() {
    // all squares divide requests
    std::map<Nutrient, int> totals;
    for(auto it = this->neighbors.begin(); it != this->neighbors.end(); it++) {
        NutrientAndAmount &naa = it->second;
        totals[naa.first] += naa.second;
    }
    for(auto it = this->neighbors.begin(); it != this->neighbors.end(); it++) {
        NutrientAndAmount &naa = it->second;
        int total = totals[naa.first];
        int possess = this->nutrients[naa.first];
        if(total > possess) { // total cannot be 0 since possess >= 0
            naa.second = possess * naa.second / total; // rounded down proportial amount of possessed amount
        }
    }
}

void MapSquare::drain(std::shared_ptr<MapSquare> self) {
    if(!this->org.isAlive()) { // only drain if has an organism
        return;
    }
    for(auto it = this->neighbors.begin(); it != this->neighbors.end(); it++) {
        NutrientAndAmount &got = it->first->neighbors[self];
        this->nutrients[got.first] += got.second;
        it->first->nutrients[got.first] -= got.second;
        got.second = 0; // prevent continued drainage if organism dies
    }
}

void MapSquare::consumeSecondary() {
    if(this->org.isAlive()) {//only consume if has an organism
        this->org.consumeSecondary(this->nutrients);
    }
}


void MapSquare::checkDead() {
    if(this->org.isAlive() && !this->org.survivedRound()) {
        for(NutrientAndAmount naa : this->org.die()) {
            this->nutrients[naa.first] += naa.second;
        }
    }
}


void MapSquare::produce(std::vector<Nutrient> &allNutrients) {
    if(!this->org.isAlive()) { // only produce if alive
        return;
    }
    std::vector<NutrientAndAmount> add = this->org.produce(allNutrients);
    int total = this->neighbors.size() + 1;
    // add portion to each neighbor's amounts
    for(auto it = this->neighbors.begin(); it != this->neighbors.end(); it++) {
        for(NutrientAndAmount &naa : add) {
            it->first->nutrients[naa.first] += naa.second / total;
        }
    }
    // add portion and leftovers to own amounts. roundoff must be added here so no arbitrary squares are favored
    for(NutrientAndAmount &naa : add) {
        this->nutrients[naa.first] += naa.second / total + naa.second % total;
    }
}


void MapSquare::readIn(std::istream &is, std::ostream &os, std::vector<Nutrient> &allNutrients) {
    os << "Number of types of nutrients on square: ";
    int n;
    is >> n;
    os << "Enter all nutrients as [name] [quantity]" << std::endl;
    for(int i = 0; i < n; i++) {
        bool first = true;
        Nutrient nut;
        int amount = -1;
        while(amount < 0 || std::find(allNutrients.begin(), allNutrients.end(), nut) == allNutrients.end()) {
            if(first) {
                first = false;
            } else {
                os << "Nutrient not known" << std::endl;
            }
            is >> nut >> amount;
            this->nutrients[nut] = amount;
        }
    }
}

void MapSquare::readOut(std::ostream &os, bool justReadOrg) {
    if(justReadOrg) {
        if(this->org.isAlive()) {
            os << this->org;
        } else {
            os << ".";
        }
        os << " ";
    } else {
        for(NutrientAndAmount n : this->nutrients) {
            os << n.first << ": " << n.second << std::endl;
        }
    }
}
