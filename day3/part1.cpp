#include <algorithm>
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
    const auto middle_string = line.size() / 2;
    const auto middle_it = std::cbegin(line) + middle_string;
    rucksacks.push_back({
      .left_  = std::string(std::cbegin(line), middle_it),
      .right_ = std::string(middle_it, std::cend(line))
    });
  }
  return rucksacks;
}

static std::vector<char> find_redoundant_items(std::vector<Rucksack> & rucksacks)
{
  std::vector<char> redoundant_items(rucksacks.size());

  return redoundant_items;
}

static void print_answer()
{
  std::cout << "\n";
}

int main()
{
  const auto rucksacks = read_the_full_file("input");
  const auto redoundant_items = find_redoundant_items(rucksacks);
  print_answer();
  return 0;
}
