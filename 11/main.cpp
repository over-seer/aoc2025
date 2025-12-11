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
  // std::println("{}", ip);
  std::set<std::string> all;
  for (const auto &[k, v] : ip) {
    all.insert(k);
    for (const auto &s : v)
      all.insert(s);
  }
  const auto ans = paths(ip, "you");
  std::println("day 11 part 1 = {}", ans);
}

size_t paths2(std::map<std::string, size_t> &cache,
              const std::map<std::string, std::set<std::string>> &graph,
              std::set<std::string> visited, const std::string &target,
              const std::string &s) {
  if (cache.contains(s)) {
    // std::println("using cache!");
    return cache.at(s);
  }
  // std::println("{}", s);
  size_t result{};
  if (!visited.contains(s)) {
    visited.insert(s);
    if (s == target) {
      result = 1UL;
    } else if (graph.contains(s)) {
      for (const std::string &s2 : graph.at(s)) {
        result += paths2(cache, graph, visited, target, s2);
      }
    }
  }
  cache.insert({s, result});
  // std::println("{}", cache);
  return result;
}

void part2(const std::string &fn) {
  const auto ip = aoc::read_file(fn) | std::views::transform(parse) |
                  std::ranges::to<std::map>();
  // std::println("{}", ip);
  std::set<std::string> all;
  for (const auto &[k, v] : ip) {
    all.insert(k);
    for (const auto &s : v)
      all.insert(s);
  }
  auto comb = [&](const std::string &a, const std::string &b) {
    std::map<std::string, size_t> cache;
    return paths2(cache, ip, std::set<std::string>{}, b, a);
  };
  const auto svr_fft = comb("svr", "fft");
  const auto fft_dac = comb("fft", "dac");
  const auto dac_out = comb("dac", "out");
  const auto svr_dac = comb("svr", "dac");
  const auto dac_fft = comb("dac", "fft");
  const auto fft_out = comb("fft", "out");
  std::println("day 11 part 2 = {}",
               svr_fft * fft_dac * dac_out + svr_dac * dac_fft * fft_out);
}

int main() {
  part1("test_input");
  part1("input");
  part2("test_input2");
  part2("input");
}