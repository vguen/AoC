#include <algorithm>
#include <cassert>
#include <charconv>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

struct Order
{
  uint64_t nb_to_move_;
  uint64_t stack_src_;
  uint64_t stack_dest_;
};

using Stacks = std::vector<std::deque<char>>;
using Orders = std::vector<Order>;

static void print_answer(std::string_view answer)
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

static void parse_stacks_line(
  std::string::iterator & word_it,
  std::string & line,
  std::vector<std::deque<char>> & dequed_stacks,
  std::function<bool(char)> const & is_stack_begin)
{
  constexpr auto STACK_DISTANCE{4u};
  constexpr auto STACK_DISPLAY_SIZE{3u};

  while (word_it != std::end(line)) {
    assert(word_it[0] == '[');
    // assert(word_it is UPPER_LETTER)
    assert(word_it[2] == ']');

    const uint64_t pos_in_line = static_cast<uint64_t>(std::distance(std::begin(line), word_it));
    const auto stack_idx = pos_in_line / STACK_DISTANCE;

    // std::cout
    //   << "pos_in_line = " << pos_in_line << " "
    //   << "stack_idx = " << stack_idx << " "
    //   << "word_it[1] = " << word_it[1]
    //   << "\n";

    if (dequed_stacks.size() <= stack_idx) {
      dequed_stacks.resize(stack_idx + 1);
    }
    dequed_stacks[stack_idx].push_back(word_it[1]);

    if (pos_in_line + STACK_DISPLAY_SIZE < line.size()) {
      std::advance(word_it, STACK_DISPLAY_SIZE);
    } else {
      word_it = line.end();
    }
    word_it = std::ranges::find_if(word_it, line.end(), is_stack_begin);
  }
}

static Stacks read_stacks(std::ifstream & stream)
{
  std::vector<std::deque<char>> dequed_stacks;

  constexpr auto is_stack_begin = [](char c){ return c == '['; };
  constexpr auto is_number = [](char c){ return '0' <= c && c <= '9'; };

  std::string line;
  while (std::getline(stream, line) && !line.empty()) {
    auto word_it = std::ranges::find_if(line, is_stack_begin);

    if (word_it == std::end(line)) {
      // TODO: if right at the beginning word_it == std::end() then
      // check that the line contains the dequed_stacks number, check that those numbers match their position and the number of dequed_stacks you have
      // then check that next line is empty
      auto number_it = std::ranges::find_if(line, is_number);
      if (number_it == std::end(line)) {
        throw std::runtime_error("Missing line with the stacks number");
      }
    } else {
      parse_stacks_line(word_it, line, dequed_stacks, is_stack_begin);
    }
  }

  assert(dequed_stacks.size() > 0);

  return dequed_stacks;
}

static std::tuple<int64_t, std::string> parse_number(std::string const & str)
{
  int64_t result{};
 
  auto [ptr, ec] { std::from_chars(str.data(), str.data() + str.size(), result) };

  if (ec == std::errc())
  {
    const auto pos = ptr - str.data();
    std::string new_string(std::begin(str) + pos, std::end(str));
    return { result, new_string };
  }
  else if (ec == std::errc::invalid_argument)
  {
    std::cout << "That isn't a number.\n";
  }
  else if (ec == std::errc::result_out_of_range)
  {
    std::cout << "This number is larger than an int.\n";
  }

  throw std::runtime_error("Should not reach here");
}

static std::tuple<uint64_t, std::string> parse_unsigned_number(std::string const & str)
{
  const auto [result, ptr] = parse_number(str);
  assert(result > 0);
  return { static_cast<uint64_t>(result), ptr };
}

