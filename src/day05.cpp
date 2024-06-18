#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

bool react(const std::string &polymer, std::vector<uint8_t> &mask) {
    bool result = false;
    size_t last_i = 0;
    while (mask[last_i])
        last_i++;
    
    size_t i = last_i + 1;
    while (mask[i])
        i++;
    
    while (i < polymer.length()) {
        auto ci = polymer[last_i];
        auto cii = polymer[i];
        if (isupper(ci) xor isupper(cii) && toupper(ci) == toupper(cii)) {
            mask[last_i] = 1;
            mask[i] = 1;
            result = true;
        };
        last_i = i;
        while (mask[last_i])
            last_i++;
        i = last_i + 1;
        while (mask[i])
            i++;
    };
    return result;
}


void solve_pt1 () {
    std::ifstream file("inputs/day05");
    std::string polymer;
    getline(file, polymer);
    std::cout << polymer.length() << std::endl;
    std::vector<uint8_t> mask(polymer.length(), 0);
    std::cout << "mask: " << std::endl;
    while (react(polymer, mask))
        continue;
    std::cout << "Part 1 solution: " << std::count(mask.begin(), mask.end(), 0) << std::endl;
}

void solve_pt2 () {
    std::ifstream file("inputs/day05");
    std::string polymer;
    getline(file, polymer);
    static std::string units = "abcdefghijklmnopqrstuvxwyz";
    size_t min_count = polymer.length();
    for (auto u: units){
        std::vector<uint8_t> mask(polymer.length(), 0);
        for (size_t i = 0; i < polymer.length(); i++) {
            if (tolower(polymer[i]) == u)
                mask[i] = 1;
        }
        while (react(polymer, mask))
            continue;
        size_t count = std::count(mask.begin(), mask.end(), 0);
        min_count = std::min(min_count, count);
    }
    std::cout << "Part 2 solution: " << min_count << std::endl;
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_pt1();
    solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}