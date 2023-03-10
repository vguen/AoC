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

template<class InputIt, class UnaryFunction>
constexpr UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
{
  for (; first != last; ++first) {
    // TODO: decltype f param to know when it needs InputIt || InputIt::value
    f(first);
  }
 
  return f; // implicit move since C++11
}

template <typename Range, typename UnaryFunction>
constexpr UnaryFunction for_each(Range & range, UnaryFunction f)
{
  return for_each(std::begin(range), std::end(range));
}

static void lookup_scenic_view2(std::ranges::input_range auto && tree_line,
    std::function<void(TreeScenicView &, unsigned)> const & func)
{
  auto & first_tree_in_line = tree_line.front();
  // first_tree_in_line.view_dist_from_left_ = 0;
  func(first_tree_in_line, 0);

  for (auto tree_it = tree_line.begin() + 1; tree_it != tree_line.end() - 1; ++tree_it) {
    auto rev_it = std::make_reverse_iterator(tree_it);
    // tree_it->view_dist_from_left_ = std::count_if(rev_it, tree_line.rend(), [&tree_it](TreeScenicView const & prev) { return tree_it->size_ > prev.size_; });
    const auto scenic_count = std::count_if(rev_it, tree_line.rend(), [&tree_it](TreeScenicView const & prev) { return tree_it->size_ > prev.size_; });
    func(*tree_it, scenic_count);
  };

  auto & last_tree_in_line = tree_line.back();
  // last_tree_in_line.view_dist_from_left_ = 0;
  func(last_tree_in_line, 0);
}

static void dumb_lookup_scenic_view_left(std::vector<std::vector<TreeScenicView>> & tree_map,
    std::function<void(TreeScenicView &, unsigned)> const & func)
{
  auto i = 0U;
  std::for_each(tree_map.begin(), tree_map.end(), [&i, &func](std::vector<TreeScenicView> & tree_line) {
    auto & first_tree_in_line = tree_line.front();
    func(first_tree_in_line, 0);

    for (auto tree_it = tree_line.begin() + 1; tree_it != tree_line.end() - 1; ++tree_it) {
      auto rev_it = std::make_reverse_iterator(tree_it);
      auto scenic_count = 0U;
      for (; rev_it != tree_line.rend() && tree_it->size_ > rev_it->size_; ++rev_it) {
      ++scenic_count;
      };
      func(*tree_it, scenic_count);
    };

    auto & last_tree_in_line = tree_line.back();
    func(last_tree_in_line, 0);

    ++i;
  });
}

static void dumb_lookup_scenic_view_right(std::vector<std::vector<TreeScenicView>> & tree_map,
    std::function<void(TreeScenicView &, unsigned)> const & func)
{
  std::for_each(tree_map.begin(), tree_map.end(), [&func](std::vector<TreeScenicView> & tree_line) {
    auto & first_tree_in_line = tree_line.back();
    func(first_tree_in_line, 0);

    for (auto tree_it = tree_line.rbegin() + 1; tree_it != tree_line.rend() - 1; ++tree_it) {
      auto rev_it = tree_it.base();
      auto scenic_count = 1U;
      for (; rev_it != tree_line.end() && tree_it->size_ > rev_it->size_; ++rev_it) {
        ++scenic_count;
      };
      func(*tree_it, scenic_count);
    };

    auto & last_tree_in_line = tree_line.front();
    func(last_tree_in_line, 0);
  });
}

static void lookup_scenic_view(std::vector<std::vector<TreeScenicView>> & tree_map,
    std::function<void(TreeScenicView &, unsigned)> const & func)
{
  std::for_each(tree_map.begin(), tree_map.end(), [&func](std::vector<TreeScenicView> & tree_line) {
    auto & first_tree_in_line = tree_line.front();
    // first_tree_in_line.view_dist_from_left_ = 0;
    func(first_tree_in_line, 0);

    for (auto tree_it = tree_line.begin() + 1; tree_it != tree_line.end() - 1; ++tree_it) {
      auto rev_it = std::make_reverse_iterator(tree_it);
      // tree_it->view_dist_from_left_ = std::count_if(rev_it, tree_line.rend(), [&tree_it](TreeScenicView const & prev) { return tree_it->size_ > prev.size_; });
      const auto scenic_count = std::count_if(rev_it, tree_line.rend(), [&tree_it](TreeScenicView const & prev) { return tree_it->size_ > prev.size_; });
      func(*tree_it, scenic_count);
    };

    auto & last_tree_in_line = tree_line.back();
    // last_tree_in_line.view_dist_from_left_ = 0;
    func(last_tree_in_line, 0);
  });
}

