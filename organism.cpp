#include "organism.h"

double OrganismType::scoreCreationOnSquare(std::map<Nutrient, int> &nutrients) const {
    int amount = nutrients[this->primaryReqNutrient];
    if(amount < this->createAmount) {
        return 0;
    } else {
        return ((double) amount) / this->createAmount;
    }
}

std::istream &operator>>(std::istream &is, OrganismType &ot) {
    std::cout << "Name of organism (recommended 1 char): ";
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





void Organism::consumeForCreation(std::map<Nutrient, int> &nutrients) {
    nutrients[this->ot->primaryReqNutrient] -= this->ot->createAmount;
}

void Organism::consume(Nutrient &nutrient, std::map<Nutrient, int> &nutrients)  {
    int needed = this->ot->consumeAmount - this->consumed;
    if(needed == 0) {
        return;
    }
    int &amount = nutrients[nutrient];
    if(amount >= needed) {
        amount -= needed;
        this->consumed += needed;
    } else {
        this->consumed += amount;
        amount = 0;
    }
}

void Organism::consumePrimary(std::map<Nutrient, int> &nutrients) {
    this->consume(this->ot->primaryReqNutrient, nutrients);
}

void Organism::consumeSecondary(std::map<Nutrient, int> &nutrients) {
    this->consume(this->ot->primaryReqNutrient, nutrients);
    this->consume(this->secondaryReqNutrient, nutrients);
}

bool Organism::survivedRound() {
    if(this->consumed >= this->ot->consumeAmount) {
        this->consumed = 0
        return true;
    } else {
        return false;
    }
}

NutrientAndAmount Organism::drainStats() const {
    return std::make_pair(this->ot->primaryReqNutrient, this->ot->drainAmount);
}

std::vector<NutrientAndAmount> &Organism::produce(std::vector<Nutrient> &nutrients) {
    int total = nutrients.size();
    std::vector<NutrientAndAmount> ans(total);
    int i = 0
    for(Nutrient &n : nutrients) {
        if(n == this->ot->primaryProdNutrient) {
            ans[i] = make_pair(n, this->ot->primaryProdAmount);
        } else {
            ans[i] = make_pair(n, this->ot->otherProdAmount);
        }
        i++;
    }
    return ans;
}


std::vector<NutrientAndAmount> &Organism::die() {
    std::vector<NutrientAndAmount> ans(3);
    ans[0] = make_pair(this->ot->primaryReqNutrient, this->ot->createAmount / 4);
    ans[1] = make_pair(this->secondaryReqNutrient, this->consumed);
    ans[2] = make_pair(this->ot->primaryProdNutrient, this->ot->createAmount * 3 / 4);
    this->isNull = true;
    return ans;
}

void Organism::create(std::shared_ptr<OrganismType> ot, Nutrient &secondaryReqNutrient) {
    this->ot = ot;
    this->secondaryReqNutrient = secondaryReqNutrient;
    this->isNull = false;
    this->consumed = 0;
}
