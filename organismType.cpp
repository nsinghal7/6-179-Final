#include "organismType.h"

double OrganismType::scoreCreationOnSquare(std::map<Nutrient, int> &nutrients) const {
    int amount = nutrients[this->primaryReqNutrient];
    if(amount < this->createAmount) {
        return 0;
    } else {
        return ((double) amount) / this->createAmount;
    }
}

void OrganismType::readIn(std::istream &is, std::ostream &os) {
    os << "Name of organism (recommended 1 char): ";
    is >> this->name;
    os << "Primary consumption nutrient name: ";
    is >> this->primaryReqNutrient;
    os << "Primary production nutrient name: ";
    is >> this->primaryProdNutrient;
    os << "Primary nutrient cost for creation: ";
    is >> this->createAmount;
    os << "Primary nutrient drain amount per square per turn: ";
    is >> this->drainAmount;
    os << "Primary/secondary nutrient consumption amount per turn: ";
    is >> this->consumeAmount;
    os << "Primary nutrient production amount per turn: ";
    is >> this->primaryProdAmount;
    os << "Other nutrient production amount per nutrient per turn: ";
    is >> this->otherProdAmount;
}

std::ostream &operator<<(std::ostream &os, const OrganismType &ot) {
    os << ot.name;
    return os;
}
