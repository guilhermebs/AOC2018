#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>

using requirements = std::set<char>;

bool contains_requirements(const std::string &steps, const requirements &req) {
    for (auto r: req) {
        if (steps.find(r) == std::string::npos)
            return false;
    }
    return true;
}

void solve() {
    std::ifstream file("inputs/day07");
    std::string line;

    std::map<char, requirements> steps;
    while (getline(file, line))
    {
        char s = line[36];
        char req = line[5];
        if (steps.contains(s))
            steps[s].insert(req);
        else {
            steps[s] = requirements{req};
        }
        if (!steps.contains(req))
            steps[req] = requirements();
    }
    std::string completed("");
    while (completed.length() < steps.size())
    {
        for (auto &[s, req]: steps) {
            if (completed.find(s) == std::string::npos && contains_requirements(completed, req)) {
                completed.push_back(s);
                break;
            }
        }
    }
    std::cout << "Part 1 solution: " << completed << std::endl;
}


int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}