#include <print>
#include <ranges>

#include "aoc.h"

using Vec3 = std::array<int64_t, 3>;
enum XYZ : size_t { X, Y, Z };

auto parse(const std::string &fn) {
  auto parse_line = [](const std::string &s) {
    const std::array<std::string, 3> nos = aoc::splitn<3, ','>(s);
    return Vec3{atoll(nos[X].c_str()), atoll(nos[Y].c_str()),
                atoll(nos[Z].c_str())};
  };
  return aoc::read_file(fn) | std::views::transform(parse_line) |
         std::ranges::to<std::vector>();
}

auto dsq(const Vec3 &pos1, const Vec3 &pos2) {
  const auto dx = pos1[X] - pos2[X];
  const auto dy = pos1[Y] - pos2[Y];
  const auto dz = pos1[Z] - pos2[Z];
  return dx * dx + dy * dy + dz * dz;
}

// using Dist = std::tuple<int64_t, Vec3, Vec3>;
using Dist = std::tuple<int64_t, size_t, size_t>;

auto add_cable(std::vector<std::set<size_t>> graphs, size_t i1, size_t i2) {
  std::set<size_t> connected_graphs;
  for (auto &&[i, g] : graphs | std::views::enumerate) {
    if (g.contains(i1) || g.contains(i2)) {
      g.insert(i1);
      g.insert(i2);
      connected_graphs.insert(i);
    }
  }
  if (connected_graphs.empty()) {
    graphs.push_back({i1, i2});
    return graphs;
  } else {
    std::vector<std::set<size_t>> new_graphs;
    std::set<size_t> new_set;
    for (auto i : connected_graphs) {
      aoc::merge(new_set, graphs[i]);
    }
    new_graphs.push_back(new_set);
    for (const auto &[i, g] : graphs | std::views::enumerate) {
      if (!connected_graphs.contains(i)) {
        new_graphs.push_back(g);
      }
    }
    return new_graphs;
  }
}

void both_parts(const std::string &fn, size_t nlinks) {
  const auto ip = parse(fn);
  // std::println("{}", ip);
  std::vector<Dist> dists;
  for (const auto &[i1, p1] : ip | std::views::enumerate) {
    for (const auto &[i2, p2] :
         ip | std::views::enumerate | std::views::drop(i1 + 1)) {
      if (i1 != i2) {
        dists.push_back({dsq(p1, p2), i1, i2});
      }
    }
  }
  std::ranges::sort(dists, [](const auto &d1, const auto &d2) {
    return std::get<0>(d1) < std::get<0>(d2);
  });
  // std::println("{}", dists | std::views::take(nlinks));
  // for (const auto &[d, i1, i2] : dists | std::views::take(nlinks))
  //  std::println("{} {} -> {} {} ({})", i1, ip[i1], i2, ip[i2], d);
  std::vector<std::set<size_t>> graphs;
  for (auto [_, i1, i2] : dists | std::views::take(nlinks)) {
    graphs = add_cable(graphs, i1, i2);
  }
  // std::println("{}, {} -> graphs : {}", i1, i2, graphs);
  // const auto ans =
  std::ranges::sort(graphs, [](const auto &g1, const auto &g2) {
    return g1.size() > g2.size();
  });
  // std::println("{}", graphs);
  size_t ans{1};
  for (const auto &g : graphs | std::views::take(3)) {
    // std::println("{}", g.size());
    ans *= g.size();
  }
  std::println("ans {}", ans);

  graphs.clear();
  for (const auto &[_, i1, i2] : dists) {
    graphs = add_cable(graphs, i1, i2);
    if (graphs.size() == 1 && graphs.at(0).size() == ip.size()) {
      std::println("ans2 = {}", ip.at(i1)[X] * ip.at(i2)[X]);
      break;
    }
  }
}

int main() {
  both_parts("test_input", 10);
  both_parts("input", 1000);
}