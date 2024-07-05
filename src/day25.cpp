#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <array>
#include <list>
#include <algorithm>

#include "helper.hpp"

using Point = std::array<int, 4>;

inline int dist(const Point &p1, const Point &p2)
{
    return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])+ abs(p1[2] - p2[2])+ abs(p1[3] - p2[3]);
}

std::vector<Point> gather_constellation(std::list<Point> &points)
{
    std::vector<Point> result{points.front()};
    points.front();
    while (true)
    {
        std::vector<std::list<Point>::iterator> add;
        for (auto p = points.begin(); p != points.end(); p++)
        {
            for (auto &c: result)
            {
                if(dist(*p, c) <= 3)
                {
                    result.push_back(*p);
                    add.push_back(p);
                    break;
                }
            }
        }
        std::cout << add.size() << std::endl;
        if (add.size() == 0) break;
        std::reverse(add.end(), add.begin());
        for (auto p : add)
        {
            points.erase(p);
        }
    }
    return result;
}


void solve()
{
    std::ifstream file("inputs/day25");
    std::string line;
    std::list<Point> points;
    while (getline(file, line))
    {
        Point point;
        int i = 0;
        for (auto s: tokenize2(line, ","))
            point[i++] = stoi(s);
        points.push_back(point);
    }
    std::cout << points.size() << std::endl;
    int solution = 0;
    while (points.size() > 0)
    {
        auto c = gather_constellation(points);
        std::cout << c.size() << " " << points.size() << std::endl;
        solution++;
    }
    std::cout << "Solution: " << solution << std::endl;
}



int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
};