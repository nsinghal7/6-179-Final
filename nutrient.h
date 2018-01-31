struct Nutrient {
    std::string name;
    
    friend std::istream &operator>>(std::istream &is, Nutrient &n) {
        is >> n.name;
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const Nutrient &n) {
        os << n.name;
        return os;
    }
}