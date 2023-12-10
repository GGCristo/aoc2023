#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ranges>
#include <array>
#include <map>

namespace ranges = std::ranges;
namespace rv = std::ranges::views;
using namespace std::literals;

std::array<std::pair<std::string_view, std::string>, 18> constexpr convert {{{"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"}, 
                                                      {"6", "6"}, {"7", "7"}, {"8", "8"}, {"9", "9"}, {"one", "1"}, 
                                                      {"two", "2"}, {"three", "3"}, {"four", "4"}, {"five", "5"}, 
                                                      {"six", "6"}, {"seven", "7"}, {"eight", "8"}, {"nine", "9"}}};

template<bool REVERSE>
std::string get_word(std::string_view const line) {
  std::size_t const n { line.size() };
  for (std::size_t i {0}; i < n; ++i) {
    auto it { [i, n, &line]() {
      if constexpr (!REVERSE) return ranges::find_if(convert, [word = line | rv::drop(i)](auto pair){ return word.starts_with(pair.first); });
      else                    return ranges::find_if(convert, [word = line | rv::take(n - i)](auto pair){ return word.ends_with(pair.first); });
    }() };
    if (it != convert.cend()) {
      return it->second;
    }
  }
  return std::string{""};
}

int main() {
  std::ifstream input{"input.txt"};
  std::string line{};
  unsigned long acumulator{0};
  while (std::getline(input, line)) {
    std::string const l_number{get_word<false>(line)};
    std::string const r_number{get_word<true>(line)};
    acumulator += std::stoul(l_number + r_number);
  }
  std::cout << "Solution: " << acumulator << std::endl;
}
