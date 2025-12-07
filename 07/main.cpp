#include <print>
#include <ranges>

#include "aoc.h"

void part1(const std::string &fn) {
  const auto ip = aoc::read_file(fn);
  std::map<size_t, size_t> beams = {
      {std::ranges::find(ip.at(0), 'S') - ip.at(0).begin(), 1UL}};
  std::println("{}", beams);

  size_t nsplit{};
  for (const std::string &line : ip | std::views::drop(1)) {
    std::map<size_t, size_t> new_beams;
    for (auto [ibeam, nhist] : beams) {
      if (line.at(ibeam) == '^') {
        nsplit++;
        new_beams[ibeam + 1] += nhist;
        new_beams[ibeam - 1] += nhist;
      } else {
        new_beams[ibeam] += nhist;
      }
    }
    beams = new_beams;
  }
  const auto ntimeline =
      std::ranges::fold_left(beams | std::views::values, 0UL, std::plus());
  std::println("day 07 part 1 = {} ({})", nsplit, fn);
  std::println("day 07 part 2 = {} ({})", ntimeline, fn);
}

int main() {
  part1("test_input");
  part1("input");
}