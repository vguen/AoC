#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

using Rucksack = std::string;
using ElfGroup = std::vector<Rucksack>;
using Badge = char;

// I want a 52 size bitset (2 times the alphabet size)
constexpr auto BITSET_SIZE = ('z' - 'a' + 1) + ('Z' - 'A' + 1);
using Bitset = std::bitset<BITSET_SIZE>;

static ElfGroup read_elf_group(std::ifstream & stream)
{
  std::string line;
  std::vector<Rucksack> elf_group;
  elf_group.resize(3);
  for (auto i{0u}; i < 3 && std::getline(stream, line) && !line.empty(); ++i) {
    elf_group[i] = line;
  }
  return elf_group;
}

static bool is_elf_group_empty(ElfGroup const & elf_group)
{
  return !elf_group[0].empty() && !elf_group[1].empty() && !elf_group[2].empty();
}

static std::vector<ElfGroup> read_the_full_file(std::string const & filename)
{
  std::ifstream stream(filename);
  if (!stream) {
    throw std::runtime_error("could not open file [" + filename + "]");
  }
  std::vector<ElfGroup> elf_groups;
  auto elf_group = read_elf_group(stream);
  while (is_elf_group_empty(elf_group)) {
    elf_groups.push_back(elf_group);
    elf_group = read_elf_group(stream);
  }
  return elf_groups;
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

static char bitset_pos_to_item(uint64_t pos)
{
  static_assert(BITSET_SIZE % 2 == 0, "BITSET_SIZE must be even");
  constexpr auto half_bitset_size = BITSET_SIZE / 2;
  static_assert(half_bitset_size == 26);
  // WHILE pos is UNSIGNED it is redoundant to check
  if (/* pos >= 0 && */ pos <= half_bitset_size - 1) {
    return pos + 'a';
  } else if (pos >= half_bitset_size && pos <= BITSET_SIZE - 1) {
    // pos begins at 26 (do not forget it is 0 indexed, alphabet[0..25].size() == 26)
    // but we need to get the index in the alphabet, so we substract the size 
    // of the first alphabet
    return pos - half_bitset_size + 'A';
  }
  throw std::runtime_error("Unhandle case, should not reach here");
}

static void fill_bitset_with_rucksack(Bitset & bitset, Rucksack const & rucksack)
{
  for (auto const item : rucksack) {
    const auto pos = item_to_bitset_pos(item);
    bitset.set(pos);
  }
}

static std::pair<char, uint64_t> find_item_common_in_each_rucksack(std::array<std::bitset<BITSET_SIZE>, 3> const & bitsets)
{
  auto bitset_badge = bitsets[0] & bitsets[1] & bitsets[2];

  // Check that the new bitset only contains one element
  assert(bitset_badge.count() == 1);
  const auto pos = std::countr_zero(bitset_badge.to_ullong());
  return { bitset_pos_to_item(pos), pos };
}

static std::vector<std::pair<Badge, uint64_t>> find_badges_in_elf_groups(std::vector<ElfGroup> const & elf_groups)
{
  std::vector<std::pair<Badge, uint64_t>> badges;
  badges.reserve(elf_groups.size());

  for (auto const & elf_group : elf_groups) {
    std::array<Bitset, 3> bitsets;

    for (auto i{0u}; i < 3; ++i) {
      fill_bitset_with_rucksack(bitsets[i], elf_group[i]);
    }

    const auto [badge, priority] = find_item_common_in_each_rucksack(bitsets);
    // increment priority since it is zero indexed
    badges.push_back(std::make_pair(badge, priority + 1));
  }

  return badges;
}

static uint64_t calculate_priority_sum(std::vector<std::pair<char, uint64_t>> const & badge_list)
{
  return std::accumulate(std::cbegin(badge_list), std::cend(badge_list),
    0u,
    [i = 0u](uint64_t sum, std::pair<char, uint64_t> const & badge) mutable { 
      return sum + badge.second; }
  );
}

static void print_answer(uint64_t priority_sum)
{
  std::cout << priority_sum << "\n";
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
  const auto elf_groups = read_the_full_file(filename);
  const auto badge_list = find_badges_in_elf_groups(elf_groups);
  const auto priority_sum = calculate_priority_sum(badge_list);
  print_answer(priority_sum);
  return 0;
}
