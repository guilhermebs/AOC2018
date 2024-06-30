#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>

const size_t size_x = 50;
const size_t size_y = 50;

struct NeighboursCount {
    size_t open;
    size_t trees;
    size_t lumberyard;
};

NeighboursCount count_neighbours (const std::vector<char> &area, size_t idx)
{
    NeighboursCount result{0, 0, 0};
    int y = idx/size_x;
    int x = idx%size_x;
    for (int yy = std::max(y-1, 0); yy < std::min(y+2, (int) size_y); yy++)
    {
        for (int xx = std::max(x-1, 0); xx < std::min(x+2, (int) size_x); xx++)
        {
            if (xx==x && yy==y) continue;
            switch (area[xx + yy * size_x])
            {
            case '.':
                result.open++;
                break;
            case '|':
                result.trees++;
                break;           
            case '#':
                result.lumberyard++;
                break;
            default:
                break;
            }
        }
    }
    return result;
}

std::vector<char> one_minute (const std::vector<char> &area)
{
    std::vector<char> result = area;
    for (size_t idx = 0; idx < area.size(); idx++)
    {
        auto neighbours = count_neighbours(area, idx);
        if (area[idx] == '.' && neighbours.trees >= 3) result[idx] = '|';
        else if (area[idx] == '|' && neighbours.lumberyard >= 3) result[idx] = '#';
        else if (area[idx] == '#' && (neighbours.trees == 0 || neighbours.lumberyard == 0)) result[idx] = '.';
    }
    return result;
}

int score(const std::vector<char> &area)
{
    int count_trees = std::count(area.begin(), area.end(), '|');
    int count_lumberyards = std::count(area.begin(), area.end(), '#');
    return count_trees * count_lumberyards;
}

void solve()
{
    std::ifstream file("inputs/day18");
    std::vector<char> area;
    area.reserve(size_x * size_y);
    std::string line;
    while (getline(file, line))
    {
        for (auto c: line)
        {
            area.push_back(c);
        }
    }
    auto original_area = area;

    for (size_t i = 0; i < 10; i++)
        area = one_minute(area);

    std::cout << "Part 1 solution: " << score(area) << std::endl;

    const size_t minutes = 1000000000;
    const size_t sample = 1000;
    area = original_area;
    std::vector<int> scores;
    scores.reserve(sample);

    for (size_t i = 0; i < sample; i++)
    {
        area = one_minute(area);
        scores.push_back(score(area));
    }

    auto cycle_start = scores.end() - 2;
    while (*cycle_start != scores.back())
        --cycle_start;
    auto cycle_size = scores.end() - cycle_start - 1;
    
    auto end_pos = cycle_start + (minutes - sample) % cycle_size;
    std::cout << "Part 2 solution: " << *(end_pos) << std::endl;

    
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}