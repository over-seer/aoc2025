#include <print>
#include <ranges>
#include <string>
#include <vector>

#include "aoc.h"

int parse_line(const std::string &l) {
  return std::atoi(l.substr(1).c_str()) * (l.front() == 'L' ? -1 : 1);
}

struct RotateResult {
  int pos{};
  int zeros{};
};

RotateResult rotate(const int pos, const std::string &instr) {
  const int di = parse_line(instr);
  // reflect left turns to be same as right turns
  const bool isR = di >= 0;
  const int dmag = abs(di);
  const int refl_start = isR ? pos : (100 - pos) % 100;
  const int refl_tot = refl_start + dmag;
  const int refl_end = refl_tot % 100;
  // reflect left turn case back
  const int newpos = isR ? refl_end : 100 - refl_end;
  int nzeros = refl_tot / 100;
  return {.pos = newpos % 100, .zeros = nzeros};
}

void both_parts(const std::string &ip) {
  const auto v = aoc::read_file(ip);
  int ans1{}, ans2{}, pos{50};
  for (const std::string & s : v) {
    const auto result = rotate(pos, s);
    if (result.pos == 0)
      ans1++;
    ans2 += result.zeros;
    pos = result.pos;
  }

  std::println("day 01 part 1 ans = {} ({})", ans1, ip);
  std::println("day 01 part 2 ans = {} ({})", ans2, ip);
}

int main() {
  both_parts("test_input");
  both_parts("input");
  std::vector<std::pair<int, std::string>> tests = {
      {50, "L25"}, {50, "R25"}, {25, "L50"},  {25, "R50"},
      {50, "L50"}, {50, "R50"}, {50, "R350"}, {50, "L350"},

  };
  for (auto [p0, d] : tests) {
    auto [pos, nz] = rotate(p0, d);
    std::println("{} {} -> {} ({} zeros)", p0, d, pos, nz);
  }
}