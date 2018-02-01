#include "organism.h"

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
        this->consumed = 0;
        return true;
    } else {
        return false;
    }
}

NutrientAndAmount Organism::drainStats() const {
    return std::make_pair(this->ot->primaryReqNutrient, this->ot->drainAmount);
}

std::vector<NutrientAndAmount> Organism::produce(std::vector<Nutrient> &nutrients) {
    int total = nutrients.size();
    std::vector<NutrientAndAmount> ans(total);
    int i = 0;
    for(Nutrient &n : nutrients) {
        if(n == this->ot->primaryProdNutrient) {
            ans[i] = std::make_pair(n, this->ot->primaryProdAmount);
        } else {
            ans[i] = std::make_pair(n, this->ot->otherProdAmount);
        }
        i++;
    }
    return ans;
}


std::vector<NutrientAndAmount> Organism::die() {
    std::vector<NutrientAndAmount> ans(3);
    ans[0] = std::make_pair(this->ot->primaryReqNutrient, this->ot->createAmount / 4);
    ans[1] = std::make_pair(this->secondaryReqNutrient.isNull() ? this->ot->primaryReqNutrient : this->secondaryReqNutrient, this->consumed);
    ans[2] = std::make_pair(this->ot->primaryProdNutrient, this->ot->createAmount * 3 / 4 + this->ot->createAmount % 4);
    this->isNull = true;
    return ans;
}

void Organism::create(std::shared_ptr<OrganismType> ot, Nutrient &secondaryReqNutrient) {
    this->ot = ot;
    this->secondaryReqNutrient = secondaryReqNutrient;
    this->isNull = false;
    this->consumed = 0;
}
