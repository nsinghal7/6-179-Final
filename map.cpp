#include "map.h"

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
            square->requestDrain(square);
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
            square->drain(square);
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
