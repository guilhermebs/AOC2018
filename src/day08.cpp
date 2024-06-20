#include <fstream>
#include <iostream>
#include "helper.hpp"

class node {
    private:
        std::vector<node> children;
        std::vector<int> metadata;

    public:
        node(std::vector<int>::iterator &input) {
            size_t n_children = *(input++);
            size_t n_metadata = *(input++);
            children.reserve(n_children);
            metadata.reserve(n_metadata);
            for (size_t i = 0; i < n_children; i++)
            {
                children.push_back(node(input));
            }
            for (size_t i = 0; i < n_metadata; i++)
                metadata.push_back(*(input++));
            
        }
    
        int sum_metadata() const {
            int result = 0;
            for (auto m: metadata)
                result += m;

            for (auto c: children)
                result += c.sum_metadata();
            return result;
        }

        int value() const {
            int result = 0;
            if (children.size() == 0) {
                for (auto m: metadata)
                    result += m;
            } else {
                for (auto m: metadata) {
                    if (m > 0 && (size_t) m <= children.size())
                        result += children[m - 1].value();
                }
            }
            return result;
        }
};


void solve() {
    std::ifstream file("inputs/day08");
    std::string line;
    getline(file, line);
    std::vector<std::string> split;
    tokenize(line, " ", split);


    std::vector<int> input;
    input.reserve(split.size());

    for (auto n: split)
        input.push_back(stoi(n));

    std::vector<int>::iterator iter = input.begin();
    node root(iter);

    std::cout << "Part 1 solution: " << root.sum_metadata() << std::endl;
    std::cout << "Part 2 solution: " << root.value() << std::endl;
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}