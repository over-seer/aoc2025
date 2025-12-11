#include <functional>
#include <print>
#include <ranges>

#include "aoc.h"

auto parse(const std::string &line) {
  const auto words = aoc::split(line, ' ');
  const auto key = words.at(0).substr(0, words.at(0).size() - 1);
  return std::make_pair(key, words | std::views::drop(1) |
                                 std::ranges::to<std::set>());
}

auto paths(const std::map<std::string, std::set<std::string>> &graph,
           const std::string &s) {
  if (s == "out")
    return 1UL;
  else
    return std::ranges::fold_left(
        graph.at(s) | std::views::transform(std::bind_front(paths, graph)), 0UL,
        std::plus());
}

void part1(const std::string &fn) {
  const auto ip = aoc::read_file(fn) | std::views::transform(parse) |
                  std::ranges::to<std::map>();
  //std::println("{}", ip);
  std::set<std::string> all;
  for (const auto &[k, v] : ip) {
    all.insert(k);
    for (const auto &s : v)
      all.insert(s);
  }
  const auto ans = paths(ip, "you");
  std::println("day 11 part 1 = {}", ans);
}

int main() {
  part1("test_input");
  part1("input");
}