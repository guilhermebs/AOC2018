#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include <array>

#include "helper.hpp"

using registers = std::array<uint, 4>;
using operation = std::array<uint, 4>;

void addr(registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] + reg[b]; }
void addi(registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] + b; }
void mulr(registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] * reg[b]; }
void muli(registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] * b; }
void banr(registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] & reg[b]; }
void bani(registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] & b; }
void borr(registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] | reg[b]; }
void bori(registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] | b; }
void setr(registers &reg, uint a, uint b, uint c) { reg[c] = reg[a]; }
void seti(registers &reg, uint a, uint b, uint c) { reg[c] = a; }
void gtir(registers &reg, uint a, uint b, uint c) { reg[c] = (a > reg[b])? 1 : 0; }
void gtri(registers &reg, uint a, uint b, uint c) { reg[c] = (reg[a] > b)? 1 : 0; }
void gtrr(registers &reg, uint a, uint b, uint c) { reg[c] = (reg[a] > reg[b])? 1 : 0; }
void eqir(registers &reg, uint a, uint b, uint c) { reg[c] = (a == reg[b])? 1 : 0; }
void eqri(registers &reg, uint a, uint b, uint c) { reg[c] = (reg[a] == b)? 1 : 0; }
void eqrr(registers &reg, uint a, uint b, uint c) { reg[c] = (reg[a] == reg[b])? 1 : 0; }

registers read_registers (std::string line) {
    registers result = {
        static_cast<uint>(line[9] - '0'),
        static_cast<uint>(line[12] - '0'),
        static_cast<uint>(line[15] - '0'),
        static_cast<uint>(line[18] - '0')}; 
    return result; 
}

operation read_operation (std::string line) {
    size_t pos = 0;
    size_t next_token = 0;
    operation result;
    size_t i = 0;
    do {
        next_token = line.find(" ", pos);
        result[i] = stoi(line.substr(pos, next_token - pos));
        pos = next_token + 1; 
        i++;
    } while (next_token != std::string::npos);
    return result;
}

void solve_pt1 () {
    std::ifstream file("inputs/day16");
    std::stringstream buffer;
    std::vector<std::string> observations;
    buffer << file.rdbuf();
    tokenize(buffer.str(), "\n\n", observations);

    std::vector<void (*)(registers &reg, uint a, uint b, uint c) > operations = {
        &addr,
        &addi,
        &mulr,
        &muli,
        &banr,
        &bani,
        &borr,
        &bori,
        &setr,
        &seti,
        &gtir,
        &gtri,
        &gtrr,
        &eqir,
        &eqri,
        &eqrr,
    };

    size_t part1_sol = 0;
    size_t n = 0;
    for (auto s:observations) {
        if (s.length() == 0) break;
        n++;
        std::vector<std::string> lines;
        tokenize(s, "\n", lines);
        auto before = read_registers(lines[0]);
        auto input = read_operation(lines[1]);
        auto after = read_registers(lines[2]);
        size_t n_valid = 0;
        for (auto op: operations) {
            auto reg = before;
            (*op)(reg, input[1], input[2], input[3]);
            if (reg == after) n_valid++;
        }
        part1_sol += (n_valid >= 3);
    }
    std::cout << "Part 1 solution: " << part1_sol << std::endl;
};

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_pt1();
    //solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}