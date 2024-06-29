#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <regex>


struct ClayVein {
    int x_start;
    int x_end;
    int y_start;
    int y_end;
};

void fill(std::vector<char> &tiles, const int x_min, const int x_max, const int y_min, const int y_max, const int x_start, const int y_start) {
    const size_t size_x = x_max - x_min + 1;
    auto idx = [&](int x, int y) { return (x - x_min) + (y - y_min) * size_x; };
    int y = y_start;
    while (y < y_max && (tiles[idx(x_start, y + 1)] == '.' || tiles[idx(x_start, y + 1)] == '|'))
    {
        tiles[idx(x_start, y)] = '|';
        y++;
    }
    tiles[idx(x_start, y)] = '|';
    if (y == y_max) return;
    // Walk left
    int x = x_start;
    while ((tiles[idx(x - 1, y)] == '.' || tiles[idx(x - 1, y)] == '|') && (tiles[idx(x - 1, y + 1)] == '#' || tiles[idx(x - 1, y + 1)] == '~'))
        x--;

    bool left_is_closed = (tiles[idx(x - 1, y)] == '#');
    int x_left = x;
    //Walk right
    x = x_start;
    while ((tiles[idx(x + 1, y)] == '.' || tiles[idx(x + 1, y)] == '|') && (tiles[idx(x + 1, y + 1)] == '#' || tiles[idx(x + 1, y + 1)] == '~'))
        x++;

    bool right_is_closed = (tiles[idx(x + 1, y)] == '#');
    int x_right = x;
    for (int x = x_left; x <= x_right; x++)
        tiles[idx(x, y)] = (right_is_closed && left_is_closed)? '~' : '|';
    
    if (!left_is_closed)
        fill(tiles, x_min, x_max, y_min, y_max, x_left - 1, y);
    if (!right_is_closed)
        fill(tiles, x_min, x_max, y_min, y_max, x_right + 1, y);
};

void print_tiles(const std::vector<char> &tiles, const int x_min, const int x_max, const int y_min, const int y_max) {
    const size_t size_x = x_max - x_min + 1;
    for (int y = y_min; y <= y_max; y++)
    {
        for (int x = x_min; x <= x_max; x++)
        {
            std::cout << tiles[(x - x_min) + (y - y_min) * size_x];
        }
        std::cout << std::endl;
    }
}
    

void solve() {
    std::ifstream file("inputs/day17");
    std::string line;
    std::vector<ClayVein> veins;


    static std::regex line_regex("([xy])=(\\d+), ([xy])=(\\d+)\\.\\.(\\d+)");
    while (getline(file, line))
    {
        std::smatch match;
        if (std::regex_match(line, match, line_regex)) {
            if (match[1] == "x") {
                veins.push_back(ClayVein{
                    stoi(match[2]),
                    stoi(match[2]),
                    stoi(match[4]),
                    stoi(match[5]),
                });
            } else {
                veins.push_back(ClayVein{
                    stoi(match[4]),
                    stoi(match[5]),
                    stoi(match[2]),
                    stoi(match[2]),
                });
            }
        }
    }
    
    int x_max = INT_MIN;
    int x_min = INT_MAX;
    int y_max = INT_MIN;
    int y_min = INT_MAX;   
    for (const auto &v: veins) {
        x_max = std::max(x_max, v.x_end);
        x_min = std::min(x_min, v.x_start);
        y_max = std::max(y_max, v.y_end);
        y_min = std::min(y_min, v.y_start);
    }
    // Any "X" coordinate is valix
    x_max++;
    x_min--;
    size_t size_x = x_max - x_min + 1;
    size_t size_y = y_max - y_min + 1;
    std::vector<char> tiles(size_x * size_y, '.');
    for (const auto &v: veins) {
        for (int y = v.y_start; y <= v.y_end; y++)
        {
            for (int x = v.x_start; x <= v.x_end; x++)
            {
                tiles[(x - x_min) + (y - y_min) * size_x] = '#';
            }
        }
    }

    // begin the fill
    int x_source = 500;
    int y_source = y_min;
    auto prev_tiles = tiles;
    do {
        prev_tiles = tiles;
        fill(tiles, x_min, x_max, y_min, y_max, x_source, y_source);
    } while (prev_tiles != tiles);

    auto part1_solution = std::count_if(tiles.begin(), tiles.end(), [](char c) { return (c == '|' || c == '~'); });
    std::cout << "Part 1 solution: " << part1_solution << std::endl;

    auto part2_solution = std::count(tiles.begin(), tiles.end(), '~');
    std::cout << "Part 2 solution: " << part2_solution << std::endl;
   
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}