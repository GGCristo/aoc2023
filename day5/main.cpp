#include <iostream>
#include <fstream>
#include <string>
#include <ranges>
#include <vector>
#include <algorithm>
#include <limits>
#include <charconv>
#include <future>

namespace rg = std::ranges;
namespace rv = std::ranges::views;
auto constexpr max_size_stream { std::numeric_limits<std::streamsize>::max() };

auto trim { rv::filter([](auto&& r){ return !r.empty(); }) };
auto numbers { rv::split(' ') | trim | rv::transform([](auto&& r) {
    unsigned long number{};
    std::from_chars(r.data(), r.data() + r.size(), number);
    return number;
}) };

struct Range {
    unsigned long start_to;
    unsigned long start_from;
    std::size_t lenght;
    bool contains(unsigned long seed) const {
        return seed >= start_from && seed < start_from + lenght;
    }
};

void ignore_line(std::ifstream& input) {
    input.ignore(max_size_stream, '\n');
}

int main() {
    std::ifstream input { "input.txt" };
    input.ignore(max_size_stream, ':');
    std::string seeds {};
    std::getline(input, seeds); // Seeds
    if (seeds.empty()) throw std::runtime_error("There are no seeds");

    ignore_line(input); // Ignore empty line

    std::vector<std::vector<Range>> almanac{};
    do {
        ignore_line(input); // Ignore map name
        std::vector<Range> local_maps {};
        for(std::string line{}; getline(input, line) && !line.empty();){
            auto const [start_to, start_from, lenght] { (line | numbers | rv::adjacent<3>).front() };
            local_maps.emplace_back(start_to, start_from, lenght);
        }
        almanac.push_back(std::move(local_maps));
    } while (input.peek() != EOF);

    auto get_smallest_seed = [&almanac](rg::range auto&& seeds) {
        return rg::min(seeds | 
            rv::transform([&almanac](unsigned long seed) {
                for (std::vector<Range> const& maps : almanac) {
                    if (auto mapIt {rg::find_if(maps, [seed](Range const& r){ return r.contains(seed); })}; mapIt != maps.cend())
                        seed = mapIt->start_to + (seed - mapIt->start_from);
                }
                return seed;
            })
        );
    };
    std::cout << "Solution 1: " << get_smallest_seed(seeds | numbers) << '\n';
    
    std::vector<std::future<unsigned long>> mins{};
    for (auto const range : seeds | numbers | rv::adjacent<2> | rv::stride(2) | rv::transform([](auto const start_lenght) {
        auto const [start, lenght] { start_lenght };
        return rv::iota(start, start + lenght);
    })) {
        mins.push_back(std::async(get_smallest_seed, range));
    }

    std::cout << "Solution 2: " << rg::min(mins | rv::transform([](std::future<unsigned long>& fut){
        return fut.get();
    })) << '\n';
}