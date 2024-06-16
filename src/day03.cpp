#include <fstream>
#include <regex>
#include <iostream>

struct point
{
    int x;
    int y;
};


class claim {
    public:
        int id;
        point top_left;
        point bottom_right;

        claim() {
        };

        claim(const std::string &line) {
            static std::regex pattern("#([0-9]+) @ ([0-9]+),([0-9]+): ([0-9]+)x([0-9]+)");
            std::smatch pieces_match;
            if (std::regex_match(line, pieces_match, pattern)) {
                id = std::stoi(pieces_match[1]);
                top_left.x = std::stoi(pieces_match[2]);
                top_left.y = std::stoi(pieces_match[3]);
                bottom_right.x = std::stoi(pieces_match[4]) + top_left.x;
                bottom_right.y = std::stoi(pieces_match[5]) + top_left.y;
            }
        }

        bool overlaps(const claim &other) const {

            if (other.top_left.x >= bottom_right.x || other.bottom_right.x <= top_left.x)
                return false;

            if (other.top_left.y >= bottom_right.y || other.bottom_right.y <= top_left.y)
                return false;
           return true;
        }

};


void solve_part1() {
    std::ifstream file("inputs/day03");
    std::string line;
    std::vector<claim> claims;

    int max_x = 0;
    int max_y = 0;

    while (getline(file, line)) {
        claim c(line);
        claims.push_back(c);
        max_x = std::max(max_x, c.bottom_right.x + 1);
        max_y = std::max(max_y, c.bottom_right.y + 1);
    }
    std::vector<int> n_claims(max_x * max_y, 0);
    for (auto c: claims) {
        for (int x=c.top_left.x; x < c.bottom_right.x; x++) {
            for (int y=c.top_left.y; y < c.bottom_right.y; y++)
                n_claims[x + y*max_x]++;
        }
    }
    
    int overlap_area = 0;
    for (auto n: n_claims) {
        if (n > 1)
            overlap_area++;
    }

    std::cout << "Part 1 solution: " << overlap_area << std::endl;
    //int overlap_area = 0;
    //std::vector<claim> overlap;
    int sol_pt2 = 0;
    for (auto ci: claims)
    {
        bool no_overlaps = true;
        for (auto cj: claims)
        {
            if (ci.id != cj.id && ci.overlaps(cj)) {
                no_overlaps = false;
                break;
            }
        }
        if (no_overlaps) {
            sol_pt2 = ci.id;
        }
    }
    std::cout << "Part 2 solution: " << sol_pt2 << std::endl;
    
}


int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_part1();
    //solve_part2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}