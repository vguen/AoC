#include <algorithm>
#include <cassert>
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

static uint64_t as_number(std::string_view const string_nb)
{
  uint64_t result{};

  auto [ptr, ec] { std::from_chars(string_nb.data(), string_nb.data() + string_nb.size(), result) };

  if (ec == std::errc())
  {
    return result;
  }
  else if (ec == std::errc::invalid_argument)
  {
    throw std::runtime_error(" isn't a number");
  }
  else if (ec == std::errc::result_out_of_range)
  {
    throw std::runtime_error(" is larger than an uint64_t");
  }
  throw std::runtime_error("Should not reach here");
}

static std::vector<uint64_t> get_elf_package(std::ifstream & stream)
{
  std::vector<uint64_t> elf;
  std::string line;
  while (std::getline(stream, line) && !line.empty()) {
    // std::cout << "line: " << std::quoted(line) << "\n";
    elf.push_back(as_number(line));
  }
  return elf;
}

static std::vector<std::vector<uint64_t>> read_the_full_file(std::string const & filename)
{
  std::ifstream input(filename);
  if (!input) {
    throw std::runtime_error("could not open file [" + filename + "]");
  }
  std::vector<std::vector<uint64_t>> elfs;
  auto elf = get_elf_package(input);
  while (!elf.empty()) {
    // std::cout << "#" << elfs.size() << "\n";
    elfs.emplace_back(elf);
    elf = get_elf_package(input);
  }
  return elfs;
}

static std::vector<uint64_t> sort_elf_by_the_most_calories_carried(std::vector<std::vector<uint64_t>> const & elfs)
{
  std::vector<uint64_t> elfs_sorted_by_total_calories_carried(elfs.size());

  std::transform(std::cbegin(elfs), std::cend(elfs),
    std::begin(elfs_sorted_by_total_calories_carried),
    [](std::vector<uint64_t> const & elf) -> uint64_t {
      return std::accumulate(std::cbegin(elf), std::cend(elf), 0UL);
  });

  std::sort(std::begin(elfs_sorted_by_total_calories_carried), std::end(elfs_sorted_by_total_calories_carried), std::greater<>());

  return elfs_sorted_by_total_calories_carried;
}
static void part1(std::vector<uint64_t> const & elfs_sorted_by_total_calories_carried)
{
  std::cout << "=== part1 ===" << "\n";
  assert(elfs_sorted_by_total_calories_carried[0] == 70698);

  std::cout << elfs_sorted_by_total_calories_carried[0] << "\n";
}

static void part2(std::vector<uint64_t> const & elfs_sorted_by_total_calories_carried)
{
  std::cout << "=== part2 ===" << "\n";
  const uint64_t total_calories_carried_by_the_3 = std::accumulate(std::cbegin(elfs_sorted_by_total_calories_carried), std::begin(elfs_sorted_by_total_calories_carried) + 3, 0UL);
  assert(total_calories_carried_by_the_3 == 206643);
  std::cout << total_calories_carried_by_the_3 << "\n";
}

int main()
{
  try {
    const auto elfs = read_the_full_file("input");
    const auto elfs_sorted_by_total_calories_carried = sort_elf_by_the_most_calories_carried(elfs);

    part1(elfs_sorted_by_total_calories_carried);
    part2(elfs_sorted_by_total_calories_carried);
    return 0;
  }
  catch (std::exception const & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
