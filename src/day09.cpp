#include <chrono>
#include <iostream>
#include <list>
#include <array>
#include <algorithm>

class marble_set {
    public:
        std::list<uint> marble_list;
        std::list<uint>::iterator cur_pos;

        marble_set() {
            marble_list = {0};
            cur_pos = marble_list.begin();
        }
        
        int place(const uint id) {
            if (id % 23 == 0) {
                advance_pos(-7);
                auto score = id + *cur_pos;
                auto to_erase = cur_pos;
                advance_pos(1);
                marble_list.erase(to_erase);
                return score;
            } else {
                advance_pos(2);
                marble_list.insert(cur_pos, id);
                advance_pos(-1);
                return 0;
            }
        }

        void print() const {
            for (auto pos = marble_list.begin(); pos != marble_list.end(); pos++)
            {
                if (pos == cur_pos) {
                    std::cout << "(" << *pos << ") ";
                } else {
                    std::cout << *pos << " ";
                }
            }
            std::cout << std::endl;
        }
    
    private:
        void advance_pos(int n) {
            if (n > 0) {
                for (int i = 0; i < n; i++)
                {
                    if (++cur_pos == marble_list.end())
                        cur_pos = marble_list.begin();
                }
            } else if (n < 0) {
                for (int i = 0; i > n; i--)
                {
                    if (cur_pos == marble_list.begin()) {
                        cur_pos = std::prev(marble_list.end());
                    } else {
                        --cur_pos;
                    }
                }
            }
        }
};



void solve() {
    marble_set marbles;
    const uint n_players = 479;
    const uint last_value = 71035*100;
    std::array<long long, n_players> scores = {0};

    for (size_t v = 1; v <= last_value; v++)
    {
        auto player = (v - 1) % n_players;
        scores[player] += marbles.place(v);
    }

    auto max_score =  std::max_element(scores.begin(), scores.end());
    std::cout << "Part 2 solution: " << *max_score  << std::endl;

}


int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}