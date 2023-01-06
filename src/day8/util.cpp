#include "util.h"

#include <algorithm>
#include <cassert>
#include <charconv>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

template <class Matrix>
static void swap_matrix(Matrix & matrix);

static std::vector<Tree> line_to_vector(std::string & string);

static void lookup_line_trees_from_left(
  TreeMap & tree_map,
  std::function<void(Tree &)> const & functor);
static void lookup_line_trees_from_right(
  TreeMap & tree_map,
  std::function<void(Tree &)> const & functor);
static void lookup_line_trees_from_above(
  TreeMap & tree_map,
  std::function<void(Tree &)> const & functor);
static void lookup_line_trees_from_below(
  TreeMap & tree_map,
  std::function<void(Tree &)> const & functor);

static void map_tree_visibility(auto & begin, auto & end,
  std::function<void(Tree &)> const & functor);

///

TreeMap read_input(std::string const & filename)
{
  std::ifstream stream(filename);
  if (!stream) {
    throw std::runtime_error("could not open file [" + filename + "]");
  }

  TreeMap tree_map;

  std::string line;
  while (std::getline(stream, line) && !line.empty()) {
    tree_map.push_back(line_to_vector(line));
  }
  return tree_map;
}

TreeMap map_tree_visibility(TreeMap & tree_map)
{
  auto maped_trees = tree_map;

  lookup_line_trees_from_left(maped_trees,  [](Tree & tree) { tree.from_left_  = true; });
  lookup_line_trees_from_right(maped_trees, [](Tree & tree) { tree.from_right_ = true; });
  lookup_line_trees_from_above(maped_trees, [](Tree & tree) { tree.from_above_ = true; });
  lookup_line_trees_from_below(maped_trees, [](Tree & tree) { tree.from_below_ = true; });

  return maped_trees;
}

std::uint64_t count_visible_trees(TreeMap const & tree_map)
{
  auto nb_visible_trees{0U};
  
  for (auto const & tree_line : tree_map) {
    nb_visible_trees += std::count_if(tree_line.cbegin(), tree_line.cend(), [](Tree const & tree) { return tree.from_left_ || tree.from_right_ || tree.from_above_ || tree.from_below_; });
  }
  return nb_visible_trees;
}

///
/// part 2
///

std::vector<std::vector<TreeScenicView>> compute_view_distance(std::vector<std::vector<TreeScenicView>> const & tree_map)
{
  return {};
}

uint64_t calc_max_scenic_view(std::vector<std::vector<TreeScenicView>> const & tree_map)
{
  return 0;
}


///

static std::vector<Tree> line_to_vector(std::string & string)
{
  std::vector<Tree> vec;
  vec.resize(string.size());
  for (auto i = 0U; i < string.size(); ++i) {
    vec[i].size_ = string[i] - '0';
  }
  return vec;
  // return string | std::views::transform([](char tree_symb) -> uint8_t { return tree_symb - '0'; })
}

template <class Matrix>
static void swap_matrix(Matrix & matrix)
{
  const auto n = matrix.size();
  for (auto i = 0U; i < n; i++) {
      for (auto j = i + 1; j < n; j++) {
          std::swap(matrix[i][j], matrix[j][i]);
      }
  }
}

static void map_tree_visibility(
  auto const & begin,
  auto const & end,
  std::function<void(Tree &)> const & functor)
{
  auto it = begin;
  // *First* tree in the line is always visible
  functor(*it);

  auto last_tallest_tree = it;

  for (it = ++it; it != end -1; ++it) {
    if (it->size_ > last_tallest_tree->size_) {
      functor(*it);
      last_tallest_tree = it;
    }
  }

  // *Last* tree in the line is always visible
  functor(*it);

  assert(++it == end);
}

static void lookup_line_trees_from_left(
  TreeMap & tree_map,
  std::function<void(Tree &)> const & functor)
{
  auto tree_line = tree_map.begin();

  // Trees in the *first* line are always visible
  for (auto & tree_in_first_line : *(tree_line)) {
    functor(tree_in_first_line);
  }

  ++tree_line;

  for (; tree_line != tree_map.end() - 1; ++tree_line) {
    map_tree_visibility(tree_line->begin(), tree_line->end(), functor);
  }

  // Trees in the *last* line are always visible
  for (auto & tree_in_last_line : *(tree_line)) {
    functor(tree_in_last_line);
  }

  assert(++tree_line == tree_map.end());
}

static void lookup_line_trees_from_right(
  TreeMap & tree_map,
  std::function<void(Tree &)> const & functor)
{
  auto tree_line = tree_map.rbegin();

  // Trees in the *first* line are always visible
  for (auto & tree_in_first_line : *(tree_line)) {
    functor(tree_in_first_line);
  }

  ++tree_line;

  for (; tree_line != tree_map.rend() - 1; ++tree_line) {
    map_tree_visibility(tree_line->rbegin(), tree_line->rend(), functor);
  }

  // Trees in the *last* line are always visible
  for (auto & tree_in_last_line : *(tree_line)) {
    functor(tree_in_last_line);
  }

  assert(++tree_line == tree_map.rend());
}

static void lookup_line_trees_from_above(
  TreeMap & tree_map,
  std::function<void(Tree &)> const & functor)
{
  swap_matrix(tree_map);
  lookup_line_trees_from_left(tree_map, functor);
  swap_matrix(tree_map);
}

static void lookup_line_trees_from_below(
  TreeMap & tree_map,
  std::function<void(Tree &)> const & functor)
{
  swap_matrix(tree_map);
  lookup_line_trees_from_right(tree_map, functor);
  swap_matrix(tree_map);
}

