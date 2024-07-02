#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
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
    std::ifstream file("inputs/day21");
    std::string line;
    getline(file, line);
    size_t ip = line[4] - '0';
    std::vector<operation> ops;
    while (getline(file, line))
        ops.push_back(read_operation(line));
    

    registers reg = {1, 0, 0, 0, 0, 0}; 
    std::unordered_set<uint> seen;
    uint prev;
    while (reg[ip] < ops.size())
    {
        auto op = ops[reg[ip]];
        auto op_name = std::get<0>(op);
        if (reg[ip] == 28)
        {
            if (seen.contains(reg[3]))
                break;
            else {
                seen.insert(reg[3]);
                prev = reg[3];
            }
        }
        //std::cout << reg[ip]  << "(" << std::get<0>(op)  << " " << std::get<1>(op)  << " " << std::get<2>(op) << " " << std::get<3>(op) <<  "):";
        //for (auto r: reg)
        //    std::cout << r << " ";
        //std::cout << std::endl;
        (functions[op_name])(reg, std::get<1>(op), std::get<2>(op), std::get<3>(op));
        reg[ip]++;
    }
    std::cout << "Part 2 solution: " << prev << std::endl;
}


int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_pt1();
    //solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}