#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <ranges>
#include <array>
#include <vector>
#include <charconv>
#include <map>
#include <algorithm>

bool constexpr part2 { true };

namespace rg = std::ranges;
namespace rv = rg::views;

enum Type : std::size_t {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND,

    COUNT,
};

auto constexpr HAND_SIZE {5};
std::array constexpr strengths {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

Type get_type(std::string_view const hand_value) {
    std::map<char, std::size_t> counter{};
    std::size_t j_cards {0};
    for (char const c : hand_value) {
        if constexpr (part2) {
            if (c == 'J') {
                ++j_cards;
                continue;
            }
        }
        ++counter[c];
    }
    switch (counter.size()) {
    case 0: [[fallthrough]];
    case 1: return Type::FIVE_OF_A_KIND;
    case 2: {
        if (auto freq { counter.begin()->second }; freq == (HAND_SIZE - 1 - j_cards) || freq == 1)
            return Type::FOUR_OF_A_KIND;
        else
            return Type::FULL_HOUSE;
    }
    case 3: {
        if (rg::find(counter, 3 - j_cards, &decltype(counter)::value_type::second) != counter.cend()) 
            return Type::THREE_OF_A_KIND;
        else 
            return Type::TWO_PAIR;
    }
    case 4: return Type::ONE_PAIR;
    default: return Type::HIGH_CARD;
    }
}

struct Hand {
    std::string value;
    unsigned long bid;
};

using Hands = std::vector<Hand>;

int main() {
    std::fstream input { "input.txt" };
    std::array<Hands, Type::COUNT> game{};
    for (std::string line{}; std::getline(input, line);) {
        Hand hand {.value { line.substr(0, HAND_SIZE) } };
        std::from_chars(line.data()+HAND_SIZE+1, line.data()+line.size(), hand.bid);

        Hands& hands { game[get_type(hand.value)] };
        hands.insert(rg::upper_bound(hands, hand, [](Hand const& lhs, Hand const& rhs){ 
            for (auto const [l, r] : rv::zip(lhs.value, rhs.value)) {
                if (l == r) continue;
                if constexpr (part2) {
                    if (l == 'J') return true;
                    if (r == 'J') return false;
                }
                return rg::find(strengths, l) < rg::find(strengths, r);
            }
            return false;
         }), std::move(hand));
    }

    auto accum {0ul};
    for (auto const& [rank, hand] : game | rv::join | rv::enumerate) {
        accum += hand.bid * (rank + 1);
    }
    std::cout << "Solution: " << accum << '\n';
}