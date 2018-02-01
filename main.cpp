#include <iostream>
#include <fstream>
#include <vector>
#include "nutrient.h"
#include "organismType.h"
#include "map.h"


int main() {
    std::cout << "Filename to load data or 'No' to use std::cin: ";
    std::string f;
    std::cin >> f;
    std::ifstream filestream;
    std::ofstream ignorestream; // will not be opened, just used to ignore output
    if(f != "No") {
        filestream.open(f);
    }

    std::istream &is = (f == "No" ? std::cin : filestream);
    std::ostream &out = ( f == "No" ? std::cout : ignorestream);

    int n = 0;
    bool first = true;
    while(n <= 0) {
        if(first) {
            first = false;
        } else {
            out << "Must have some nutrients" << std::endl;
            is.clear();
            is.ignore(100, '\n');
        }
        // get all nutrients
        out << "Number of nutrient types: ";
        is >> n;
    }

    std::vector<Nutrient> nutrients(n);
    for(int i = 0; i < n; i++) {
        out << "Name of nutrient: ";
        is >> nutrients[i];
    }
    n = 0;
    first = true;
    while(n <= 0) {
        if(first) {
            first = false;
        } else {
            out << "Must have some organisms" << std::endl;
            is.clear();
            is.ignore(100, '\n');
        }
        // get all organism types
        out << "Number of organism types: ";
        is >> n;
    }

    std::vector<std::shared_ptr<OrganismType>> types(n);
    for(int i = 0; i < n; i++) {
        types[i] = std::make_shared<OrganismType>();
        types[i]->readIn(is, out, nutrients);
    }

    // get the map
    Map map(nutrients, types);
    map.readIn(is, out);

    if(f != "No") {
        filestream.close();
    }

    // run rounds
    int round = 1;
    while(true) {
        std::cout << "Starting round " << round << std::endl;
        std::cout << "Enter command (help for help): ";
        std::string command;
        std::cin >> command;
        if(command == "next") {
            map.runRound();
            round ++;
            std::cout << map; // show current map with organisms, no nutrient info
        } else if(command == "info") {
            int row;
            int col;
            std::cout << "Enter coordinates of square to get info on as [row] [col]: ";
            std::cin >> row >> col;
            map.readOutSquare(std::cout, row, col);
        } else if(command == "map") {
            std::cout << "Enter the name of the nutrient to map: ";
            Nutrient nut;
            std::cin >> nut;
            map.readMapForNutrient(std::cout, nut);
        } else if(command == "skip") {
            int skip;
            std::cin >> skip; // they're impatient, so don't bother with more instructions
            for(int i = 0; i < skip; i++) {
                map.runRound();
                round ++;
            }
            std::cout << map;
        } else if (command == "quit") {
            break;
        } else { // go to help for all unknown commands
            if(command != "help") {
                std::cout << "Command not recognized" << std::endl;
            }
            std::cout << "Use 'next' to run the next round and show the map of organisms afterwards" << std::endl
                        << "Use 'info' to get information on the nutrients at a square" << std::endl
                        << "Use 'map' to get a full map with amounts for a nutrient" << std::endl
                        << "Use 'skip [int k]' to run the next k rounds and only show the map at the end" << std::endl
                        << "Use 'quit' to quit the program" << std::endl;
        }
    }
    return 0;
}
