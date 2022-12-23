#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string_view>
#include <tuple>

constexpr auto MARKUP_SIZE{14U};

static void print_answer(size_t pos)
{
  std::cout << pos << "\n";
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

static bool markup_found(std::string_view const data)
{
  bool are_each_element_different{true};
  for (auto i = 0U; i < data.size() - 1; ++i) {
    for (auto j = i + 1; j < data.size(); ++j) {
      // std::cout << data[i] << " != " << data[j] << "\n";
      are_each_element_different = are_each_element_different && data[i] != data[j];
    }
    // std::cout << "\n";
  }
  return are_each_element_different;
}

static std::tuple<std::string_view, std::size_t> find_start_of_packet_markup(std::string_view const datastream)
{
  assert(datastream.size() >= MARKUP_SIZE);
  auto markup_begin = std::cbegin(datastream);
  auto markup_end = std::cbegin(datastream) + MARKUP_SIZE;
  size_t pos{0};

  while (markup_end != std::cend(datastream) && !markup_found({markup_begin, markup_end})) {
    ++pos;
    ++markup_begin;
    ++markup_end;
  }

  return { {markup_begin, markup_end}, pos + MARKUP_SIZE };
}

static std::string read_input(std::string const & filename)
{
  std::ifstream stream(filename);
  if (!stream) {
    throw std::runtime_error("could not open file [" + filename + "]");
  }

  std::string datastream;

  std::string line;
  while (std::getline(stream, line) && !line.empty()) {
    std::copy(std::cbegin(line), std::cend(line), std::back_inserter(datastream));
  }

  return datastream;
}

int main(int argc, char * argv[])
{
  const auto filename = use_cli_or_default_value(argc, argv);
  const auto datastream = read_input(filename);
  const auto [markup, pos] = find_start_of_packet_markup(datastream);
  print_answer(pos);
  return 0;
}
