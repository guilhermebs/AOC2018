#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <tuple>
#include <array>
#include <algorithm>
#include <numeric>

class Group
{
    public:
        std::string name;
        bool is_infection;
        int units;
        int hit_points;
        std::string immunity;
        std::string weakness;
        int atk_damage;
        std::string atk_type;
        int initiative;

        Group(std::string name, bool is_infection, int units, int hit_points, std::string immunity, std::string weakness, int atk_damage, std::string atk_type, int initiative) :
            name(name), is_infection(is_infection), units(units), hit_points(hit_points), immunity(immunity), weakness(weakness),
            atk_damage(atk_damage), atk_type(atk_type), initiative(initiative) {};

        inline int effective_pwr() const { return units * atk_damage; };

        std::array<int, 2> target_selection_order() const
        {
            return {effective_pwr(), initiative};
        }

        int damage(Group & other, bool dry_run) const
        {
            int damage = effective_pwr();
            if (other.weakness.find(atk_type) != std::string::npos)
                damage *= 2;
            if (other.immunity.find(atk_type) != std::string::npos)
                damage = 0;
            if (!dry_run)
            {
                int kills = std::min(damage / other.hit_points, other.units);
                other.units -= kills;
                return kills;
            }

            return damage;
        }

        std::array<int, 3> attack_preference (Group &other) const
        {
            return {damage(other, true), other.effective_pwr(), other.initiative};
        }
};

int play_round(std::vector<Group> &groups)
{
    std::sort(groups.begin(), groups.end(), [](Group &g1, Group &g2){ return g1.target_selection_order() > g2.target_selection_order(); });
    std::vector<int> targets(groups.size(), -1);
    std::vector<uint8_t> is_selected(groups.size(), 0);
    for (size_t i = 0; i < groups.size(); i++)
    {
        auto gi = groups[i];
        int target = -1;
        std::array<int, 3> target_pref = {INT_MIN, INT_MIN, INT_MIN};
        for (size_t j = 0; j < groups.size(); j++)
        {
            auto gj = groups[j];
            if (gi.is_infection == gj.is_infection) continue;
            if (is_selected[j]) continue;
            auto pref = gi.attack_preference(gj);
            std::cout << gi.name << " would deal " << pref[0] << " damage to " << gj.name << std::endl;
            if (pref > target_pref && pref[0] > 0)
            {
                target_pref = pref;
                target = j;
            }
        }
        if (target != -1)
        {
            is_selected[target] = 1;
            targets[i] = target;
            //std::cout << "target: " << groups[target] << std::endl;
        }
    }
    std::cout << std::endl;
    std::vector<size_t> indices(groups.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&](size_t i, size_t j) { return groups[i].initiative > groups[j].initiative; });
    for (auto i: indices)
    {
        if (targets[i] == -1) continue;
        auto kills = groups[i].damage(groups[targets[i]], false);
        std::cout << groups[i].name << " attacks " <<  groups[targets[i]].name <<", killing " << kills << std::endl;
    }
    groups.erase(std::remove_if(groups.begin(), groups.end(), [](Group &g) { return (g.units == 0); }), groups.end());

    auto pred = [](Group &g) { return g.is_infection; };
    if (std::all_of(groups.begin(), groups.end(), pred) || std::none_of(groups.begin(), groups.end(), pred))
    {
        int result = 0;
        for (auto &g: groups) result += g.units;
        return result;
    }
    return -1;
}

const std::vector<Group> example{
    Group("Immune system 1", false, 17, 5390, "", "radiation, bludgeoning", 4507, "fire", 2),
    Group("Immune system 2", false, 989, 1274, "fire", "bludgeoning, slashing", 25, "slashing", 3),
    Group("Infection 1", true, 801, 4706, "", "radiation", 116, "bludgeoning", 1),
    Group("Infection 2", true, 4485, 2961, "radiation", "fire, cold", 12, "slashing", 4),
};

