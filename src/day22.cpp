#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <stdexcept>

#include "helper.hpp"

const uint depth = 4848;
const uint target_x = 15;
const uint target_y = 700;

//const uint depth = 510;
//const uint target_x = 10;
//const uint target_y = 10;

const uint gi_y0 = 16807;
const uint gi_x0 = 48271;
const uint el_mod = 20183;

enum Tool {
    neither,
    torch,
    climb,
};

enum Region {
    rocky,
    wet,
    narrow,
};


void solve_pt1() {
    const uint size_x = target_x + 1;
    const uint size_y = target_y + 1;
    std::vector<uint> erosion_lvl(size_x * size_y, 0);
    std::vector<Region> cave(size_x * size_y);

    for (size_t idx = 0; idx < cave.size(); idx++)
    {
        auto x = idx%size_x;
        auto y = idx/size_x;
        uint gi;
        if ((x == 0 && y == 0) || (x == target_x && y == target_y))
            gi = 0;
        else if (x == 0 && y != 0)
            gi = gi_x0 * y;
        else if (y == 0 && x != 0)
            gi = gi_y0 * x;
        else
            gi = erosion_lvl[x-1 + y * size_x] * erosion_lvl[x + (y-1) * size_x];

        erosion_lvl[idx] = (gi + depth) % el_mod;
        cave[idx] = Region(erosion_lvl[idx] % 3);
    }
    std::cout << "Part 1 solution: " << std::accumulate(cave.begin(), cave.end(), 0) << std::endl;

}

using State = std::tuple<int, int, size_t, Tool>;

std::vector<State> get_neighbours(const std::vector<Region> &cave, size_t size_x, const State &state)
{
    auto [_, t, idx, tool] = state;
    size_t size_y = cave.size() / size_x;
    size_t x = idx % size_x;
    size_t y = idx / size_x;

    if (x == target_x && y == target_y && tool != Tool::torch)
        return {{t + 7, t + 7, idx, Tool::torch}};

    if (x == size_x - 1 || y == size_y - 1)
        throw std::runtime_error("value oob!");
    
    std::vector<size_t> adjacent = {idx + size_x, idx + 1};
    if (y > 0) adjacent.push_back(idx - size_x);
    if (x > 0) adjacent.push_back(idx - 1);
    std::vector<State> result;
    for (auto a: adjacent)
    {
        auto c = cave[a];
        int xa = a % size_x;
        int ya = a / size_x;
        auto h = abs(xa - (int) target_x) + abs(ya - (int) target_y);
        if ((int) tool != (int) c)
            result.push_back({t + 1 + h, t + 1, a, tool});
        else
        {
            auto new_tool = ((tool + 1) % 3 != cave[idx])? Tool((tool + 1)%3) : Tool((tool + 2) % 3);
            result.push_back({t + 8 + h, t + 8, a, new_tool});
        }
    }
    return result;
}


void solve_pt2() {
    const uint size_x = target_x * 10;
    const uint size_y = target_y * 10;
    std::vector<uint> erosion_lvl(size_x * size_y, 0);
    std::vector<Region> cave(size_x * size_y);

    for (size_t idx = 0; idx < cave.size(); idx++)
    {
        auto x = idx%size_x;
        auto y = idx/size_x;
        uint gi;
        if ((x == 0 && y == 0) || (x == target_x && y == target_y))
            gi = 0;
        else if (x == 0 && y != 0)
            gi = gi_x0 * y;
        else if (y == 0 && x != 0)
            gi = gi_y0 * x;
        else
            gi = erosion_lvl[x-1 + y * size_x] * erosion_lvl[x + (y-1) * size_x];

        erosion_lvl[idx] = (gi + depth) % el_mod;
        cave[idx] = Region(erosion_lvl[idx] % 3);
    }

    std::priority_queue<State , std::vector<State>, std::greater<State>> queue;
    queue.push(std::make_tuple(target_x + target_y, 0, 0, Tool::torch));
    const size_t target_id = target_x + target_y * size_x;
    const Tool target_tool = Tool::torch;
    std::unordered_map<std::tuple<size_t, Tool>, int> seen;
    int part2_solution = 0;
    int i = 0;
    while (queue.size() > 0)
    {
        auto state = queue.top();
        auto [_, t, id, tool] = state;
        queue.pop();
        if (id == target_id && tool == target_tool)
        {
            part2_solution = t;
            break;
        }
        for (auto n: get_neighbours(cave, size_x, state))
        {
            auto [_, tn, idn, tooln] = n;
            std::tuple<size_t, Tool> sn{idn, tooln};
            if (!seen.contains(sn) || seen[sn] > tn)
            {
                queue.push(n);
                seen[sn] = tn;
            }
        }
        i++;
    }
    std::cout << "Part 2 solution: " << part2_solution << std::endl; 
    
    
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_pt1();
    solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}