#include <fstream>
#include <string>
#include <iostream>

bool react(std::string &polymer) {
    bool result = false;
    size_t i = 1;
    while (i < polymer.length()) {
        auto ci = polymer[i-1];
        auto cii = polymer[i];
        if (isupper(ci) xor isupper(cii) && toupper(ci) == toupper(cii)) {
            polymer.erase(i-1, 2);
            result = true;
        };
        i++;
    };
    return result;
}


void solve_pt1 () {
    std::ifstream file("inputs/day05");
    std::string polymer;
    getline(file, polymer);
    std::cout << polymer.length() << std::endl;
    while (react(polymer))
        continue;
    std::cout << "Part 1 solution: " << polymer.length() << std::endl;
}

void solve_pt2 () {
    std::ifstream file("inputs/day05");
    std::string polymer;
    getline(file, polymer);
    static std::string units = "abcdefghijklmnopqrstuvxwyz";
    size_t min_count = polymer.length();
    for (auto u: units){
        std::string polymer_clean = polymer;
        while (react(polymer_clean))
            continue;
    }
    std::cout << "Part 2 solution: " << polymer.length() << std::endl;
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_pt1();
    //solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}