#include <functional>
#include <print>
#include <ranges>

#include "aoc.h"

auto transpose(const std::vector<std::vector<std::string>> &ip) {
  const auto ncol = ip.at(0).size();
  const auto nrow = ip.size();
  std::vector op{ncol, std::vector<std::string>{nrow, ""}};
  for (size_t i = 0; i < nrow; i++) {
    for (size_t j = 0; j < ncol; j++) {
      op.at(j).at(i) = ip.at(i).at(j);
    }
  }
  return op;
}

auto transpose(const std::vector<std::string> &ip) {
  const auto ncol = ip.at(0).size();
  const auto nrow = ip.size();
  std::vector op{ncol, std::string(nrow, ' ')};
  for (size_t i = 0; i < nrow; i++) {
    for (size_t j = 0; j < ncol; j++) {
      op.at(j).at(i) = ip.at(i).at(j);
    }
  }
  return op;
}

auto parse(const std::string &fn) {
  std::println("debug");
  auto split = [](const std::string &line) {
    return aoc::split(line, ' ') |
           std::views::filter([](const std::string &s) { return !s.empty(); }) |
           std::ranges::to<std::vector>();
  };
  const auto ip = aoc::read_file(fn) | std::views::transform(split) |
                  std::ranges::to<std::vector>();
  const auto ncol = ip.at(0).size();
  assert(
      std::ranges::all_of(ip, [&](const auto &r) { return r.size() == ncol; }));
  return ip;
}

void part1(const std::string &fn) {
  const auto ip = parse(fn);
  const auto ncol = ip.at(0).size();
  const auto nrow = ip.size();
  // std::println("{} {}", nrow, ncol);
  // std::println("{}", ip);
  size_t ans{};
  for (auto ic = 0UL; ic < ncol; ic++) {
    auto i = atoll(ip.at(0).at(ic).c_str());
    const auto op = ip.back().at(ic).at(0);
    // std::println("{} {}", i, op);
    for (auto ir = 1UL; ir + 1 < nrow; ir++) {
      const auto j = atoll(ip.at(ir).at(ic).c_str());
      if (op == '*')
        i *= j;
      else if (op == '+')
        i += j;
      else
        throw std::runtime_error("bad operation");
      // std::println("{} {}", i, j);
    }
    // std::println("{}", i);
    ans += i;
  }
  std::println("{}", ans);
}

size_t solve_problem(const auto &problem) {
  const char op = problem.front().back();
  const auto nrow = problem.front().size();
  auto toz = [&](const std::string &s) {
    const auto i = static_cast<size_t>(atoll(aoc::trim(s.substr(0,nrow - 1)).c_str()));
    //std::println("toz => {} {}",s,i);
    return i;
  };
  /*
  if (op == '*') {
    return std::ranges::fold_left_first(problem | std::views::transform(toz),
                                        std::multiplies());
  } else if (op == '+') {
    return std::ranges::fold_left_first(problem | std::views::transform(toz),
                                        std::plus());
  }*/
  
  if (op == '*') {
    size_t ans = 1;
    for(const std::string & s : problem) ans *= toz(s);
    //std::println("* {}",ans);
    return ans;
  } else if (op == '+') {
    size_t ans = 0;
    for(const std::string & s : problem) ans += toz(s);
    //std::println("+ {}",ans);
    return ans;
  }
  throw std::runtime_error("bad op?");
}

void part2(const std::string &fn) {
  const auto ip = aoc::read_file(fn);
  const auto nrow = ip.size();
  const auto ncol = ip.at(0).size();

  std::println("{}", transpose(ip));
  const std::string empty(nrow, ' ');
  const auto problems =
      transpose(ip) |
      std::views::lazy_split(empty);// | std::ranges::to<std::vector>();
  std::println("{}", problems);

  //const auto ans = std::ranges::fold_left(
  //    ip | std::views::transform(solve_problem), 0UL, std::plus());
  size_t ans = 0;
  for (const auto & problem : problems) ans += solve_problem(problem);
  std::println("{}", ans);
}

int main() {
  part1("test_input");
  part1("input");
  part2("test_input");
  part2("input");
}