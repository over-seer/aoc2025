#include <functional>
#include <print>
#include <ranges>

#include "aoc.h"

void part1(const std::string &fn) {
  std::println("debug");
  auto split = [](const std::string &line) {
    return aoc::split(line, ' ') |
           std::views::filter([](const std::string &s) { return !s.empty(); }) |
           std::ranges::to<std::vector>();
  };
  const auto ip = aoc::read_file(fn) | std::views::transform(split) |
                  std::ranges::to<std::vector>();
  const auto ncol = ip.front().size();
  const auto nrow = ip.size();
  assert(
      std::ranges::all_of(ip, [&](const auto &r) { return r.size() == ncol; }));
  //std::println("{} {}", nrow, ncol);
  //std::println("{}", ip);
  size_t ans{};
  for (auto ic = 0UL; ic < ncol; ic++) {
    auto i = atoll(ip.front().at(ic).c_str());
    const auto op = ip.back().at(ic).front();
    //std::println("{} {}", i, op);
    for (auto ir = 1UL; ir + 1 < nrow; ir++) {
      const auto j = atoll(ip.at(ir).at(ic).c_str());
      if (op == '*')
        i *= j;
      else if (op == '+')
        i += j;
      else
        throw std::runtime_error("bad operation");
      //std::println("{} {}", i, j);
    }
    //std::println("{}", i);
    ans += i;
  }
  std::println("{}", ans);
}


int main() {
  part1("test_input");
  part1("input");
}