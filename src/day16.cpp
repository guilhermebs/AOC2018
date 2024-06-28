#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include <array>
#include <set>
#include <algorithm>

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

struct Observation {
    registers before;
    operation operation;
    registers after;
};

void solve_pt1 () {
    std::ifstream file("inputs/day16");
    std::stringstream buffer;
    std::vector<std::string> observations_str;
    std::vector<Observation> observations;
    buffer << file.rdbuf();
    tokenize(buffer.str(), "\n\n", observations_str);
    for (auto s:observations_str) {
        if (s.length() == 0) break;
        std::vector<std::string> lines;
        tokenize(s, "\n", lines);
        observations.push_back(Observation {
            read_registers(lines[0]),
            read_operation(lines[1]),
            read_registers(lines[2]),
        });
    }
 
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
    for (auto obs:observations) {
        size_t n_valid = 0;
        for (auto op: operations) {
            auto reg = obs.before;
            (*op)(reg, obs.operation[1], obs.operation[2], obs.operation[3]);
            if (reg == obs.after) n_valid++;
        }
        part1_sol += (n_valid >= 3);
    }
    std::cout << "Part 1 solution: " << part1_sol << std::endl;

    std::vector<std::set<size_t>> possible_op_numbers(operations.size());
    size_t op_i = 0;
    for (auto op: operations) {
        bool fits_all;
        for (size_t i = 0; i < operations.size(); i++) {
            fits_all = true;
            for (auto obs: observations) {
                if (obs.operation[0] != i) continue;
                auto reg = obs.before;
                (*op)(reg, obs.operation[1], obs.operation[2], obs.operation[3]);
                if (reg != obs.after)  {
                    fits_all = false;
                    break;
                }
            }
            if (fits_all) {
                possible_op_numbers[op_i].insert(i);
            }
        }
        op_i++;
    }
    op_i = 0;
    for (const auto &fits: possible_op_numbers) {
        std::cout << op_i++ << ": ";
        for (const auto &i: fits )
            std::cout << i << " ";
        std::cout << std::endl;
    }

    std::vector<size_t> op_numbers(operations.size());
    bool modified = false;
    do {
        modified = false;
        for (size_t opi = 0; opi < possible_op_numbers.size(); opi++) {
            if (possible_op_numbers[opi].size() == 1) {
                modified = true;
                auto n = *(possible_op_numbers[opi].begin());
                //op_numbers[opi] = m;
                op_numbers[n] = opi;
                for (auto &s: possible_op_numbers)
                    s.erase(n);
            }
        }
    } while (modified);

    for (auto n: op_numbers)
        std::cout << n << " ";
    std::cout << std::endl;

    // Check
    for (auto obs: observations) {
        auto reg = obs.before;
        (*operations[op_numbers[obs.operation[0]]])(reg, obs.operation[1], obs.operation[2], obs.operation[3]);
        if (reg != obs.after)  {
            std::cout << "Problem with op number: " << obs.operation[0] << std::endl;
        }
    }
    
    // Run!
    std::vector<std::string> test_program_str;
    tokenize(*(observations_str.end() - 1), "\n", test_program_str);
    registers reg = {0, 0, 0, 0};
    for (auto &line: test_program_str) {
        if (line == "") continue;
        auto op = read_operation(line);
        (*operations[op_numbers[op[0]]])(reg, op[1], op[2], op[3]);
    }

    std::cout << "Part 2 solution: " << reg[0] << std::endl;;

};

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_pt1();
    //solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}