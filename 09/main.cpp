#include <print>
#include <ranges>

#include "aoc.h"

using Vec2 = std::array<int64_t, 2>;

auto parse(const std::string &fn) {
  auto parse_line = [](const std::string &s) {
    const std::array<std::string, 2> nos = aoc::splitn<2, ','>(s);
    return Vec2{atoll(nos[0].c_str()), atoll(nos[1].c_str())};
  };
  return aoc::read_file(fn) | std::views::transform(parse_line) |
         std::ranges::to<std::vector>();
}

auto area(const Vec2 &pos1, const Vec2 &pos2) {
  const auto dx = std::abs(pos1[0] - pos2[0]) + 1;
  const auto dy = std::abs(pos1[1] - pos2[1]) + 1;
  return dx * dy;
}

auto get_areas(const std::vector<Vec2> &ip) {
  std::vector<std::tuple<int64_t, Vec2, Vec2>> areas;
  for (const auto &[i1, p1] : ip | std::views::enumerate) {
    for (const auto &[i2, p2] :
         ip | std::views::enumerate | std::views::drop(i1 + 1)) {
      areas.push_back({area(p1, p2), p1, p2});
      // println("{} -> {} = {}", p1, p2, area(p1, p2));
    }
  }
  std::ranges::sort(areas);
  return areas;
}

void part1(const std::string &fn) {
  const auto ip = parse(fn);
  const auto areas = get_areas(ip);
  const auto maxa = std::get<0>(areas.back());
  std::println("day 09 part 1 = {} ({})", maxa, fn);
}

bool is_rg(const auto &lines, const Vec2 &c1, const Vec2 &c2) {
  const auto [x1, x2] = std::minmax(c1[0], c2[0]);
  const auto [y1, y2] = std::minmax(c1[1], c2[1]);
  auto is_overlap = [](int64_t a0, int64_t a1, int64_t b0, int64_t b1) {
    return a0 < b1 && b0 < a1;
  };
  for (const auto [v1, v2] : lines) {
    if (v1[0] == v2[0]) {
      const auto lx = v1[0];
      const auto [ly1, ly2] = std::minmax(v1[1], v2[1]);
      if (lx > x1 && lx < x2 && is_overlap(ly1, ly2, y1, y2)) {
        return false;
      }
    } else {
      const auto ly = v1[1];
      const auto [lx1, lx2] = std::minmax(v1[0], v2[0]);
      if (ly > y1 && ly < y2 && is_overlap(lx1, lx2, x1, x2)) {
        return false;
      }
    }
  }
  return true;
}

void part2(const std::string &fn) {
  const auto ip = parse(fn);
  const auto areas = get_areas(ip);

  // create a wrapped-around version of input as lines (pairs of points)
  const auto lines = std::views::concat(ip, std::views::single(ip.front())) |
                     std::views::pairwise;

  const auto result =
      std::ranges::find_if(areas | std::views::reverse, [&](const auto &t) {
        return is_rg(lines, std::get<1>(t), std::get<2>(t));
      });

  std::println("day 09 part 2 = {} ({})", std::get<0>(*result), fn);
}

int main() {
  part1("test_input");
  part1("input");
  part2("test_input");
  part2("input");
}