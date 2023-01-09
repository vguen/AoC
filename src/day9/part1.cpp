#include <iostream>

#include "util.h"

static void print_answer(uint64_t nb_tail_position_visited)
{
  std::cout << nb_tail_position_visited << "\n";
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
  const auto movements = read_input(filename);
  const auto [head_positions, tail_positions] = move_head_and_tail(movements);
  const auto nb_tail_position_visited = count_position_visited_at_least_once(tail_positions);
  print_answer(nb_tail_position_visited);
  return 0;
}
