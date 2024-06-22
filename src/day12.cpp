#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <unordered_map>

class pot_collection {
    private:
        std::string state;
        int left_pot = 0;
        std::unordered_map<std::string, char> mapping;
    public:
        pot_collection(const std::string filename) {
            std::ifstream file(filename);
            std::string line;
            getline(file, line);
            state = line.substr(15);
            getline(file, line);
            while (getline(file, line))
            {
                mapping[line.substr(0, 5)] = line[9];
            }
        }
        void print() const {
            std::cout << state << std::endl;
            std::cout << "left pot: " << left_pot << std::endl;
        }
        
        void evolve() {
            if (state.substr(0, 5).compare(".....") != 0) {
                state = "....." + state;
                left_pot -= 5;
            }

            if (state.substr(state.length()-5, 5).compare(".....") != 0)
                state = state + ".....";

            std::string new_state = state;
            for (size_t i = 2; i < state.length() - 2; i++) {
                new_state[i] = mapping[state.substr(i - 2, 5)];
            }

            state = new_state; 
        }
        
        int score() const {
            int result = 0;
            for (size_t i = 0; i < state.length(); i++) {
                if (state[i] == '#')
                    result += i + left_pot;
            }
            return result;
        }
};


void solve() {
    pot_collection collection("inputs/day12");
    collection.print();
    for (size_t i = 0; i < 20; i++) {
        collection.evolve();
        collection.print();
    }

    std::cout << "Part 1 solution: " << collection.score() << std::endl;
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}