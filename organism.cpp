#include "organism.h"

double OrganismType::scoreCreationOnSquare(std::vector<NutrientAndAmount> &nutrients) const {
    int amount = -1;
    for(NutrientAndAmount naa : nutrients) {
        if(naa->first == this->primaryReqNutrient) {
            amount = naa->second;
            break
        }
    }
    if(amount < this->createAmount) {
        return 0
    } else {
        return ((double) amount) / this->createAmount;
    }
}

std::istream &operator>>(std::istream &is, OrganismType &ot) {
    std::cout << "Name of organism (1 char): ";
    is >> ot.name;
    std::cout << "Primary consumption nutrient name: ";
    is >> ot.primaryReqNutrient;
    std::cout << "Primary production nutrient name: ";
    is >> ot.primaryProdNutrient;
    std::cout << "Primary nutrient cost for creation: ";
    is >> ot.createAmount;
    std::cout << "Primary nutrient drain amount per square per turn: ";
    is >> ot.drainAmount;
    std::cout << "Primary/secondary nutrient consumption amount per turn: ";
    is >> ot.consumeAmount;
    std::cout << "Primary nutrient production amount per turn: ";
    is >> ot.primaryProdAmount;
    std::cout << "Other nutrient production amount per nutrient per turn: ";
    is >> ot.otherProdAmount;
    return is;
}

std::ostream &operator<<(std::ostream &os, const OrganismType &ot) {
    os << ot.name;
    return os;
}
