#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <unordered_map>
#include <optional>
#include <map>
#include <format>

namespace ranges = std::ranges;
namespace rv = std::ranges::views;

std::vector<std::string> readInput() {
    std::vector<std::string> result{};
    std::ifstream input{ "input.txt" };
    std::string line {};
    while (std::getline(input, line)) {
        result.push_back(std::move(line));
    }
    return result;
}

struct Point {
    long x{};
    long y{};
    auto operator<=>(Point const&) const = default;
};

struct NumberValid {
    std::string value {}; // Posible valid value
    bool valid {false}; // Next to symbol
    std::optional<Point> gear_pos {}; // Posible gear position
    void clear() {
        value.clear();
        valid = false;
        gear_pos.reset();
    }
};

struct Gear {
    std::pair<std::optional<unsigned long>, std::optional<unsigned long>> values {{}, {}};
    bool valid { false };
    void add_value(unsigned long value) {
        if (!values.first) values.first = value;
        else if (!values.second) {
            values.second = value;
            valid = true;
        } else valid = false;
    }
};

void check_valid(std::vector<std::string> const& grid, Point const position, NumberValid& number_valid) {
    auto const [x, y] {position};

    auto check_symbol = [&grid, &number_valid](Point const& position) {
        if (char const c {grid[position.x][position.y]}; c == '*') {
            number_valid.gear_pos = position;
            number_valid.valid = true;
        } else {
            number_valid.valid = !std::isdigit(c) && c != '.';
        }
        return number_valid.valid;
    };
    
    if (x != 0 && check_symbol({x-1, y})) return; // N
    if (x != grid.size()-1 && check_symbol({x+1, y})) return; // S
    if (y != 0 && check_symbol({x, y-1})) return; // W
    if (y != grid[x].size()-1 && check_symbol({x, y+1})) return; // E

    if (x != 0 && y != 0 && check_symbol({x-1, y-1})) return; // NW
    if (x != 0 && y != grid[x-1].size()-1 && check_symbol({x-1, y+1})) return; // NE
    if (x != grid.size()-1 && y != 0 && check_symbol({x+1, y-1})) return; // SW
    if (x != grid.size()-1 && y != grid[x+1].size()-1 && check_symbol({x+1, y+1})) return; // SE
}


int main() {
    std::vector<std::string> input { readInput() };
    unsigned long acumulator {};
    NumberValid number{};
    std::map<Point, Gear> gear_record {};

    auto join_matrix { rv::transform([](auto linePair){ 
        return rv::cartesian_product(ranges::single_view{std::get<0>(linePair)}, std::get<1>(linePair) | rv::enumerate);
    }) | rv::join };

    for (auto const& [line, column_c] : input | rv::enumerate | join_matrix) {
        auto const& [column, c] = column_c;
        if (std::isdigit(c)) {
            number.value += c;
            if (!number.valid) check_valid(input, Point {line, column}, number);
        } else {
            if (number.valid) acumulator += std::stoul(number.value);
            if (number.gear_pos) gear_record[*number.gear_pos].add_value(std::stoul(number.value));
            number.clear();
        }
    }
    auto result2 {[&gear_record](){
        unsigned long acum {};
        for (auto const& [point, gear] : gear_record) {
            if (gear.valid) acum += *gear.values.first * *gear.values.second;
        }
        return acum;
    }()};
    std::cout << "Solution: " << acumulator << '\n';
    std::cout << "Solution part 2: " << result2 << '\n';
}