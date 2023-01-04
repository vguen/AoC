#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct Tree
{
  bool from_left_   = false;
  bool from_right_  = false;
  bool from_above_  = false;
  bool from_below_  = false;
  uint8_t size_     = 0;
};

using TreeLine = std::vector<Tree>;
using TreeMap = std::vector<TreeLine>;

TreeMap read_input(std::string const & filename);
TreeMap map_tree_visibility(TreeMap & tree_map);
std::uint64_t count_visible_trees(TreeMap const & tree_map);
