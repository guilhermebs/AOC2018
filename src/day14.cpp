#include <iostream>
#include <vector>
#include <array>
#include <chrono>


void solve_pt1() {
    const size_t input = 990941;
    const size_t nrecipes = input + 10;
    std::vector<size_t> recipes = {3, 7};
    recipes.reserve(nrecipes);

    size_t elf1 = 0;
    size_t elf2 = 1;
    
    while (recipes.size() < nrecipes) {
        auto new_score = recipes[elf1] + recipes[elf2];
        if (new_score >= 10)
            recipes.push_back((new_score / 10) % 10);
        recipes.push_back(new_score % 10);
        elf1 = (elf1 + 1 + recipes[elf1]) % recipes.size();
        elf2 = (elf2 + 1 + recipes[elf2]) % recipes.size();

        //for (auto r: recipes)
        //    std::cout  << r;
        //std::cout  << std::endl;
    }

    std::cout << "Part 1 solution: ";
    for (auto i = input; i < nrecipes; i++)
        std::cout  << recipes[i];
    std::cout << std::endl;
}

bool check_pt2(const std::array<size_t, 6> &input, const std::vector <size_t> &recipes) {
    auto r = recipes.end() - input.size();
    auto i = input.begin();
    while (r != recipes.end())
    {
        if (*(r++) != *(i++))
            return false;
    }
    return true;
}


void solve_pt2() {
    const std::array<size_t, 6> input = {9, 9, 0, 9, 4, 1};
    std::vector<size_t> recipes = {3, 7};

    size_t elf1 = 0;
    size_t elf2 = 1;
    
    while (recipes.size() < 6 || !check_pt2(input, recipes)) {
        auto new_score = recipes[elf1] + recipes[elf2];
        if (new_score >= 10) {
            recipes.push_back((new_score / 10) % 10);
            if (check_pt2(input, recipes))
                break;
        }
        recipes.push_back(new_score % 10);
        elf1 = (elf1 + 1 + recipes[elf1]) % recipes.size();
        elf2 = (elf2 + 1 + recipes[elf2]) % recipes.size();
    }
    std::cout << "Part 2 solution : " << recipes.size() - input.size() << std::endl;
}



int main() {
    auto started = std::chrono::high_resolution_clock::now();
    //solve_pt1();
    solve_pt2();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}