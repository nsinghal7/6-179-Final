
#ifndef NUTRIENT_H
#define NUTRIENT_H

#include <iostream>

struct Nutrient {
    std::string name;
    
    bool isNull() {
        return name == "";
    }

    friend std::istream &operator>>(std::istream &is, Nutrient &n) {
        is >> n.name;
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const Nutrient &n) {
        os << n.name;
        return os;
    }

    friend bool operator==(const Nutrient &a, const Nutrient &b) {
        return a.name == b.name;
    }

    friend bool operator!=(const Nutrient &a, const Nutrient &b) {
        return a.name != b.name;
    }

    friend bool operator<(const Nutrient &a, const Nutrient &b) {
        return a.name < b.name;
    }
};

typedef std::pair<Nutrient, int> NutrientAndAmount;

#endif
