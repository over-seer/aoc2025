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
  std::reverse(l.begin(), l.end());
  // std::println("{}", l);
  const std::bitset<NBIT> bits{l};
  const auto bitswitches = switches | std::views::transform(switchtobits);
  // std::println("{} {}", l, bits.to_ulong());
  const int ncomb = 1 << switches.size();
  // std::println("2^{} = {}", switches.size(), ncomb);
  auto ans = switches.size();
  for (int i = 0; i < ncomb; i++) {
    const std::bitset<NBIT> bitcomb{i};
    // std::println("{}",bitcomb.to_ulong());
    std::bitset<NBIT> lights{};
    for (size_t ib = 0; ib < bitcomb.size(); ib++) {
      if (bitcomb[ib]) {
        lights = lights ^ bitswitches[ib];
      }
    }
    if (lights == bits) {
      ans = std::min(ans, bitcomb.count());
      // std::println("{} {}
      // {}",lights.to_string(),bitcomb.count(),bitcomb.to_string());
    }
  }
  // std::println("   {}",ans);
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

struct Switches {
  std::vector<std::vector<int>> switches;
  std::map<std::set<int>, std::map<int, std::set<int>>> cache;
};

auto map_switch_by_position(const auto &switches) {
  std::map<size_t, std::set<size_t>> result;
  for (const auto &[ix, swtch] : switches | std::views::enumerate) {
    for (const auto ij : swtch) {
      result[ij].insert(ix);
    }
  }
  return result;
}

auto get_switch_order(auto switches) {
  const auto ns = switches.size();
  std::vector<size_t> order;
  std::vector<std::map<size_t, std::set<size_t>>> switch_by_position_by_order;
  while (order.size() < ns) {
    const auto switch_by_position = map_switch_by_position(switches);
    size_t minswitches = ns;
    size_t next_pos{};
    for (const auto &[pos, switches] : switch_by_position) {
      if (switches.size() < minswitches) {
        minswitches = switches.size();
        next_pos = pos;
      }
    }
    size_t next_switch{};
    size_t maxpositions{};
    for (size_t sw : switch_by_position.at(next_pos)) {
      const size_t npos = switches.at(sw).size();
      if (npos > maxpositions) {
        maxpositions = npos;
        next_switch = sw;
      }
    }
    order.push_back(next_switch);
    switch_by_position_by_order.push_back(switch_by_position);
    switches.at(next_switch).clear();
  }
  return std::make_pair(order, switch_by_position_by_order);
}

void solve(const auto &order, size_t ix, const auto &switches,
           const auto &switch_by_position, auto jolts, size_t npress,
           size_t totpress, size_t &minpress) {
  if (totpress >= minpress)
    return;
  // see if joltage reached
  if (std::ranges::all_of(jolts, [](auto i) { return i == 0; })) {
    minpress = std::min(minpress, totpress);
    // std::println("success {}", minpress);
    return;
  }
  // check for overshoot
  if (std::ranges::any_of(jolts, [](auto i) { return i < 0; })) {
    // std::println("overshoot {}", jolts);
    return;
  }
  if (ix == order.size())
    return;
  const size_t isw = order.at(ix);
  const auto &swtch = switches.at(isw);
  const auto &sbp = switch_by_position.at(ix);
  // std::println("{} {} {} {} {} {}", ix, swtch, jolts, npress, totpress,
  // minpress);
  size_t next_press_min{};
  size_t next_press_max{10000};
  // check for only combination
  for (auto ipos : swtch) {
    const int diff = jolts.at(ipos);
    next_press_max =
        std::cmp_greater(next_press_max, diff) ? diff : next_press_max;
    if (sbp.at(ipos).size() == 1) {
      next_press_min = diff;
      next_press_max = diff;
      // std::println("only one option {}", diff);
      break;
    }
  }
  for (size_t npress_next = next_press_min; npress_next <= next_press_max;
       npress_next++) {
    auto new_jolts = jolts;
    for (auto ipos : swtch)
      new_jolts.at(ipos) -= npress_next;
    solve(order, ix + 1, switches, switch_by_position, new_jolts, npress_next,
          totpress + npress_next, minpress);
  }
}

size_t min_jolt_flips(const auto &jolts, const auto &switches) {
  const auto nj = jolts.size();
  const auto ns = switches.size();
  // const auto switch_by_position = map_switch_by_position(switches);
  const auto [order, switch_by_position] = get_switch_order(switches);
  // std::println("{}", order);
  size_t minpress{std::accumulate(jolts.begin(), jolts.end(), 0)};
  solve(order, 0, switches, switch_by_position, jolts, 0, 0, minpress);
  //std::println("{} {} {}", order, jolts, minpress);
  return minpress;
}

void part2(const std::string &fn) {
  const auto ip = aoc::read_file(fn) | std::views::transform(splitl) |
                  std::ranges::to<std::vector>();
  size_t ans = 0;
  for (const auto &[_, s, j] : ip) {
    const auto jolts = getnos(j);
    const auto switches =
        s | std::views::transform(getnos) | std::ranges::to<std::vector>();
    ans += min_jolt_flips(jolts, switches);
  }
  std::println("{}", ans);
}

int main() {
  part1("test_input");
  part1("input");
  part2("test_input");
  part2("input");
}
