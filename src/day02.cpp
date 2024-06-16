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

int compare_ids(std::string &id1, std::string &id2) {
    if (id1.length() != id2.length())
        return -1;
    int differences = -1;
    for (u_long i = 0; i < id1.length(); i++) {
        if (id1[i] != id2[i] && differences == -1) {
            differences = i;
        } else if (id1[i] != id2[i] && differences != -1) {
            return -1;
        }
    }
    return differences;
}

void solve_part2() {
    std::ifstream file("inputs/day02");
    std::vector<std::string> box_ids;
    std::string line;

    while (std::getline(file, line)){
        box_ids.push_back(line);
    }

    std::string solution = "";
    for(auto id1: box_ids) {
        for(auto id2: box_ids) {
            int compare = compare_ids(id1, id2);
            if (compare != -1) {
                id1.erase(compare, 1);
                solution = id1;
                break;
            }
        }
    }

    std::cout << "Part 2 solution: " << solution << std::endl;
    
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_part1();
    solve_part2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}