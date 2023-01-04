#include <iostream>

#include "util.h"

static void print_answer(std::uint64_t number_of_trees_visible)
{
  std::cout << number_of_trees_visible << "\n";
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
  auto tree_map = read_input(filename);
  tree_map = map_tree_visibility(tree_map);
  const auto number_of_trees_visible = count_visible_trees(tree_map);
  print_answer(number_of_trees_visible);
  return 0;
}
