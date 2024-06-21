#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>


class light {
    public:
        int pos[2];
        int vel[2];
        light(std::string line) {
            pos[0] = std::stoi(line.substr(10, 6));
            pos[1] = std::stoi(line.substr(18, 6));
            vel[0] = std::stoi(line.substr(36, 2));
            vel[1] = std::stoi(line.substr(40, 2));
        }
        void evolve() {
            pos[0] += vel[0];
            pos[1] += vel[1];
        }

        void revert() {
            pos[0] -= vel[0];
            pos[1] -= vel[1];
        }
};

void print_lights(const std::vector<light> &lights) {
    int min_x = INT_MAX;
    int max_x = INT_MIN;
    int min_y = INT_MAX;
    int max_y = INT_MIN;

    for (auto l: lights) {
        min_x = std::min(min_x, l.pos[0]);
        min_y = std::min(min_y, l.pos[1]);
        max_x = std::max(max_x, l.pos[0]);
        max_y = std::max(max_y, l.pos[1]);
    }

    for(auto y = min_y; y <= max_y; y++) {
        for (auto x = min_x; x <= max_x; x++) {
            bool found = false;
            for (auto l: lights) {
                if (l.pos[0] == x && l.pos[1] == y) {
                    found = true;
                    break;
                }
            }
            if (found) {
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }

}

void solve() {
    std::ifstream file("inputs/day10");
    std::string line;
    std::vector<light> lights;
    while (getline(file, line))
    {
        lights.push_back(light(line));
    }

    std::array<long long, 2> x_bb;
    std::array<long long, 2> y_bb;
    std::array<long long, 2> prev_x_bb;
    std::array<long long, 2> prev_y_bb;
    long long prev_area = __LONG_LONG_MAX__;
    for (size_t t = 0; t < 100000; t++)
    {
        prev_x_bb = x_bb;
        prev_y_bb = y_bb;
        x_bb = {INT_MAX, INT_MIN};
        y_bb = {INT_MAX, INT_MIN};

        for (auto &l: lights) {
            l.evolve();
            x_bb[0] = std::min(x_bb[0], (long long) l.pos[0]);
            y_bb[0] = std::min(y_bb[0], (long long) l.pos[1]);
            x_bb[1] = std::max(x_bb[1], (long long) l.pos[0]);
            y_bb[1] = std::max(y_bb[1], (long long) l.pos[0]);
        }
        long long area = (x_bb[1] - x_bb[0]) * (y_bb[1] - y_bb[0]);
        if (area > prev_area)
            break;
        prev_area = area;
    }
    
    for (auto &l: lights)
        l.revert();
    
    print_lights(lights);
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}