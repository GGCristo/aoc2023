#include <iostream>
#include <fstream>
#include <string>
#include <ranges>
#include <algorithm>
#include <format>
#include <map>

namespace ranges = std::ranges;
namespace rv = std::ranges::views;

auto parse_sets(std::string_view const line) { 
    if (auto initial_pos { ranges::find(line, ':')}; initial_pos == line.cend())
        throw std::runtime_error("couldn't find ':', or there are no data available");
    else
        return ranges::subrange{std::next(initial_pos), line.cend()} | rv::split(';');
}

auto parse_rolls { rv::split(',') | rv::transform([](auto pair) {
    auto parse_pair { pair | rv::split(' ') | rv::drop(1) };
    std::string number_str {std::string_view{*parse_pair.begin()}}; // GCC doesn't support string from range yet :(
    return std::make_pair(std::stoul(number_str), std::string_view{*(ranges::next(parse_pair.begin()))});
})};

bool is_valid_game(std::string_view const line) {
    std::map<std::string_view, unsigned long> static const GAME { {"red", 12}, {"green", 13}, {"blue", 14} };
    for (auto set : parse_sets(line)) {
        for (auto const& [number, color] : set | parse_rolls) {
            if (auto it { GAME.find(color) }; it == GAME.cend())
                throw std::runtime_error(std::format("Color {} is not valid", color));
            else if (number > it->second) return false;
        }
    }
    return true;
}

unsigned long fewest_cube(std::string_view const line) {
    std::map<std::string_view, unsigned long> game { {"red", 0}, {"green", 0}, {"blue", 0} };
    for (auto set : parse_sets(line)) {
        for (auto const& [number, color] : set | parse_rolls) {
            if (auto it { game.find(color) }; it == game.cend())
                throw std::runtime_error(std::format("Color {} is not valid", color));
            else
                it->second = std::max(it->second, number);
        }
    }
    return ranges::fold_left(game | rv::values, 1, std::multiplies<>{});
}

int main() {
    std::ifstream input{"input.txt"};
    std::string line{};
    unsigned long result_one{0};
    unsigned long result_two{0};
    while(std::getline(input, line)) {
        unsigned long id { [&line]() {
            auto parse_id { line | rv::split(' ') | rv::drop(1) };
            std::string second_w { std::string_view{ parse_id.front() } }; // "ID:"
            second_w.pop_back(); // "ID"
            return std::stoul(second_w);
        }() };
        if (is_valid_game(line)) result_one += id;
        result_two += fewest_cube(line);
    }
    std::cout << "Result_1: " << result_one << '\n';
    std::cout << "Result_2: " << result_two << '\n';
}