#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <regex>

class Nanobot
{
    public:
        int x;
        int y;
        int z;
        uint r;

        Nanobot(const std::string &line)
        {
            static std::regex pattern("pos=<(-?\\d+),(-?\\d+),(-?\\d+)>, r=(\\d+)");
            std::smatch match;
            if (std::regex_match(line, match, pattern)) {
                x = stoi(match[1]);
                y = stoi(match[2]);
                z = stoi(match[3]);
                r = stoi(match[4]);
            } else {
                std::cout << "Error!" << std::endl;
            }
        }

        inline uint distance (const Nanobot &other)
        {
            return abs(x - other.x) + abs(y - other.y) + abs(z - other.z);
        }
};


void solve_pt1()
{
    std::ifstream file("inputs/day23");
    std::string line;
    std::vector<Nanobot> nanobots;
    while (getline(file, line))
    {
        nanobots.push_back(Nanobot(line));
    }
    uint max_r = 0;
    size_t max_idx = 0;
    for (size_t i = 0; i < nanobots.size(); i++)
    {
        if (nanobots[i].r > max_r)
        {
            max_r = nanobots[i].r;
            max_idx = i;
        }
    }
    size_t part1_solution = 0;
    for (auto nb: nanobots)
        part1_solution += (nanobots[max_idx].distance(nb) <= max_r);
    std::cout << "Part 1 solution: " << part1_solution << std::endl;
    
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_pt1();
    //solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}