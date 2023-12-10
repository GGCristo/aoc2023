#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <ranges>
#include <charconv>
#include <set>
#include <vector>
#include <algorithm>
#include <cmath>

namespace ranges = std::ranges;
namespace rv = std::ranges::views;

std::set<unsigned long> to(auto&& r) {
    std::set<unsigned long> result{};
    for (unsigned long number : r) { result.insert(number); }
    return result;
}

void get_copies (std::vector<unsigned long> const& matching_numbers, unsigned long idx, unsigned long& result) {
    result += matching_numbers[idx];
    for (unsigned long i{1}; i <= matching_numbers[idx]; ++i) {
        get_copies(matching_numbers, idx+i, result);
    }
}

int main() {
    std::ifstream input { "input.txt" };
    auto get_numbers = rv::split(' ') | rv::filter([](auto word){ return !word.empty(); }) |
        rv::transform([](auto&& r){ return std::stoul(std::string{std::string_view{r}}); });
    std::string chunk {};
    std::vector<unsigned long> matching_numbers{}; // [game] -> number of matching numbers
    unsigned long result1 { 0 };

    while(std::getline(input, chunk, ':')) {
        if (!std::getline(input, chunk, '|')) throw std::runtime_error("nothing after ':");
        std::set winning_numbers { to(chunk | get_numbers) };

        if (!std::getline(input, chunk)) throw std::runtime_error("no number after winning numbers");
        std::set current_numbers { to(chunk | get_numbers) };

        unsigned long winners_number {[&winning_numbers, &current_numbers]() {
            std::vector<unsigned long> winners {};
            ranges::set_intersection(winning_numbers, current_numbers, std::back_inserter(winners));
            return winners.size();
        }()};
        
        matching_numbers.push_back(winners_number);
        if (winners_number != 0) result1 += std::pow(2, winners_number-1);
    }
    std::cout << "Result_1: " << result1 << '\n';

    unsigned long result2 {0};
    for (unsigned long game {0}; game < matching_numbers.size(); ++game) {
        get_copies(matching_numbers, game, ++result2);
    }
    std::cout << "Result_2: " << result2 << '\n';
}