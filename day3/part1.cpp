#include <algorithm>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Rucksack
{
  std::string left_;
  std::string right_;
};

static std::vector<Rucksack> read_the_full_file(std::string const & filename)
{
  std::ifstream stream(filename);
  if (!stream) {
    throw std::runtime_error("could not open file [" + filename + "]");
  }
  std::vector<Rucksack> rucksacks;
  std::string line;
  while (std::getline(stream, line) && !line.empty()) {
    // std::cout << line << ": ";
    const auto middle_string = line.size() / 2;
    const auto middle_it = std::cbegin(line) + middle_string;
    // std::cout << "[" << std::string(std::cbegin(line), middle_it) << "] [" << std::string(middle_it, std::cend(line)) << "]" << "\n";
    rucksacks.push_back({
      .left_  = std::string(std::cbegin(line), middle_it),
      .right_ = std::string(middle_it, std::cend(line))
    });
  }
  return rucksacks;
}

static uint64_t item_to_bitset_pos(char item)
{
  int64_t pos{-1};
  if (item >= 'a' && item <= 'z') {
    pos = item - 'a';
  } else if (item >= 'A' && item <= 'Z') {
    pos = item - 'A' + 25 + 1;
  }
  assert(pos > 0);
  return static_cast<uint64_t>(pos);
}

static std::vector<char> find_redoundant_items(std::vector<Rucksack> const & rucksacks)
{
  std::vector<char> redoundant_items(rucksacks.size());

  // I want a 52 size bitset (2 times the alphabet size)
  constexpr auto bitset_size = ('z' - 'a' + 1) + ('Z' - 'A' + 1);

  for (auto const & rucksack : rucksacks) {
    std::bitset<bitset_size> bitset;

    for (auto const item : rucksack.left_) {
      auto pos = item_to_bitset_pos(item);
      bitset.set(pos);
    }

    for (auto const item : rucksack.right_) {
      auto pos = item_to_bitset_pos(item);
      if (bitset.test(pos)) {
        redoundant_items.push_back(item);
        break;
      }
    }
  }

  return redoundant_items;
}

static uint64_t calculate_sum_priorities(std::vector<char> const & redoundant_items)
{
  uint64_t sum{0};

  for (auto const item : redoundant_items) {
    uint32_t value{0};
    if (item >= 'a' && item <= 'z') {
      value = item - 'a' + 1;
    } else if (item >= 'A' && item <= 'Z') {
      value = item - 'A' + 26 + 1;
    }
    sum += value;
  }

  return sum;
}

static void print_answer(uint64_t sum_of_priorities)
{
  std::cout << sum_of_priorities << "\n";
}

int main()
{
  const auto rucksacks = read_the_full_file("input");
  const auto redoundant_items = find_redoundant_items(rucksacks);
  const auto sum_of_priorities = calculate_sum_priorities(redoundant_items);
  print_answer(sum_of_priorities);
  return 0;
}
