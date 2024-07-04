#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <tuple>
#include <array>
#include <algorithm>
#include <bit>
#include <queue>
#include <regex>

class Cube
{
    public:
        int len;
        std::array<int, 3> center;
        int dist;

        Cube() = default;
        Cube(int len, std::array<int, 3> center): len(len), center(center)
        {
            dist = abs(center[0]) + abs(center[1]) + abs(center[2]);
        };

        std::vector<Cube> split() const {
            int o = std::max(len/4, 1);
            auto l = len/2;
            std::vector<Cube> result = {
                Cube(l, std::array<int, 3>{center[0] + o, center[1] + o, center[2] + o}),
                Cube(l, std::array<int, 3>{center[0] + o, center[1] + o, center[2] - o}),
                Cube(l, std::array<int, 3>{center[0] + o, center[1] - o, center[2] + o}),
                Cube(l, std::array<int, 3>{center[0] - o, center[1] + o, center[2] + o}),
                Cube(l, std::array<int, 3>{center[0] + o, center[1] - o, center[2] - o}),
                Cube(l, std::array<int, 3>{center[0] - o, center[1] + o, center[2] - o}),
                Cube(l, std::array<int, 3>{center[0] - o, center[1] - o, center[2] + o}),
                Cube(l, std::array<int, 3>{center[0] - o, center[1] - o, center[2] - o}),
            };
            if (len == 2)
            {
                result.push_back(Cube(l, center));
                result.push_back(Cube(l, std::array<int, 3>{center[0] + o, center[1], center[2]}));
                result.push_back(Cube(l, std::array<int, 3>{center[0] - o, center[1], center[2]}));
                result.push_back(Cube(l, std::array<int, 3>{center[0], center[1] + o, center[2]}));
                result.push_back(Cube(l, std::array<int, 3>{center[0], center[1] - o, center[2]}));
                result.push_back(Cube(l, std::array<int, 3>{center[0], center[1], center[2] + o}));
                result.push_back(Cube(l, std::array<int, 3>{center[0], center[1], center[2] - o}));
                result.push_back(Cube(l, std::array<int, 3>{center[0] + o, center[1] + o, center[2]}));
                result.push_back(Cube(l, std::array<int, 3>{center[0] + o, center[1] - o, center[2]}));
                result.push_back(Cube(l, std::array<int, 3>{center[0] - o, center[1] + o, center[2]}));
                result.push_back(Cube(l, std::array<int, 3>{center[0] - o, center[1] - o, center[2]}));
                result.push_back(Cube(l, std::array<int, 3>{center[0] + o, center[1], center[2] + o}));
                result.push_back(Cube(l, std::array<int, 3>{center[0] + o, center[1], center[2] - o}));
                result.push_back(Cube(l, std::array<int, 3>{center[0] - o, center[1], center[2] + o}));
                result.push_back(Cube(l, std::array<int, 3>{center[0] - o, center[1], center[2] - o}));
                result.push_back(Cube(l, std::array<int, 3>{center[0], center[1] + o, center[2] + o}));
                result.push_back(Cube(l, std::array<int, 3>{center[0], center[1] + o, center[2] - o}));
                result.push_back(Cube(l, std::array<int, 3>{center[0], center[1] - o, center[2] + o}));
                result.push_back(Cube(l, std::array<int, 3>{center[0], center[1] - o, center[2] - o}));
            }
            return result;
        }

        void print() const
        {
            std::cout << "(" << center[0] << ", " << center[1] << ", " << center[2] << ", " << len << ")";
        }

        bool operator< (const Cube &other) const
        {
            return (len == other.len)? (dist > other.dist) : (len < other.len);
        }
};

class Nanobot
{
    public:
        std::array<int, 3> coords;
        uint r;

        Nanobot(const std::string &line)
        {
            static std::regex pattern("pos=<(-?\\d+),(-?\\d+),(-?\\d+)>, r=(\\d+)");
            std::smatch match;
            if (std::regex_match(line, match, pattern)) {
                coords[0] = stoi(match[1]);
                coords[1] = stoi(match[2]);
                coords[2] = stoi(match[3]);
                r = stoi(match[4]);
            } else {
                std::cout << "Error!" << std::endl;
            }
        }

        uint distance (const Nanobot &other) const
        {
            uint result = 0;
            for (int i = 0; i < 3; i++)
                result += abs(coords[i] - other.coords[i]);
            return result;
        }

        bool in_cube(const Cube &cube) const
        {
            std::array<int, 3> proj;
            for (int i = 0; i < 3; i++)
                proj[i] = std::clamp(coords[i], cube.center[i] - cube.len/2, cube.center[i] + cube.len/2);
            return (abs(proj[0] - coords[0]) + abs(proj[1] - coords[1]) + abs(proj[2] - coords[2]) <= (int) r);
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
    for (auto &nb: nanobots)
        part1_solution += (nanobots[max_idx].distance(nb) <= max_r);
    std::cout << "Part 1 solution: " << part1_solution << std::endl;
    
}

void solve_pt2()
{
    std::ifstream file("inputs/day23");
    std::string line;
    std::vector<Nanobot> nanobots;
    while (getline(file, line))
        nanobots.push_back(Nanobot(line));
    
    std::array<int, 3> coords_min = {INT_MAX, INT_MAX, INT_MAX};
    std::array<int, 3> coords_max = {INT_MIN, INT_MIN, INT_MIN};
    for (const auto &nb: nanobots)
    {
        for (int i=0; i < 3; i++)
        {
            coords_min[i] = std::min(coords_min[i], nb.coords[i]);
            coords_max[i] = std::max(coords_max[i], nb.coords[i]);
        }
    }

    std::array<int, 3> center = {0, 0, 0};

    uint length = std::max({
        abs(coords_max[0]), abs(coords_min[0]), abs(coords_max[1]), abs(coords_min[1]), abs(coords_max[2]), abs(coords_min[2])
    });
    length = std::bit_ceil(length);

    std::priority_queue<std::pair<size_t, Cube>> queue;
    Cube init_cube(length, center);
    auto n = std::count_if(nanobots.begin(), nanobots.end(), [&](const Nanobot &nb) { return nb.in_cube(init_cube);});
    queue.push({n, init_cube});
    int part2_sol = 0;;
    int i = 0;
    while (queue.size() > 0)
    {
        auto [n, cube] = queue.top();
        queue.pop();
        if (cube.len == 1)
        {
            part2_sol = cube.dist;
            init_cube = cube; 
            break;
        }
        for (auto c: cube.split())
        {
            static auto pred = [&](const Nanobot &nb) { return nb.in_cube(c); } ;
            auto nn = std::count_if(nanobots.begin(), nanobots.end(), pred);
            queue.push(std::make_pair(nn, c));
        }
        //if (i > 50) break;
        i++;
    }
    std::cout << "Part 2 solution: " << part2_sol << std::endl;
    
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_pt1();
    solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
};