#include <iostream>
#include <fstream>
#include <ranges>
#include <charconv>
#include <functional>
#include <limits>
#include <algorithm>

auto constexpr max_size_stream { std::numeric_limits<std::streamsize>::max() };

namespace rg = std::ranges;
namespace rv = rg::views;

unsigned long to_number(rg::range auto&& r) {
    if constexpr (std::ranges::contiguous_range<decltype(r)>) {
        unsigned long result{0};
        std::from_chars(r.data(), r.data() + r.size(), result);
        return result;
    } else {
        return std::stoul(std::string{r.begin(), r.end()});
    }
}

unsigned long get_possible_wins(unsigned long time, unsigned long distance_record) {
    return rg::count_if(rv::iota(1ul, time), [time, distance_record](unsigned long tp) { 
        return (tp * (time - tp)) > distance_record; // distance = v*(t-tp); v=tp;
    });
}

unsigned long accumulate(rg::range auto&& time_r, rg::range auto&& distance_r) {
    return rg::fold_left(rv::zip(time_r, distance_r) | rv::transform([](auto const& time_distance) {
        return std::apply(get_possible_wins, time_distance);
    }), 1, std::multiplies<>{});
}

int main() {
    std::fstream input { "input.txt" };
    input.ignore(max_size_stream, ':');
    std::string times{};
    std::getline(input, times);
    input.ignore(max_size_stream, ':');
    std::string distances{};
    std::getline(input, distances);

    auto numbers { rv::split(' ') | rv::filter(std::not_fn(rg::empty)) | rv::transform([](auto&& r) {return to_number(r); }) };
    std::cout << "Solution 1: " << accumulate(times | numbers, distances | numbers) << '\n';

    auto nospace { rv::filter([](unsigned char c) { return !std::isspace(c); }) };
    std::cout << "Solution 2: " << get_possible_wins(to_number(times | nospace), to_number(distances | nospace)) << '\n';
}