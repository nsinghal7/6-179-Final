#include "map.h"

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











void Map::readIn(std::istream &is, std::ostream &os) {
    os << "Dimensions of map as [rows] [cols]: ";
    int rows;
    int cols;
    is >> rows >> cols;
    // create map
    for(int i = 0; i < rows; i++) {
        std::vector<MapSquare> row;
        for(int j = 0; j < cols; j++) {
            row.push_back(make_shared(this->allNutrients));
        }
        this->map.push_back(row);
    }
    // link adjacent squares to each other
    Nutrient def = this->allNutrients[0];
    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
            for(int drow = -1; drow <= 1; drow ++) {
                for(int dcol = -1; dcol <= 1; dcol ++) {
                    if(drow != 0 || dcol != 0) {
                        // link to all adjacent squares, wrapping around edges of array
                        std::shared_ptr<MapSquare> neighbor = this->map[(row + drow) % rows][(col + dcol) % cols];
                        this->map[row][col]->neighbors[neighbor] = std::make_pair(def, 0);
                    }
                }
            }
        }
    }
    // fill squares with nutrients
    int n;
    os << "Number of squares with nutrient contents: ";
    is >> n;
    for(int i = 0; i < n; i++) {
        os << "coordinates of a square with nutrient contents as [row] [col]: ";
        int row;
        int col;
        is >> row >> col;
        this->map[row][col]->readIn(is, os);
    }
}


std::ostream &operator<<(std::ostream &os, const Map &m) {
    for(int row = 0; row < this->map.size(); row++) {
        for(int col = 0; col < this->map[row].size(); col++) {
            this->map[row][col]->readOut(os, true);
        }
        os << std::endl;
    }
}


void Map::readOutSquare(std::ostream &os, int row, int col) {
    this->map[row][col]->readOut(os, false);
}


void Map::runRound() {
    // Generate life on all squares that can and don't already have life
    for(auto row : this->map) {
        for(auto square : row) {
            square->checkCreateLife(this->allTypes);
        }
    }
    // Organisms begin eating what they can of just their primary nutrient
    for(auto row : this->map) {
        for(auto square : row) {
            square->consumePrimary();
        }
    }
    // Each organism tries to drain primary nutrients from its neighbor. Cannot actually drain yet since others might compete
    for(auto row : this->map) {
        for(auto square : row) {
            square->requestDrain();
        }
    }
    // Each square figures out how much each of its neighbors get based on how much they requested
    for(auto row : this->map) {
        for(auto square : row) {
            square->divideRequests();
        }
    }
    // Each organism then takes its allotment from each of its neighbors
    for(auto row : this->map) {
        for(auto square : row) {
            square->drain();
        }
    }
    // Each organism then consumes the rest of what it needs but didn't get before
    for(auto row : this->map) {
        for(auto square : row) {
            square->consumeSecondary();
        }
    }
    // check if any organism starved. If so, dissolve its resources and add them to its neighboring squares
    for(auto row : this->map) {
        for(auto square : row) {
            square->checkDead();
        }
    }
    // all surviving organisms should then produce resources
    for(auto row : this->map) {
        for(auto square : row) {
            square->produce(this->allNutrients);
        }
    }
}
