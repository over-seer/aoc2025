#include <print>
#include <ranges>

#include "aoc.h"

void part1(const std::string &ip) {
  long int ans = 0;
  const auto pairs = aoc::split(ip, ',');
  auto contr = [](const std::string &s) {
    if (s.size() % 2 == 0) {
      const size_t hl = s.size() / 2;
      const std::string hs = s.substr(0, hl);
      if (hs == s.substr(hl)) {
        // std::println("  {}", s);
        return atol(s.c_str());
      }
    }
    return 0l;
  };
  for (const auto &[a, b] :
       pairs | std::views::transform(aoc::splitn<2, '-'>)) {
    long int first = atol(a.c_str());
    long int last = atol(b.c_str());
    for (long int i = first; std::cmp_less_equal(i, last); i++) {
      ans += contr(std::to_string(i));
    }
  }
  std::println("day 02 part 1 ans = {}", ans);
}

void part2(const std::string &ip) {
  long int ans = 0;
  const auto pairs = aoc::split(ip, ',');
  auto contr = [](const std::string &s) {
    for (int n = 2; std::cmp_less(n, 20); n++) {
      if (s.size() % n == 0) {
        const size_t hl = s.size() / n;
        const std::string hs = s.substr(0, hl);
        bool isdodgy = true;
        for (size_t pos = 1; std::cmp_less(pos, n); pos++) {
          if (hs != s.substr(pos * hl, hl)) {
            isdodgy = false;
            // std::println("  {}", s);
            break;
          }
        }
        if (isdodgy) {

          // std::println("  {}", s);
          return atol(s.c_str());
          break;
        }
      }
    }

    return 0l;
  };
  for (const auto &[a, b] :
       pairs | std::views::transform(aoc::splitn<2, '-'>)) {
    long int first = atol(a.c_str());
    long int last = atol(b.c_str());
    for (long int i = first; std::cmp_less_equal(i, last); i++) {
      ans += contr(std::to_string(i));
    }
  }
  std::println("day 02 part 2 ans = {}", ans);
}

int main() {
  part1(aoc::read_file("test_input").front());
  part1(aoc::read_file("input").front());
  part2(aoc::read_file("test_input").front());
  part2(aoc::read_file("input").front());
}