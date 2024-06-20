#include <fstream>
#include <iostream>
#include "helper.hpp"

class node {
    public:
        std::vector<node> children;
        std::vector<int> metadata;
    
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

size_t create_node(const std::vector<int> &input, const size_t start, node &out) {
    auto pos = start;
    size_t n_children = input[pos++];
    size_t n_metadata = input[pos++];
    out.children.reserve(n_children);
    out.metadata.reserve(n_metadata);
    for (size_t i = 0; i < n_children; i++)
    {
        node child;
        pos = create_node(input, pos, child);
        out.children.push_back(child);
    }

    for (size_t i = 0; i < n_metadata; i++)
        out.metadata.push_back(input[pos++]);
    
    return pos;
}


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

    node root;
    create_node(input, 0, root);


    std::cout << "Part 1 solution: " << root.sum_metadata() << std::endl;
    std::cout << "Part 2 solution: " << root.value() << std::endl;
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}