static void lookup_scenic_view_left(std::vector<std::vector<TreeScenicView>> & tree_map)
{
  // std::for_each(tree_map.begin(), tree_map.end(), [](std::vector<TreeScenicView> & tree_line) {
  //   lookup_scenic_view2(tree_line, [](TreeScenicView & tree, unsigned scenic_score) { tree.view_dist_from_left_ += scenic_score; });
  // });
  dumb_lookup_scenic_view_left(tree_map, [](TreeScenicView & tree, unsigned scenic_score) { tree.view_dist_from_left_ += scenic_score; });
}

static void lookup_scenic_view_right(std::vector<std::vector<TreeScenicView>> & tree_map)
{
  // std::for_each(tree_map.begin(), tree_map.end(), [](std::vector<TreeScenicView> & tree_line) {
  //     // TODO?
  //   // auto & reverse_tree_line = tree_line | std::views::reverse;
  //   lookup_scenic_view2(tree_line, [](TreeScenicView & tree, unsigned scenic_score) { tree.view_dist_from_right_ += scenic_score; });
  // });
  dumb_lookup_scenic_view_left(tree_map, [](TreeScenicView & tree, unsigned scenic_score) { tree.view_dist_from_right_ += scenic_score; });
}

static void lookup_scenic_view_above_and_below(std::vector<std::vector<TreeScenicView>> & tree_map)
{
  swap_matrix(tree_map);

  // above is now left
  dumb_lookup_scenic_view_left(tree_map, [](TreeScenicView & tree, unsigned scenic_score) { tree.view_dist_from_above_ += scenic_score; });
  // below is now right
  dumb_lookup_scenic_view_right(tree_map, [](TreeScenicView & tree, unsigned scenic_score) { tree.view_dist_from_below_ += scenic_score; });

  // put once again in proper order
  swap_matrix(tree_map);
}

std::vector<std::vector<TreeScenicView>> compute_view_distance(std::vector<std::vector<TreeScenicView>> tree_map)
{
  lookup_scenic_view_left(tree_map);
  lookup_scenic_view_right(tree_map);
  lookup_scenic_view_above_and_below(tree_map);

  return tree_map;
}

uint64_t calc_max_scenic_view(std::vector<std::vector<TreeScenicView>> const & tree_map)
{
  using scenic_view_t = uint64_t;
  scenic_view_t max_scenic_view{0U};
  // for (auto const & tree_line : tree_map) {
    // for (auto const & tree : tree_line) {
    for (auto i = 0U; i < tree_map.size(); ++i) {
      auto & tree_line = tree_map[i];
      for (auto j = 0U; j < tree_line.size(); ++j) {
        auto & tree = tree_line[j];
      const scenic_view_t tree_scenic_view = tree.view_dist_from_left_ * tree.view_dist_from_above_ * tree.view_dist_from_below_ * tree.view_dist_from_right_;
      std::cout
        << "size = " << std::to_string(tree.size_) << " "
        << "[L] "    << tree.view_dist_from_left_
        << " * [A] " << tree.view_dist_from_above_
        << " * [B] " << tree.view_dist_from_below_
        << " * [R] " << tree.view_dist_from_right_
        << " = " << tree_scenic_view
        << "\n";
      max_scenic_view = std::max(max_scenic_view, tree_scenic_view);
      std::cout << "[" << i << ";" << j << "] max = " << max_scenic_view << "\n";
      if (max_scenic_view == 11054913) {
        throw 11054913;
      }
    }
  }
  return max_scenic_view;
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

