#include <chrono>
#include <fstream>
#include <unordered_set>
#include "helper.hpp"

void solve_part1() {
    std::ifstream file("inputs/day01");
    std::string line;
    
    int value = 0;
    while (std::getline(file, line))
    {
        value += std::stoi(line);
    }
    
    std::cout << "Part 1: " << value << std::endl;
}

void solve_part2() {
    std::ifstream file("inputs/day01");
    std::string line;

    std::unordered_set<int> values{0}; 
    int cur_value = 0;
    int solution = -999;
    
    while (true)
    {
        if (std::getline(file, line)) {
            cur_value += std::stoi(line);
            if (values.contains(cur_value)) {
                solution = cur_value;
                break;
            } else {
                values.insert(cur_value);
            }
        } else {
            file.clear();
            file.seekg(0);
        }
    }
    
    std::cout << "Part 1: " << solution << std::endl;
}


int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_part1();
    solve_part2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";

}