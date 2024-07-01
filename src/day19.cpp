#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include <array>
#include <unordered_map>
#include <algorithm>

#include "helper.hpp"

using registers = std::array<uint, 6>;
using operation = std::tuple<std::string, uint, uint, uint>;
using op_func = void (*)(registers &reg, uint a, uint b, uint c);

std::unordered_map<std::string, op_func> functions = {
  {"addr", [](registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] + reg[b]; }},
  {"addi", [](registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] + b; }},
  {"mulr", [](registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] * reg[b]; }},
  {"muli", [](registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] * b; }},
  {"banr", [](registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] & reg[b]; }},
  {"bani", [](registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] & b; }},
  {"borr", [](registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] | reg[b]; }},
  {"bori", [](registers &reg, uint a, uint b, uint c) { reg[c] = reg[a] | b; }},
  {"setr", [](registers &reg, uint a, uint b, uint c) { reg[c] = reg[a]; }},
  {"seti", [](registers &reg, uint a, uint b, uint c) { reg[c] = a; }},
  {"gtir", [](registers &reg, uint a, uint b, uint c) { reg[c] = (a > reg[b])? 1 : 0; }},
  {"gtri", [](registers &reg, uint a, uint b, uint c) { reg[c] = (reg[a] > b)? 1 : 0; }},
  {"gtrr", [](registers &reg, uint a, uint b, uint c) { reg[c] = (reg[a] > reg[b])? 1 : 0; }},
  {"eqir", [](registers &reg, uint a, uint b, uint c) { reg[c] = (a == reg[b])? 1 : 0; }},
  {"eqri", [](registers &reg, uint a, uint b, uint c) { reg[c] = (reg[a] == b)? 1 : 0; }},
  {"eqrr", [](registers &reg, uint a, uint b, uint c) { reg[c] = (reg[a] == reg[b])? 1 : 0; }},
};


operation read_operation (std::string line) {
    std::vector<std::string> tokens;
    tokenize(line, " ", tokens);
    operation result = {
        tokens[0],
        stoi(tokens[1]),
        stoi(tokens[2]),
        stoi(tokens[3]),
    };
    return result;
}

std::vector<long long> trial_division3(long long n) {
    std::vector<long long> factorization;
    for (int d : {2, 3, 5}) {
        while (n % d == 0) {
            factorization.push_back(d);
            n /= d;
        }
    }
    static std::array<int, 8> increments = {4, 2, 4, 2, 4, 6, 2, 6};
    int i = 0;
    for (long long d = 7; d * d <= n; d += increments[i++]) {
        while (n % d == 0) {
            factorization.push_back(d);
            n /= d;
        }
        if (i == 8)
            i = 0;
    }
    if (n > 1)
        factorization.push_back(n);
    return factorization;
}

void solve_pt1 () {
    std::ifstream file("inputs/day19");
    std::string line;
    getline(file, line);
    size_t ip = line[4] - '0';
    std::vector<operation> ops;
    while (getline(file, line))
        ops.push_back(read_operation(line));
    

    registers reg = {0, 0, 0, 0, 0, 0}; 
    while (reg[ip] < ops.size())
    {
        auto op = ops[reg[ip]];
        auto op_name = std::get<0>(op);
        (functions[op_name])(reg, std::get<1>(op), std::get<2>(op), std::get<3>(op));
        reg[ip]++;
    }

    std::cout << "Part 1 solution: " << reg[0] << std::endl;
}

void solve_pt2 () {
    std::ifstream file("inputs/day19");
    std::string line;
    getline(file, line);
    size_t ip = line[4] - '0';
    std::vector<operation> ops;
    while (getline(file, line))
        ops.push_back(read_operation(line));

    registers reg = {1, 0, 0, 0, 0, 0}; 
    //registers reg = {0, 1, 9, 0, 10551331, 10551331};
    //registers reg = {1, 2, 9, 0, 10551331, 10551331};
    int i = 0;
    while (reg[ip] < ops.size())
    {
        auto reg_b = reg;
        auto op = ops[reg[ip]];
        auto op_name = std::get<0>(op);
        (functions[op_name])(reg, std::get<1>(op), std::get<2>(op), std::get<3>(op));
        reg[ip]++;
        i++;
        if (reg[0] != reg_b[0] || reg[1] != reg_b[1])
        {
            std::cout << i << ": ";
            std::cout << reg_b[ip]  << "(" << std::get<0>(op)  << " " << std::get<1>(op)  << " " << std::get<2>(op) << " " << std::get<3>(op) <<  "):";
            for (auto r: reg_b)
                std::cout << r << " ";
            std::cout << "-> ";
            for (auto r: reg)
                std::cout << r << " ";
            std::cout << std::endl;
        }
    }

    std::cout << "Part 2 solution: " << reg[0] << std::endl;
}



int main() {
    auto started = std::chrono::high_resolution_clock::now();
    //solve_pt1();
    //solve_pt2();
    auto factors = trial_division3(10551331);
    for (auto f: factors)
        std::cout << f << " ";
    std::cout << std::endl;
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}