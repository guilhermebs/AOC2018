#include <iostream>
#include <vector>
#include <chrono>


void solve() {
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

int main() {
    auto started = std::chrono::high_resolution_clock::now();
    solve();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
}