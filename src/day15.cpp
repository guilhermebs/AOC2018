#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <deque>
#include <queue>
#include <memory>
#include <chrono>


class Tile {
    public:
        virtual bool is_empty() = 0;
        virtual bool is_unit() = 0;
        virtual ~Tile() = default;
};

class EmptyTyle: public Tile {
    public:
        EmptyTyle() {}
        bool is_empty() override { return true; }
        bool is_unit() override { return false; }
        ~EmptyTyle() = default;

};

class WallTile: public Tile {
    public:
        WallTile() {}
        bool is_empty() override { return false; }
        bool is_unit() override { return false; }
        ~WallTile() override {};
};

class UnitTile: public Tile {
    public:
        bool is_elve;
        int health;
        int attack_pwr;

        UnitTile(bool is_elve, int attack): is_elve(is_elve), health(200), attack_pwr(attack) {}
        bool is_empty() override { return false; }
        bool is_unit() override { return true; }
        bool attack(UnitTile & other) { 
            other.health -= attack_pwr;
            return other.health <= 0;
        }
        ~UnitTile() {};
};


class Battle {
    private:
        size_t x_max;
        const std::shared_ptr<Tile> empty_ptr = std::make_shared<EmptyTyle>();
        const std::shared_ptr<Tile> wall_ptr = std::make_shared<WallTile>();
        size_t init_elves;
        std::vector<std::shared_ptr<Tile> > tiles;

        std::tuple<int, size_t, size_t> closest_unit(size_t idx_start, bool is_elve) const {
            std::vector<uint8_t> seen(tiles.size(), 0);
            std::deque<std::tuple<int, size_t, size_t>> queue;
            queue.push_back(std::make_tuple(0, idx_start, 0));
            seen[idx_start] = 1;
            while (!queue.empty())
            {
                auto [nsteps, idx, dn] = queue.front();
                queue.pop_front();
                auto neighbours = {idx - x_max, idx - 1, idx + 1, idx + x_max};
                for (auto i: neighbours) {
                    if (seen[i])
                        continue;
                    else
                        seen[i] = 1;
                    if (tiles[i]->is_empty())
                    {
                        auto dneighbour = (nsteps == 0)? i : dn;
                        queue.push_back(std::make_tuple(nsteps+1, i, dneighbour));
                    }
                    else if (tiles[i]->is_unit())
                    {
                        auto unit = std::static_pointer_cast<UnitTile>(tiles[i]);
                        if (unit->is_elve == is_elve)
                            return std::make_tuple(nsteps+1, i, dn);
                    }
                }
            }
            return std::make_tuple(-1, 0, 0);
        }

        std::vector<std::pair<size_t, std::shared_ptr<UnitTile>>> get_unit_idx() const {
            std::vector<std::pair<size_t, std::shared_ptr<UnitTile>>> result;
            size_t idx = 0;
            for (auto &tile: tiles) {
                if (tile->is_unit()) {
                    result.push_back(std::make_pair(idx, std::static_pointer_cast<UnitTile>(tile)));
                }
                idx++;
            }
            return result;
        }

        size_t move(size_t unit_idx) {
            auto unit = std::static_pointer_cast<UnitTile>(tiles[unit_idx]);
            auto [steps, closest_idx, closest_neighbour] = closest_unit(unit_idx, !unit->is_elve);
            if (steps <= 1) return unit_idx;
            tiles[unit_idx] = empty_ptr;
            tiles[closest_neighbour] = unit;
            return closest_neighbour;
        }

        void attack (size_t unit_idx) {
            auto unit = std::static_pointer_cast<UnitTile>(tiles[unit_idx]);
            auto neighbours = {unit_idx - x_max, unit_idx - 1, unit_idx + 1, unit_idx + x_max};
            std::shared_ptr<UnitTile> target;
            size_t target_idx;
            int min_health = INT_MAX;
            for (auto n: neighbours) {
                if (tiles[n] -> is_unit()) {
                    auto nunit = std::static_pointer_cast<UnitTile>(tiles[n]);
                    if (nunit->is_elve != unit->is_elve && nunit->health < min_health && nunit-> health > 0) {
                        target_idx = n;
                        min_health = nunit->health;
                        target = nunit;
                    }
                }
            }
            if (target) {
                if (unit->attack(*target)) {
                    tiles[target_idx] = empty_ptr;
                }
            }
        }


