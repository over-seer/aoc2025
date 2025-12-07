#include <print>
#include <ranges>

#include "aoc.h"

void part1(const std::string &fn) {
  const auto ip = aoc::read_file(fn);
  std::set<size_t> beams = {std::ranges::find(ip.at(0), 'S') -
                            ip.at(0).begin()};
  std::println("{}", beams);

  size_t nsplit{};
  for (const std::string &line : ip | std::views::drop(1)) {
    std::set<size_t> new_beams;
    for (size_t ibeam : beams) {
      if (line.at(ibeam) == '^') {
        nsplit++;
        new_beams.insert(ibeam + 1);
        new_beams.insert(ibeam - 1);
      } else {
        new_beams.insert(ibeam);
      }
    }
    beams = new_beams;
  }
  std::println("day 07 part 1 = {} ({})", nsplit, fn);
}

int main() {
  part1("test_input");
  part1("input");
}