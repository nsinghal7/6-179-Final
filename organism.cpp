#include "organism.h"

double OrganismType::scoreCreationOnSquare(std::vector<NutrientAndAmount> &nutrients) const {
    int amount = -1;
    for(NutrientAndAmount &naa : nutrients) {
        if(naa->first == this->primaryReqNutrient) {
            amount = naa->second;
            break;
        }
    }
    if(amount < this->createAmount) {
        return 0
    } else {
        return ((double) amount) / this->createAmount;
    }
}

std::istream &operator>>(std::istream &is, OrganismType &ot) {
    is >> ot.name;
    is >> ot.primaryReqNutrient;
    is >> ot.primaryProdNutrient;
    is >> ot.createAmount;
    is >> ot.drainAmount;
    is >> ot.consumeAmount;
    is >> ot.primaryProdAmount;
    is >> ot.otherProdAmount;
    return is;
}

std::ostream &operator<<(std::ostream &os, const OrganismType &ot) {
    os << ot.name;
    return os;
}





void Organism::consumeForCreation(std::vector<NutrientAndAmount> &nutrients) {
    for(NutrientAndAmount &naa : nutrients) {
        if(naa->first == this->ot->primaryReqNutrient) {
            naa->second -= this->ot->createAmount;
            break;
        }
    }
}

void Organism::consume(Nutrient &nutrient, std::vector<NutrientAndAmount> &nutrients)  {
    int needed = this->ot->consumeAmount - this->consumed;
    if(needed == 0) {
        return;
    }
    for(NutrientAndAmount &naa : nutrients) {
        if(naa->first == nutrient) {
            if(naa->second >= needed) {
                naa->second -= needed;
                this->consumed += needed;
                break;
            } else {
                this->consumed += naa->second;
                naa->second = 0;
            }
        }
    }
}

void Organism::consumePrimary(std::vector<NutrientAndAmount> &nutrients) {
    this->consume(this->ot->primaryReqNutrient, nutrients);
}

void Organism::consumeSecondary(std::vector<NutrientAndAmount> &nutrients) {
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
    return ans;
}
