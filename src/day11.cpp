#include <chrono>
#include <iostream>
#include <array>

void solve() {
    const int input = 8199;
    const int size = 300;

    std::array<int, size*size> cells;
    for (size_t x = 0; x < size; x++) {
        long long rack_id = (x + 11);
        for (size_t y = 0; y < size; y++) {
            long long power_level = rack_id * (y + 1);
            power_level += input;
            power_level *= rack_id;
            power_level = (power_level / 100) % 10;
            power_level -= 5;
            cells[x + size * y] = power_level;
        }
    }
    size_t max_x = 0;
    size_t max_y = 0;
    int max_fuel = INT_MIN;
    for (size_t y = 0; y < size - 3; y++) {
        for (size_t x = 0; x < size - 3; x++) {
            int sum = 0;
            for (size_t yy = y; yy < y + 3; yy++) {
                for (size_t xx = x; xx < x + 3; xx++) {
                    sum += cells[xx + size * yy];
                }
            }
            if (sum > max_fuel) {
                max_fuel = sum;
                max_x = x;
                max_y = y;
            }
        }
    }
    std::cout << max_fuel << std::endl;
    std::cout << "Part 1 solution: " << max_x + 1 << "," << max_y + 1 << std::endl;

    // Part 2
    max_x = 0;
    max_y = 0;
    size_t max_size = 0;
    max_fuel = INT_MIN;
    for (size_t y = 0; y < size; y++) {
        for (size_t x = 0; x < size; x++) {
            size_t max_allowed_size = std::min(size - x, size - y) - 1;
            for (size_t s = 1; s <= max_allowed_size; s++) {
                int sum = 0;
                for (size_t yy = y; yy < y + s; yy++) {
                    for (size_t xx = x; xx < x + s; xx++) {
                        sum += cells[xx + size * yy];
                    }
                }
                if (sum > max_fuel) {
                    max_fuel = sum;
                    max_x = x;
                    max_y = y;
                    max_size = s;
                }
            }
        }
    }
    std::cout << max_fuel << std::endl;
    std::cout << "Part 2 solution: " << max_x + 1 << "," << max_y + 1 << "," << max_size << std::endl;
 
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}