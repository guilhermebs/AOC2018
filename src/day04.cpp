#include <fstream>
#include <regex>
#include <iostream>
#include <unordered_map>

#include "helper.hpp"

class guard {
    private:
        uint sleep_minues[60];
    public:
        guard() {
            for (int i = 0; i < 60; i++)
                sleep_minues[i] = 0;
        }

        void add_period(int start, int end) {
            for (int i = start; i < end; i++)
                sleep_minues[i]++;
        }

        uint minutes_sleep() {
            uint result = 0;
            for (int i=0; i < 60; i++) {
                result += sleep_minues[i];
            }
            return result;
        }

        uint most_frequent_sleep() {
            uint result = 0;
            uint freq = 0;
            for (int i=0; i < 60; i++) {
                if (sleep_minues[i] > freq) {
                    freq = sleep_minues[i];
                    result = i;
                }
            }
            return result;
        }

        void print() {
            for (int i=0; i < 60; i++) {
                std::cout << sleep_minues[i] << " ";
            }
            std::cout << std::endl;
        }
        
};

void solve_part1() {
    std::ifstream file("inputs/day04");
    std::string line;
    std::vector<std::string> events;

    while (getline(file, line))
        events.push_back(line);
    
    std::sort(events.begin(), events.end());

    static std::regex shift_start_pattern("\\[1518-(\\d\\d)-(\\d\\d) (\\d\\d):(\\d\\d)\\] Guard #(\\d+) begins shift");
    static std::regex falls_asleep_pattern("\\[1518-(\\d\\d)-(\\d\\d) (\\d\\d):(\\d\\d)\\] falls asleep");
    static std::regex wakes_up_pattern("\\[1518-(\\d\\d)-(\\d\\d) (\\d\\d):(\\d\\d)\\] wakes up");
    std::smatch pieces_match;
    std::unordered_map<int, guard> guards;

    int cur_guard_id = -1;
    int cur_sleep_start = -1;
    for (auto e: events) {
        if (std::regex_match(e, pieces_match, shift_start_pattern)) {
            cur_guard_id = stoi(pieces_match[5]);
            cur_sleep_start = -1;
        } else if (std::regex_match(e, pieces_match, falls_asleep_pattern))
        {
            cur_sleep_start = stoi(pieces_match[4]);
        } else if (std::regex_match(e, pieces_match, wakes_up_pattern))
        {
            if (cur_sleep_start == -1)
                throw("Sleep did not start!");
            if (!guards.contains(cur_guard_id)) {
                guards[cur_guard_id] = guard();
            }
            guards[cur_guard_id].add_period(cur_sleep_start, stoi(pieces_match[4]));
        }
    }

    int result = 0;
    uint max_sleep = 0;    
    for (auto &[key, value]: guards) {
        //std::cout << key << "," << max_sleep << std::endl;
        if (value.minutes_sleep() > max_sleep) {
            max_sleep = value.minutes_sleep();
            std::cout << key << "," << max_sleep << std::endl;
            result = key * value.most_frequent_sleep();
        }

    }
    std::cout << "Part 1 solution: " << result << std::endl;

}


int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_part1();
    //solve_part2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}