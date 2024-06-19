#include <fstream>
#include <string>
#include <iostream>

bool react(std::string &polymer) {
    bool result = false;
    int i = 0;
    int ii = 1;
    while (ii < (int) polymer.length()) {
        auto ci = polymer[i];
        auto cii = polymer[ii];
        if (isupper(ci) xor isupper(cii) && toupper(ci) == toupper(cii)) {
            polymer.erase(polymer.begin() + i);
            polymer.erase(polymer.begin() + ii - 1);
            result = true;
            i = std::max(i-1, 0);
            ii = std::max(i+1, ii-1);
            continue;
        };
        ++i;
        ++ii;
    };
    return result;
}


void solve () {
    std::ifstream file("inputs/day05");
    std::string polymer;
    getline(file, polymer);
    react(polymer);
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
        polymer_clean.erase(std::remove(polymer_clean.begin(), polymer_clean.end(), u), polymer_clean.end());
        polymer_clean.erase(std::remove(polymer_clean.begin(), polymer_clean.end(), toupper(u)), polymer_clean.end());
        react(polymer_clean);
        min_count = std::min(min_count, polymer_clean.length());
    }
    std::cout << "Part 2 solution: " << min_count << std::endl;
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}