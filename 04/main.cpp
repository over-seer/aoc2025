#include <print>
#include <ranges>

#include "aoc.h"

using DIJ = std::array<int, 2>;
using Map = std::vector<std::string>;
constexpr auto neighbours() {
  return std::array<DIJ, 8>{DIJ{-1, -1}, DIJ{-1, 0}, DIJ{-1, 1}, DIJ{0, -1},
                            DIJ{0, 1},   DIJ{1, -1}, DIJ{1, 0},  DIJ{1, 1}};
}

bool is_room(const auto &map, size_t irow, size_t icol) {
  size_t nrolls = 0;
  for (const auto [dr, dc] : neighbours()) {
    if (map.at(irow + dr).at(icol + dc) == '@') {
      if (++nrolls > 3)
        return false;
    }
  }
  // std::println("{},{}",irow,icol);
  return true;
}

void part1(const std::string &fn) {
  const auto ip = aoc::pad(aoc::read_file(fn), '.', 1);
  const size_t nrow = ip.size();
  const size_t ncol = ip.front().size();
  size_t ans = 0;
  for (size_t ir = 1; ir + 1 < nrow; ++ir) {
    for (size_t ic = 1; ic + 1 < ncol; ++ic) {
      if (ip.at(ir).at(ic) == '@' && is_room(ip, ir, ic))
        ans++;
    }
  }
  std::println("day 04 part 1 = {} ({})", ans, fn);
}

Map remove_accessible(const Map &map) {
  const size_t nrow = map.size();
  const size_t ncol = map.front().size();
  auto newmap = map;
  for (size_t ir = 1; ir + 1 < nrow; ++ir) {
    for (size_t ic = 1; ic + 1 < ncol; ++ic) {
      if (map.at(ir).at(ic) == '@' && is_room(map, ir, ic))
        newmap.at(ir).at(ic) = '.';
    }
  }
  return newmap;
}

void part2(const std::string &fn) {
  auto map = aoc::pad(aoc::read_file(fn), '.', 1);
  const auto nrolls_start = std::ranges::count(map | std::views::join, '@');
  auto nrolls_prev = nrolls_start;

  while (true) {
    auto newmap = remove_accessible(map);
    auto nrolls_now = std::ranges::count(newmap | std::views::join, '@');
    if (nrolls_now == nrolls_prev) {
      break;
    }
    map = newmap;
    nrolls_prev = nrolls_now;
  }

  std::println("day 04 part 2 = {} ({})", nrolls_start - nrolls_prev, fn);
}

int main() {
  part1("test_input");
  part1("input");
  part2("test_input");
  part2("input");
}