const std::vector<Group> input{
//Immune System:
//2728 units each with 5703 hit points (weak to fire) with an attack that does 18 cold damage at initiative 12
    Group("Immune system 1", false, 2728, 5703, "", "fire", 18, "cold", 12),
//916 units each with 5535 hit points (weak to bludgeoning) with an attack that does 55 slashing damage at initiative 20
    Group("Immune system 2", false, 916, 5535, "", "bludgeoning", 55, "slashing", 20),
//2255 units each with 7442 hit points (weak to radiation) with an attack that does 31 bludgeoning damage at initiative 8
    Group("Immune system 3", false, 2255, 7442, "", "radiation", 31, "bludgeoning", 8),
//112 units each with 4951 hit points (immune to cold) with an attack that does 360 fire damage at initiative 9
    Group("Immune system 4", false, 112, 4951, "cold", "", 360, "fire", 9),
//7376 units each with 6574 hit points (immune to cold, slashing, fire) with an attack that does 7 bludgeoning damage at initiative 4
    Group("Immune system 5", false, 7376, 6574, "cold, slashing, fire", "", 7, "bludgeoning", 4),
//77 units each with 5884 hit points (weak to slashing) with an attack that does 738 radiation damage at initiative 6
    Group("Immune system 6", false, 77, 5884, "", "slashing", 738, "radiation", 6),
//6601 units each with 8652 hit points (weak to fire, cold) with an attack that does 11 fire damage at initiative 19
    Group("Immune system 7", false, 6601, 8652, "", "fire, cold", 11, "fire", 19),
//3259 units each with 10067 hit points (weak to bludgeoning) with an attack that does 29 cold damage at initiative 13
    Group("Immune system 8", false, 3259, 10067, "", "bludgeoning", 29, "cold", 13),
//2033 units each with 4054 hit points (immune to cold; weak to fire, slashing) with an attack that does 18 slashing damage at initiative 3
    Group("Immune system 9", false, 2033, 4054, "cold", "fire, slashing", 18, "slashing", 3),
//3109 units each with 3593 hit points with an attack that does 9 bludgeoning damage at initiative 11
    Group("Immune system 10", false, 3109, 3593, "", "", 9, "bludgeoning", 11),
//
//Infection:
//1466 units each with 57281 hit points (weak to slashing, fire) with an attack that does 58 slashing damage at initiative 7
    Group("Infection 1", true, 1466, 57281, "", "slashing, fire", 58, "slashing", 7),
//247 units each with 13627 hit points with an attack that does 108 fire damage at initiative 15
    Group("Infection 2", true, 247, 13627, "", "", 108, "fire", 15),
//1298 units each with 41570 hit points (immune to fire, bludgeoning) with an attack that does 63 fire damage at initiative 14
    Group("Infection 3", true, 1298, 41570, "fire, bludgeoning", "", 63, "fire", 14),
//2161 units each with 40187 hit points (weak to fire) with an attack that does 33 slashing damage at initiative 5
    Group("Infection 4", true, 2161, 40187, "", "fire", 33, "slashing", 5),
//57 units each with 55432 hit points (weak to cold) with an attack that does 1687 radiation damage at initiative 17
    Group("Infection 5", true, 57, 55432, "", "cold", 1687, "radiation", 17),
//3537 units each with 24220 hit points (weak to cold) with an attack that does 11 fire damage at initiative 10
    Group("Infection 6", true, 3537, 24220, "", "cold", 11, "fire", 10),
//339 units each with 44733 hit points (immune to cold, bludgeoning; weak to radiation, fire) with an attack that does 258 cold damage at initiative 18
    Group("Infection 7", true, 339, 44733, "cold, bludgeoning", "radiation, fire", 258, "cold", 18),
//1140 units each with 17741 hit points (weak to bludgeoning; immune to fire, slashing) with an attack that does 25 fire damage at initiative 2
    Group("Infection 8", true, 1140, 17741, "fire, slashing", "bludgeoning", 25, "fire", 2),
//112 units each with 44488 hit points (weak to bludgeoning, radiation; immune to cold) with an attack that does 749 radiation damage at initiative 16
    Group("Infection 9", true, 112, 44488, "cold", "bludgeoning, radiation", 749, "radiation", 16),
//2918 units each with 36170 hit points (immune to bludgeoning; weak to slashing, cold) with an attack that does 24 radiation damage at initiative 1
    Group("Infection 10", true, 2918, 36170, "bludgeoning", "slashing, cold", 24, "radiation", 1),
};

void solve_pt1()
{
    auto groups = input;
    int solution = -1;
    while (solution == -1)
    {
        solution = play_round(groups);
        std::cout << std::endl;
    }
    std::cout << "Part 1 solution: " << solution << std::endl;

}

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve_pt1();
    //solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
};