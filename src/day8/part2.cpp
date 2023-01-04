#include <fstream>
#include <iostream>

#include "util.h"

static void print_answer(uint64_t max_scenic_view)
{
  std::cout << max_scenic_view << "\n";
}

static std::vector<TreeScenicView> line_to_vector(std::string & string)
{
  std::vector<TreeScenicView> vec;
  vec.resize(string.size());
  for (auto i = 0U; i < string.size(); ++i) {
    vec[i].size_ = string[i] - '0';
  }
  return vec;
  // return string | std::views::transform([](char tree_symb) -> uint8_t { return tree_symb - '0'; })
}

static std::vector<std::vector<TreeScenicView>> read_input_part2(std::string const & filename)
{
  std::ifstream stream(filename);
  if (!stream) {
    throw std::runtime_error("could not open file [" + filename + "]");
  }

  std::vector<std::vector<TreeScenicView>> tree_map;

  std::string line;
  while (std::getline(stream, line) && !line.empty()) {
    tree_map.push_back(line_to_vector(line));
  }
  return tree_map;
}

static std::string use_cli_or_default_value(int argc, char * argv[])
{
  if (argc > 2) {
    std::cerr << "part2 [filename]" << "\n";
    std::exit(1);
  } else if (argc == 2) {
    return argv[1];
  } else {
    return "input";
  }
  throw std::runtime_error("Should not end up here");
}

int main(int argc, char * argv[])
{
  const auto filename = use_cli_or_default_value(argc, argv);
  auto tree_map = read_input_part2(filename);
  tree_map = compute_view_distance(tree_map);
  const auto max_scenic_view = calc_max_scenic_view(tree_map);
  print_answer(max_scenic_view);
  return 0;
}