    public:
        Battle(std::string fn, int elve_atk, int goblin_atk) {
            std::ifstream file(fn);
            std::string line;
            size_t y = 0;
            init_elves = 0;
            while (getline(file, line))
            {
                x_max = line.size();
                size_t x = 0;
                for (int c: line) {
                    switch (c)
                    {
                    case '#':
                        tiles.push_back(wall_ptr);
                        break;
                    case '.':
                        tiles.push_back(empty_ptr);
                        break;
                    case 'E':
                        init_elves++;
                        tiles.push_back(std::make_shared<UnitTile>(true, elve_atk));
                        break;
                    case 'G':
                        tiles.push_back(std::make_shared<UnitTile>(false, goblin_atk));
                        break;
                    default:
                        throw("Invalid character in input!");
                    }
                    x++;
                }
                y++;
            }
        }
        
        void print() const {
            size_t idx = 0;
            for (auto &tile: tiles) {
                if (tile->is_empty()) {
                    std::cout << '.';
                } else if (tile->is_unit())
                {
                    auto unit = std::static_pointer_cast<UnitTile>(tile);
                    if (unit->is_elve)
                    {
                        std::cout << 'E';
                    }
                    else
                    {
                        std::cout << 'G';
                    }
                } else
                {
                    std::cout << "#";
                }
                if ((++idx) % x_max == 0)
                    std::cout << std::endl;
            }
            auto unit_idx = get_unit_idx();
            for (auto [idx, unit]: unit_idx)
                std::cout << idx << ", " << unit->health << std::endl;
        }


        bool play_round() {
            auto unit_idx = get_unit_idx();
            for (auto [idx, unit]: unit_idx)
            {
                if (is_over()) return false;
                if (unit->health <= 0) continue;
                auto new_idx = move(idx);
                attack(new_idx);
            }
            return true;
        }
        
        int is_over() const {
            auto unit_idx = get_unit_idx();
            auto pred = [](std::pair<size_t, std::shared_ptr<UnitTile>> u) {return u.second->is_elve;};
            if (std::all_of(unit_idx.begin(), unit_idx.end(), pred)) return true;
            if (std::none_of(unit_idx.begin(), unit_idx.end(), pred)) return true;
            return false;
        }

        int total_health() const {
            auto unit_idx = get_unit_idx();
            int result = 0;
            for (auto &[_, u]: unit_idx) {
                result += u->health; 
            }
            return result;
        }

        bool is_total_elve_victory() const {
            auto unit_idx = get_unit_idx();
            auto pred = [](std::pair<size_t, std::shared_ptr<UnitTile>> u) {return u.second->is_elve;};
            size_t elve_count = std::count_if(unit_idx.begin(), unit_idx.end(), pred);
            return (elve_count == init_elves);
        }

};


void solve_pt1() {
    Battle battle("inputs/day15", 3, 3);
    int round = 0;
    while (!battle.is_over()) {
        round++;
        if(!battle.play_round())
            round--;
    }
    std::cout << "Finised after : " << round << std::endl;
    std::cout << "Part 1 solution: " << round * battle.total_health() << std::endl;
};

void solve_pt2() {
    int a = 3;
    int outcome;
    while (true)
    {
        a++;
        Battle battle("inputs/day15", a, 3);
        int round = 0;
        while (!battle.is_over()) {
            round++;
            if(!battle.play_round())
                round--;
        }
        std::cout << "Attack: " << a << std::endl;
        std::cout << "Finised after : " << round << std::endl;
        std::cout << "Total health : " << battle.total_health() << std::endl;
        battle.print();
        if (battle.is_total_elve_victory()) {
            outcome = round * battle.total_health();
            break;
        }
    }
    std::cout << "Part 2 solution: " << outcome  << std::endl;
};

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    //solve_pt1();
    solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}