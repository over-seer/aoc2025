#include "aoc.h"
#include <format>
#include <print>
#include <ranges>

size_t jolts_2_digits(const std::string &bank) {
  const auto result =
      std::ranges::max_element(bank | std::views::take(bank.size() - 1));
  const auto pos = result - bank.begin();
  const char c1 = *result;
  const char c2 = std::ranges::max(bank | std::views::drop(pos + 1));
  return atoi(std::format("{}{}", c1, c2).c_str());
}

size_t jolts(const std::string &bank, size_t n) {
  size_t first = 0;
  std::string num;
  for (size_t i = 0; i < n; i++) {
    const size_t last = bank.size() - (n - i) + 1;
    const auto result = std::ranges::max_element(bank | std::views::take(last) |
                                                 std::views::drop(first));
    first = result - bank.begin() + 1;
    num.push_back(*result);
  }
  return atoll(num.c_str());
}

size_t jolts_alternative(const std::string &bank, size_t n) {
  size_t k = bank.size() - n;
  std::string num;
  for (char c : bank) {
    while (num.size() > 0 && num.back() < c && k > 0) {
      num.pop_back();
      k--;
    }
    num.push_back(c);
  }
  num.resize(n);
  return atoll(num.c_str());
}

void part1(const std::string &fn) {
  const auto ip = aoc::read_file(fn);
  size_t ans1a{}, ans1b{}, ans1c{}, ans2a{}, ans2b{};
  for (const auto &s : ip) {
    ans1a += jolts_2_digits(s);
    ans1b += jolts(s, 2);
    ans1c += jolts_alternative(s, 2);
    ans2a += jolts(s, 12);
    ans2b += jolts_alternative(s, 12);
  }
  std::println("day 03 part 1a = {} ({})", ans1a, fn);
  std::println("day 03 part 1b = {} ({})", ans1b, fn);
  std::println("day 03 part 1c = {} ({})", ans1c, fn);
  std::println("day 03 part 2a = {} ({})", ans2a, fn);
  std::println("day 03 part 2b = {} ({})", ans2b, fn);
}

int main() {
  part1("test_input");
  part1("input");
}