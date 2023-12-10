#include <iostream>
#include <fstream>
#include <string>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <limits>
#include <string_view>

auto constexpr MAX_SIZE_STREAM { std::numeric_limits<std::streamsize>::max() };
namespace rv = std::ranges::views;

int main() {
    std::fstream input { "input.txt" };
    std::string directions {};
    std::getline(input, directions);
    
    input.ignore(MAX_SIZE_STREAM, '\n');
    auto map_parser { rv::istream<std::string>(input) | rv::filter([](std::string w) { return w != "="; }) };
    std::unordered_map<std::string, std::pair<std::string, std::string>> map {};
    for (auto readerIt { map_parser.begin() }; input.peek() != EOF; ++readerIt) {
        std::string from { *readerIt };
        std::string left { *++readerIt }; left.pop_back(); left.erase(0, 1);
        std::string right { *++readerIt }; right.pop_back();
        map.try_emplace(std::move(from), std::move(left), std::move(right));
    }
    // for (auto const& [from, value] : map) {
    //     std::cout << from << '|' << value.first << '|' << value.second << "|\n";
    // }

    auto counter { 0ul };
    std::string current_pos {"AAA"};
    for (auto it = directions.cbegin(); current_pos != "ZZZ"; ++it) {
        if (it == directions.cend()) it = directions.cbegin();
        if (*it == 'L') current_pos = map[current_pos].first;
        else current_pos = map[current_pos].second;
        ++counter;
    }
    std::cout << "Steps: " << counter << '\n';
}