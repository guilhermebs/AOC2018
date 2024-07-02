#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <algorithm>

#include "helper.hpp"

using coords_t = std::tuple<int, int>;
using map_t = std::unordered_map<coords_t, std::unordered_set<coords_t>>;

coords_t change_coords(const coords_t &coords, char dir)
{
    coords_t result = coords;
    switch (dir)
    {
    case 'N':
        std::get<1>(result)++;
        break;
    case 'E':
        std::get<0>(result)++;   
        break;
    case 'S':
        std::get<1>(result)--;
        break;
    case 'W':
        std::get<0>(result)--;
        break;
    default:
        throw std::runtime_error("Invalid character!");
        break;
    }
    return result;
}

coords_t create_room_map(const std::string &str, const coords_t &origin, map_t &room_map)
{
    if (str.length() == 0) return origin;
    coords_t coords = origin;
    size_t level = 0;
    std::vector<size_t> next_level_marks;
    for (size_t i = 0; i < str.length(); i++)
    {
        auto c = str[i];
        if (c == '(')
        {
            if (++level == 1)
                next_level_marks.push_back(i);
        }
        else if (level == 1 && c == '|')
        {
            next_level_marks.push_back(i);
        }
        else if (c == ')')
        {
            if (--level == 0)
            {
                next_level_marks.push_back(i);
                break;
            }
        }
        else if (level == 0)
        {
            auto new_coords = change_coords(coords, c);
            room_map[coords].insert(new_coords);
            room_map[new_coords].insert(coords);
            coords = new_coords;
        }
    }
    if (next_level_marks.size() == 0) return coords;
    std::unordered_set<coords_t> next_c;
    for (size_t i = 0; i < next_level_marks.size() - 1; i++)
    {
        auto start = next_level_marks[i] + 1;
        auto len = next_level_marks[i+1] - next_level_marks[i] - 1;
        auto new_str = str.substr(start, len);
        next_c.insert(create_room_map(new_str, coords, room_map));
    }
    auto remaining = str.substr(next_level_marks.back() + 1);
    std::cout << next_c.size() << std::endl;
    for (auto c: next_c)
        coords = create_room_map(remaining, c, room_map);
    return coords;
}

std::vector<coords_t> room_coords (const map_t &room_map)
{
    std::vector<coords_t> coords;
    for (auto &[k, _]: room_map)
        coords.push_back(k);

    std::sort(coords.begin(), coords.end(), [](coords_t a, coords_t b) {
        return (std::get<1>(a) == std::get<1>(b))?(std::get<0>(a) < std::get<0>(b)):(std::get<1>(a) > std::get<1>(b));
    });

    return coords;
}



int shortest_dist (const map_t &room_map, const coords_t &start, const coords_t &end)
{
    std::deque<std::pair<int, coords_t>> queue = {{0, start}};
    std::unordered_set<coords_t> visited = {start};
    while (queue.size() > 0)
    {
        auto &[dist, c] = queue.front();
        queue.pop_front();
        if (c == end) return dist;
        for (auto &nc: room_map.at(c))
        {
            if (visited.contains(nc)) continue;
            visited.insert(nc);
            queue.push_back(std::make_pair(dist + 1, nc));
        }
    }
    return -1;
}


void solve()
{
    std::ifstream file("inputs/day20");
    std::string line;
    getline(file, line);
    //line = "^ENWWW(NEEE|SSE(EE|N))$";
    //line = "^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$";
    auto regex = line.substr(1, line.size() - 2);
    map_t room_map;
    coords_t origin = std::make_tuple(0, 0);
    create_room_map(regex, origin, room_map);
    std::cout << "Room map done" << std::endl;

    std::deque<std::pair<int, coords_t>> queue = {{1, origin}};
    std::unordered_set<coords_t> visited = {origin};
    int part1_sol = 0;
    int part2_sol = 0;
    while (visited.size() < room_map.size()) {
        auto &[dist, c] = queue.front();
        queue.pop_front();
        part1_sol = std::max(dist, part1_sol);
        if (dist > 1000) part2_sol++;
        for (auto nc: room_map[c])
        {
            if (visited.contains(nc)) continue;
            visited.insert(nc);
            queue.push_back({dist + 1, nc});
        }
    }
    std::cout << "Part 1 solution: " << part1_sol << std::endl;
    std::cout << "Part 2 solution: " << part2_sol << std::endl;
    //auto coords = room_coords(room_map);
    //coords_t most_distant;
    //int max_distance = 0;
    //for (auto &c: coords)
    //{
    //    auto dist = abs(std::get<0>(c)) + abs(std::get<1>(c));
    //    std::cout << std::get<0>(c) << ", " << std::get<1>(c) << ": ";
    //    for (auto &nc: room_map[c])
    //        std::cout << "(" << std::get<0>(nc) << ", " << std::get<1>(nc) << "), ";
    //    std::cout << std::endl;
    //    if (dist > max_distance)
    //    {
    //        max_distance = dist;
    //        most_distant = c;
    //    }
    //}
    //std::cout << std::get<0>(most_distant) << ", " << std::get<1>(most_distant) << std::endl;
    //auto dist = shortest_dist(room_map, origin, most_distant);
    //std::cout << dist << std::endl;
}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}