#include <array>
#include <algorithm>
#include <cassert>
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <tuple>

struct Section
{
  uint64_t begin_;
  uint64_t size_;
};

using PairSection = std::array<Section, 2>;

static void print_answer(uint64_t answer)
{
  std::cout << answer << "\n";
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

static std::tuple<uint64_t, char const *, std::ptrdiff_t> get_number(std::string_view line)
{
  uint64_t number{};
  auto [ptr, ec] { std::from_chars(line.data(), line.data() + line.size(), number) };
 
  if (ec == std::errc())
  {
    const auto ptr_size = line.size() - (ptr - line.data());
    return { number, ptr, ptr_size };
  }
  else if (ec == std::errc::invalid_argument)
  {
    std::stringstream ss;
    ss << std::quoted(line) << " isn't a number";
    throw std::runtime_error(ss.str());
  }
  else if (ec == std::errc::result_out_of_range)
  {
    std::stringstream ss;
    ss << std::quoted(line) << " is larger than an uint64_t";
  }
  throw std::runtime_error("Unhandled from_chars error");
}

static std::vector<PairSection> read_sections_from_file(std::string filename)
{
  std::ifstream stream(filename);
  if (!stream) {
    throw std::runtime_error("could not open file [" + filename + "]");
  }

  std::vector<PairSection> sections;

  std::string line;
  while (std::getline(stream, line) && !line.empty()) {

    std::array<Section, 2> pair_sections;
    char const * ptr_to_line = line.data();
    std::size_t ptr_size = line.size();

    for (auto i = 0u; i < 2; ++i) {

      uint64_t begin_section{};
      std::tie(begin_section, ptr_to_line, ptr_size) = get_number({ ptr_to_line, ptr_size });

      assert(ptr_to_line[0] == '-');
      ptr_to_line = &(ptr_to_line[1]);
      --ptr_size;

      uint64_t end_section{};
      std::tie(end_section, ptr_to_line, ptr_size) = get_number({ ptr_to_line, ptr_size });

      assert(begin_section <= end_section);
      const auto section_size = end_section - begin_section;
      pair_sections[i] = { begin_section, section_size };

      if (i == 0) {
        assert(ptr_to_line[0] == ',');
        ++ptr_to_line;
        --ptr_size;
      }
      else if (i == 1) 
      {
        assert(ptr_size == 0);
      }

    }

    sections.push_back(pair_sections);
  }

  return sections;
}

static std::tuple<Section, Section> find_longuest_section(PairSection const & section_pair)
{
  auto longuest_section = section_pair[0];
  auto shortest_section = section_pair[1];
  if (shortest_section.size_ > longuest_section.size_) {
    std::swap(longuest_section, shortest_section);
  }
  return { longuest_section, shortest_section };
}

static bool is_section_overlapping(PairSection const & pair_section)
{
  const auto & [longuest_section, shortest_section] = find_longuest_section(pair_section);
  const auto longuest_section_end = longuest_section.begin_ + longuest_section.size_;
  const auto shortest_section_end = shortest_section.begin_ + shortest_section.size_;
  return longuest_section.begin_ <= shortest_section.begin_ &&
          shortest_section_end <= longuest_section_end;
}

static std::vector<PairSection> find_overlapping_sections(std::vector<PairSection> const & sections)
{
  std::vector<PairSection> overlapping_sections;
  overlapping_sections.reserve(sections.size());

  for (auto const & section : sections | std::views::filter(is_section_overlapping)) {
    overlapping_sections.push_back(section);
  }

  return overlapping_sections;
}

static uint64_t count_overlapping_sections(std::vector<PairSection> const & overlapping_sections)
{
  return overlapping_sections.size();
}

int main(int argc, char * argv[])
{
  const auto filename = use_cli_or_default_value(argc, argv);
  const auto sections = read_sections_from_file(filename);
  const auto fully_overlapped_pairs = find_overlapping_sections(sections);
  const auto nb_overlapping_sections = count_overlapping_sections(fully_overlapped_pairs);
  print_answer(nb_overlapping_sections);
  return 0;
}

