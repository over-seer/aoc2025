#include <functional>
#include <print>
#include <ranges>

#include "aoc.h"

using Lims = std::array<long long int, 2>;

bool is_in_lims(auto ingr, const auto &lims) {
  return ingr >= lims[0] && ingr <= lims[1];
}

auto parse_lims = [](const std::string &s) {
  const auto [lo, hi] = aoc::splitn<2, '-'>(s);
  return std::array{atoll(lo.c_str()), atoll(hi.c_str())};
};

void part1(const std::string &fn) {
  const auto ip = aoc::read_paragraph_ip(fn);

  const auto lims = ip.at(0) | std::views::transform(parse_lims) |
                    std::ranges::to<std::set>();
  const auto ingrs = ip.at(1) | std::views::transform([](const std::string &s) {
                       return atoll(s.c_str());
                     });

  // std::println("{}", lims);
  // std::println("{}", ingrs);

  int ans{};
  for (const auto ingr : ingrs) {
    for (const auto &lohi : lims) {
      if (is_in_lims(ingr, lohi)) {
        ans++;
        break;
      }
    }
  }
  std::println("day 05 part 1 = {} ({})", ans, fn);
}

void part2(const std::string &fn) {
  const auto ip = aoc::read_paragraph_ip(fn);

  const auto lims = ip.at(0) | std::views::transform(parse_lims) |
                    std::ranges::to<std::set>();

  auto lo_curr = lims.begin()->at(0);
  auto hi_curr = lims.begin()->at(1);
  std::set<Lims> merged;
  for (const auto [lo, hi] : lims | std::views::drop(1)) {
    if (lo > hi_curr) {
      merged.insert(Lims{lo_curr, hi_curr});
      lo_curr = lo;
      hi_curr = hi;
    } else {
      hi_curr = std::max(hi_curr, hi);
    }
  }
  merged.insert(Lims{lo_curr, hi_curr});
  // std::println("{}", lims);
  // std::println("{}", merged);

  auto rangelen = [](const Lims &p) { return p[1] - p[0] + 1; };

  const auto ans = std::ranges::fold_left(
      merged | std::views::transform(rangelen), 0, std::plus());

  std::println("day 05 part 2 = {} ({})", ans, fn);
}

int main() {
  part1("test_input");
  part1("input");
  part2("test_input");
  part2("input");
}
