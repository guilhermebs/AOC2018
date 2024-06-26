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

        UnitTile(bool is_elve): is_elve(is_elve), health(200) {}
        bool is_empty() override { return false; }
        bool is_unit() override { return true; }
        ~UnitTile() {};
};


class Battle {
    private:
        size_t x_max;
        const std::shared_ptr<Tile> empty_ptr = std::make_shared<EmptyTyle>();
        const std::shared_ptr<Tile> wall_ptr = std::make_shared<WallTile>();
        std::vector<std::shared_ptr<Tile> > tiles;

        int shortest_path(size_t idx_start, size_t idx_end, int cutoff = INT_MAX) const {
            std::vector<uint8_t> seen(tiles.size(), 0);
            std::deque<std::pair<int, size_t>> queue = {std::make_pair(0, idx_start)};
            while (!queue.empty())
            {
                auto [nsteps, idx] = queue.front();
                std::cout << nsteps << ", " << idx << std::endl;
                queue.pop_front();
                seen[idx] = 1;
                if (nsteps >= cutoff)
                    return -1;

                auto neighbours = {idx - x_max, idx - 1, idx + 1, idx + x_max};
                for (auto n: neighbours)
                {
                    std::cout << n << std::endl;
                    if (seen[n])
                        continue;
                    if (n == idx_end)
                    {
                        return nsteps + 1;
                    } else if (tiles[n]->is_empty()) 
                    {
                        queue.push_back(std::make_pair(nsteps + 1, n));
                    }
                }
            }
            return -1;
        }

        std::tuple<int, size_t, size_t> closest_unit(size_t idx_start, bool is_elve) const {
            std::unordered_set<size_t> seen;
            std::priority_queue<
                std::tuple<int, size_t, size_t>,
                std::vector<std::tuple<int, size_t, size_t>>,
                std::greater<std::tuple<int, size_t, size_t> > > queue;
            queue.push(std::make_tuple(0, idx_start, 0));
            while (!queue.empty())
            {
                auto [nsteps, idx, dn] = queue.top();
                //std::cout << "nsteps: " << nsteps << " idx: " << idx << " dn: " << dn << std::endl;
                seen.insert(idx);
                queue.pop();
                auto neighbours = {idx - x_max, idx - 1, idx + 1, idx + x_max};
                for (auto i: neighbours) {
                    //std::cout << i << std::endl;
                    if (seen.contains(i))
                        continue;
                    if (tiles[i]->is_empty())
                    {
                        //std::cout << "here" << std::endl;
                        auto dneighbour = (nsteps == 0)? i : dn;
                        queue.push(std::make_tuple(nsteps+1, i, dneighbour));
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

        void get_unit_idx(std::vector<std::pair<size_t, std::shared_ptr<UnitTile> > > &unit_idx) const {
            unit_idx.clear();
            size_t idx = 0;
            for (auto &tile: tiles) {
                if (tile->is_unit()) {
                    unit_idx.push_back(std::make_pair(idx, std::static_pointer_cast<UnitTile>(tile)));
                }
                idx++;
            }
        }

        size_t move(size_t unit_idx) {
            std::cout << "moving: " << unit_idx;
            auto unit = std::static_pointer_cast<UnitTile>(tiles[unit_idx]);
            auto [steps, closest_idx, closest_neighbour] = closest_unit(unit_idx, !unit->is_elve);
            std::cout << " closest unit: " << closest_idx;
            std::cout << " steps: " << steps;
            std::cout << " closest_neighbour: " << closest_neighbour;
            std::cout << std::endl;
            if (steps <= 1) return unit_idx;
            //auto neighbours = {unit_idx - x_max, unit_idx - 1, unit_idx + 1, unit_idx + x_max};
            //int shortest_dist = INT_MAX;
            //size_t closest_neighbour;
            //for (auto n: neighbours) {
            //    if (tiles[n]->is_empty()) {
            //        auto dist = shortest_path(n, closest_idx, steps);
            //        std::cout <<  n << ", " << dist << std::endl;
            //        if (dist > 0 && dist < shortest_dist) {
            //            shortest_dist = dist;
            //            closest_neighbour = n;
            //        }
            //    }
            //}
            //std::cout << "closest_neighbour: " << closest_neighbour << ", dist: " << shortest_dist << std::endl;
            //if (shortest_dist == INT_MAX) throw("shoudl not be here!");
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
                    if (nunit->is_elve != unit->is_elve && nunit->health < min_health) {
                        target_idx = n;
                        min_health = nunit->health;
                        target = nunit;
                    }
                }
            }
            if (target) {
                target->health -= 3;
                if (target->health <= 0) {
                    tiles[target_idx] = empty_ptr;
                }
            }
        }


    public:
        Battle(std::string fn) {
            std::ifstream file(fn);
            std::string line;
            size_t y = 0;
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
                        tiles.push_back(std::make_shared<UnitTile>(true));
                        break;
                    case 'G':
                        tiles.push_back(std::make_shared<UnitTile>(false));
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
            std::vector<std::pair<size_t, std::shared_ptr<UnitTile> > > unit_idx;
            get_unit_idx(unit_idx);
            for (auto [idx, unit]: unit_idx)
                std::cout << idx << ", " << unit->health << std::endl;
        }


        void play_round() {
            std::vector<std::pair<size_t, std::shared_ptr<UnitTile> > > unit_idx;
            get_unit_idx(unit_idx);
            for (auto [idx, unit]: unit_idx)
            {
                if (unit->health <= 0) continue;
                auto new_idx = move(idx);
                attack(new_idx);
                //print();
            }
        }
        
        bool is_over() const {
            std::vector<std::pair<size_t, std::shared_ptr<UnitTile> > > unit_idx;
            get_unit_idx(unit_idx);
            auto pred = [](std::pair<size_t, std::shared_ptr<UnitTile>> u) {return u.second->is_elve;};
            if (std::all_of(unit_idx.begin(), unit_idx.end(), pred)) return true;
            if (std::none_of(unit_idx.begin(), unit_idx.end(), pred)) return true;
            return false;
        }

        int total_health() const {
            std::vector<std::pair<size_t, std::shared_ptr<UnitTile> > > unit_idx;
            get_unit_idx(unit_idx);
            int result = 0;
            for (auto &[_, u]: unit_idx) {
                result += u->health; 
            }
            return result;
        }

};

void solve() {
    Battle battle("inputs/day15_ex2");
    battle.print();
    int round = 0;
    battle.print();
    while (true)
    {
        std::cout << round << std::endl;
        battle.print();
        battle.play_round();
        if (battle.is_over()) break;
        round++;
    }
    battle.print();
    std::cout << "Finised after : " << round << std::endl;
    std::cout << "Part 1 solution: " << round * battle.total_health() << std::endl;
};

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}