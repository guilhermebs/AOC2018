#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>


void solve() {
    const uint depth = 4848;
    const uint target_x = 15;
    const uint target_y = 700;

    //const uint depth = 510;
    //const uint target_x = 10;
    //const uint target_y = 10;

    const uint gi_y0 = 16807;
    const uint gi_x0 = 48271;
    const uint el_mod = 20183;

    const uint size_x = target_x + 1;
    const uint size_y = target_y + 1;
    std::vector<uint> erosion_lvl(size_x * size_y, 0);
    std::vector<uint> cave(size_x * size_y, 0);

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
        cave[idx] = erosion_lvl[idx] % 3;
    }
    std::cout << "Part 1 solution: " << std::accumulate(cave.begin(), cave.end(), 0) << std::endl;

}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}