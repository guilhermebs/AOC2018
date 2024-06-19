#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>

struct point
{
    int x;
    int y;
};


void solve_pt1() {
    std::ifstream file("inputs/day06");
    std::string line;
    std::vector<point> coordinates;

    int max_x = 0;
    int max_y = 0;
    while (getline(file, line))
    {
       auto comma = line.find(',');
       point c {
            stoi(line.substr(0, comma)),
            stoi(line.substr(comma + 1))
       };
       coordinates.push_back(c);
       std::cout << c.x << ", " << c.y << std::endl;
       max_x = std::max(max_x, c.x);
       max_y = std::max(max_y, c.y);
    }
    ++max_x;
    ++max_y;

    std::vector<int> locations(max_x*max_y, -99);
    std::unordered_set<int> border_locations;

    for (int y = 0; y < max_y; y++)
    {
        for (int x = 0; x < max_x; x++)
        {
            int closest = -99;
            int closest_dist = INT_MAX;
            for (size_t i = 0; i < coordinates.size(); ++i) {
                auto c = coordinates[i];
                auto dist = labs(c.x - x) + labs(c.y - y);
                if (dist < closest_dist) {
                    closest_dist = dist;
                    closest = i;
                } else if (dist == closest_dist)
                    closest = -99;
                
            }
            locations[x + max_x * y] = closest;
            std::cout << closest << " ";
            if (x == 0 || x == max_x - 1 || y == 0 || y == max_y - 1)
                border_locations.insert(closest);
        }
        std::cout << std::endl;
    }

    int largest_area = 0;
    for (size_t i = 0; i < coordinates.size(); i++)
    {
        if (!border_locations.contains(i)) {
            int size = std::count(locations.begin(), locations.end(), i);
            largest_area = std::max(size, largest_area);
        }
    }
    std::cout << "Part 1 solution: " << largest_area << std::endl;
    
    
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_pt1();
    //solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}