static Orders read_orders(std::ifstream & stream)
{
  constexpr auto MOVE_TOKEN_SIZE{4u};
  constexpr auto FROM_TOKEN_SIZE{4u};
  constexpr auto TO_TOKEN_SIZE{2u};

  Orders orders;

  std::string line;
  while (std::getline(stream, line) && !line.empty()) {

    const std::string begin_nb_to_move(std::cbegin(line) + MOVE_TOKEN_SIZE + 1, std::cend(line));
    const auto [nb_to_move, from_stack_str] = parse_unsigned_number(begin_nb_to_move);

    const std::string advance_from_stack_str(std::cbegin(from_stack_str) + 1 + FROM_TOKEN_SIZE + 1, std::cend(from_stack_str));

    // std::cout
    //   << "nb_to_move = " << nb_to_move << " | "
    //   << "from_stack_str = " << std::quoted(from_stack_str) << " | "
    //   << "advance_from_stack_str = " << std::quoted(advance_from_stack_str)
    //   << "\n";

    const auto [stack_src, dest_stack_str] = parse_unsigned_number(advance_from_stack_str);

    const std::string advance_dest_stack_str(std::begin(dest_stack_str) + 1 + TO_TOKEN_SIZE + 1, std::cend(dest_stack_str));

    // std::cout
    //   << "stack_src = " << stack_src << " | "
    //   << "dest_stack_str = " << std::quoted(dest_stack_str) << " | "
    //   << "advance_dest_stack_str = " << std::quoted(advance_dest_stack_str)
    //   << "\n";

    const auto [stack_dest, _] = parse_unsigned_number(advance_dest_stack_str);

    // std::cout << "stack_dest = " << stack_dest << "\n";
    // std::cout << "\n";

    orders.push_back({
      .nb_to_move_ = nb_to_move,
      .stack_src_ = stack_src - 1,
      .stack_dest_ = stack_dest - 1
    });
  }

  return orders;
}

static std::tuple<Stacks, Orders> read_input(std::string const & filename)
{
  std::ifstream stream(filename);
  if (!stream) {
    throw std::runtime_error("could not open file [" + filename + "]");
  }

  auto stacks = read_stacks(stream);

  auto orders = read_orders(stream);
  // Orders orders;

  // orders.push_back({ .nb_to_move_ = 1, .stack_src_ = 2 - 1, .stack_dest_ = 1 - 1});
  // orders.push_back({ .nb_to_move_ = 3, .stack_src_ = 1 - 1, .stack_dest_ = 3 - 1});
  // orders.push_back({ .nb_to_move_ = 2, .stack_src_ = 2 - 1, .stack_dest_ = 1 - 1});
  // orders.push_back({ .nb_to_move_ = 1, .stack_src_ = 1 - 1, .stack_dest_ = 2 - 1});

  return { stacks, orders };
}

// static std::string stack_to_string(std::deque<char> stack)
// {
//   std::string s{"{ "};
//   s.reserve(stack.size());
//   for (auto i = stack.size(); i > 0; --i) {
//     s += stack.front() + std::string(", ");
//     stack.pop_front();
//   }
//   s += "}";
//   return s;
// }

static Stacks rearrange_crates(Stacks stacks, Orders const & orders)
{
  for (auto const & order : orders) {
    auto & stack_src = stacks[order.stack_src_];
    auto & stack_dest = stacks[order.stack_dest_];

    // std::cout << "stack_src " << stack_to_string(stack_src) << "\n";

    std::stack<char> tmp_stack;

    for (auto i = order.nb_to_move_; i > 0; --i) {
      const auto crate = stack_src.front();
      stack_src.pop_front();
      tmp_stack.push(crate);
    }
    for (auto i = order.nb_to_move_; i > 0; --i) {
      const auto crate = tmp_stack.top();
      tmp_stack.pop();
      stack_dest.push_front(crate);
    }

    // std::cout << "stack_dest " << stack_to_string(stack_dest) << "\n";
  }
  return stacks;
}

static std::string get_first_crate_of_each_stack(Stacks const & rearrange_stacks)
{
  std::string answer{""};
  answer.reserve(rearrange_stacks.size());
  for (auto const & stack : rearrange_stacks) {
    answer.push_back(stack.front());
  }
  return answer;
}

int main(int argc, char * argv[])
{
  const auto filename = use_cli_or_default_value(argc, argv);
  const auto [stacks, orders] = read_input(filename);
  const auto rearrange_stacks = rearrange_crates(stacks, orders);
  const auto answer = get_first_crate_of_each_stack(rearrange_stacks);
  print_answer(answer);
  return 0;
}
