#include <iostream>
#include <fstream>
#include <ranges>
#include <vector>
#include <limits>

auto constexpr MAX_SIZE_STREAM { std::numeric_limits<std::streamsize>::max() };
namespace rv = std::ranges::views;

struct Ghost {
    std::vector<std::string, std::size_t> loop{};
};

int main() {
    std::fstream input { "../input.txt" };
    std::string directions {};
    std::getline(input, directions);
    
    input.ignore(MAX_SIZE_STREAM, '\n');
    std::vector<Ghost> ghosts {};
    for (auto const& w : rv::istream<std::string>(input)) {
        
    }
}