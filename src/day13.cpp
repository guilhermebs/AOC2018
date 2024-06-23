#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

enum turn {
    LEFT,
    RIGHT,
    STRAIGHT,
};

class cart {
    private:
        char dir;
        turn next_turn;
    
        void turn_left() {
            switch (dir)
            {
            case '>':
                dir = '^';
                break;
            case '<':
                dir = 'v';
                break;
            case '^':
                dir = '<';
                break;
            case 'v':
                dir = '>';
                break;
            default:
                break;
            }
        }

        void turn_right() {
            switch (dir)
            {
            case '>':
                dir = 'v';
                break;
            case '<':
                dir = '^';
                break;
            case '^':
                dir = '>';
                break;
            case 'v':
                dir = '<';
                break;
            default:
                break;
            }
        }

    public:
        size_t x;
        size_t y;

        cart(size_t x, size_t y, char dir): dir(dir), next_turn(LEFT), x(x), y(y)  {}

        void print() {
            std::cout << x << ", " << y << " " << dir << std::endl;
        }

        bool operator<(const cart &obj) const {
            if (y == obj.y)
                return x < obj.x;
            return y < obj.y; 
        }

        void move(const std::vector<std::string> &map) {
            switch (dir)
            {
                case '>':
                    x++;
                    break;
                case '<':
                    x--;
                    break;
                case '^':
                    y--;
                    break;
                case 'v':
                    y++;
                    break;
                default:
                    throw("Unexpected direction!");
            }

            switch (map[y][x])
            {
                case '/':
                    switch (dir)
                    {
                        case '>':
                            dir = '^';
                            break;
                        case '<':
                            dir = 'v';
                            break;
                        case '^':
                            dir = '>';
                            break;
                        case 'v':
                            dir = '<';
                            break;
                    }
                    break;

                case '\\':
                    switch (dir)
                    {
                        case '>':
                            dir = 'v';
                            break;
                        case '<':
                            dir = '^';
                            break;
                        case '^':
                            dir = '<';
                            break;
                        case 'v':
                            dir = '>';
                            break;
                    }
                    break;

                case '+': 
                    switch (next_turn)
                    {
                    case LEFT:
                        turn_left();
                        next_turn = STRAIGHT;
                        break;
                    case STRAIGHT:
                        next_turn = RIGHT;
                        break;
                    case RIGHT:
                        turn_right();
                        next_turn = LEFT;
                    default:
                        break;
                    }
                case '-':
                    break;
                case '|':
                    break;
                default:
                    throw("Invalid char in map!");
            }
        }
};

bool crash_location(const std::vector<cart> &carts, size_t &crash_i, size_t &crash_j) {
    bool result = false;
    for (size_t i = 0; i < carts.size(); i++)
    {
        for (size_t j = i + 1; j < carts.size(); j++)
        {
            if ((carts[i].x == carts[j].x) && (carts[i].y == carts[j].y)) {
                crash_i = i;
                crash_j = j;
                return true;
            }
        }
    }
    return result;
}


void solve() {
    std::fstream file("inputs/day13");
    std::string line;
    std::vector<std::string> map;
    std::vector<cart> carts;
    
    size_t y = 0;
    while (getline(file, line))
    {
        size_t x = 0;
        for (auto &c: line) {
            if (c == '>' || c == '<') {
                carts.push_back(cart(x, y, c));
                c = '-';
            } else if (c == '^' || c == 'v') {
                carts.push_back(cart(x, y, c));
                c = '|';
            }
            x++;
        }
        map.push_back(line);
        y++;
    }
    size_t crash_i;
    size_t crash_j;
    while (!crash_location(carts, crash_i, crash_j))
    {
        std::sort(carts.begin(), carts.end());
        for (auto &c: carts) {
            c.move(map);
        }
    }
    std::cout << "Part 1 result: " << carts[crash_i].x << "," << carts[crash_j].y << std::endl;
    while (crash_location(carts, crash_i, crash_j)) {
        carts.erase(carts.begin() + crash_j);
        carts.erase(carts.begin() + crash_i);
    }
    while (carts.size() > 1)
    {
        std::sort(carts.begin(), carts.end());
        std::vector<size_t> to_remove;
        for (size_t i = 0; i < carts.size(); i++)
        {
            carts[i].move(map);
            for (size_t j = 0; j < carts.size(); j++) {
                if ((i != j) && (carts[i].x == carts[j].x) && (carts[i].y == carts[j].y)) {
                    to_remove.push_back(i);
                    to_remove.push_back(j);
                }
            }
        }
        std::sort(to_remove.begin(), to_remove.end(), std::greater<>());
        std::unique(to_remove.begin(), to_remove.end());
        for (auto i: to_remove) {
            carts.erase(carts.begin() + i);
        }
    };

    std::cout << "Part 2 result: " << carts[0].x << "," << carts[0].y << std::endl;
    
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}