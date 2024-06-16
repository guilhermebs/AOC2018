#include <fstream>
#include "helper.hpp"

void solve_part1() {
    std::ifstream file("inputs/day02");
    std::string line;

    int count_2 = 0;
    int count_3 = 0;

    while (std::getline(file, line))
    {
        std::sort(line.begin(), line.end());
        std::string unique;
        std::unique_copy(line.begin(), line.end(), std::back_inserter(unique));
        bool has_2 = false;
        bool has_3 = false;
        for (auto uc: unique) {
            int count = 0;
            for (auto c: line) {
                count += (uc == c);
            }
            has_2 |= (count == 2);
            has_3 |= (count == 3);
        }
        count_2 += has_2;
        count_3 += has_3;
    }
    int solution = count_2 * count_3;
    std::cout << "Part 1 solution: " << solution << std::endl;
    
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_part1();
    //solve_part2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}