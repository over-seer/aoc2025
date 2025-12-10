#include <print>
#include <ranges>

#include "aoc.h"

auto splitl(const std::string &l) {
  const auto words = aoc::split(l, ' ') |
                     std::views::transform([&](const std::string &s) {
                       return s.substr(1, s.size() - 2);
                     }) |
                     std::ranges::to<std::vector>();
  const auto lights = words.at(0);
  const auto jolts = words.back();
  const auto switches = words | std::views::take(words.size() - 1) |
                        std::views::drop(1) | std::ranges::to<std::vector>();
  // std::vector<std::vector<int>> switches;
  return std::make_tuple(lights, switches, jolts);
}

auto getnos(const std::string &s) {
  const auto strs = aoc::split(s, ',');
  std::vector<int> nos;
  for (const auto &str : strs) {
    nos.push_back(atoi(str.c_str()));
  }
  return nos;
}

constexpr int NBIT = 16;
auto switchtobits(const std::vector<int> &is) {
  std::bitset<NBIT> bits{};
  for (const int i : is)
    bits[i] = true;
  return bits;
}

size_t min_flips(std::string l, const std::vector<std::vector<int>> &switches) {
  std::replace(l.begin(), l.end(), '#', '1');
  std::replace(l.begin(), l.end(), '.', '0');
  std::reverse(l.begin(),l.end());
  //std::println("{}", l);
  const std::bitset<NBIT> bits{l};
  const auto bitswitches = switches | std::views::transform(switchtobits);
  // std::println("{} {}", l, bits.to_ulong());
  const int ncomb = 1 << switches.size();
  // std::println("2^{} = {}", switches.size(), ncomb);
  auto ans = switches.size();
  for (int i = 0; i < ncomb; i++) {
    const std::bitset<NBIT> bitcomb{i};
    //std::println("{}",bitcomb.to_ulong());
    std::bitset<NBIT> lights{};
    for (size_t ib = 0; ib < bitcomb.size(); ib++) {
      if (bitcomb[ib]) {
        lights = lights ^ bitswitches[ib];
      }
    }
    if (lights == bits) {
      ans = std::min(ans, bitcomb.count());
      //std::println("{} {} {}",lights.to_string(),bitcomb.count(),bitcomb.to_string());
    }
  }
  //std::println("   {}",ans);
  return ans;
}

void part1(const std::string &fn) {
  const auto ip = aoc::read_file(fn) | std::views::transform(splitl) |
                  std::ranges::to<std::vector>();
  size_t maxlen{};
  size_t ans = 0;
  for (const auto &[lights, s, _] : ip) {
    // const auto lights = getnos(l);
    const auto switches =
        s | std::views::transform(getnos) | std::ranges::to<std::vector>();
    maxlen = std::max(maxlen, switches.size());
    ans += min_flips(lights, switches);
  }
  std::println("{}", ans);
}

int main() {
  part1("test_input");
  part1("input");
}
