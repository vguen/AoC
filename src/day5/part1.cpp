#include <deque> // TODO: usefull other than for testing ?
#include <iostream>
#include <stack>
#include <string>
#include <string_view>
#include <vector>
#include <tuple>

struct Order
{
  uint64_t nb_to_move_;
  uint64_t stack_src_;
  uint64_t stack_dest_;
};

using Stacks = std::vector<std::stack<char>>;
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

static std::tuple<Stacks, Orders> read_input()
{
  Stacks stacks;

  stacks.emplace_back(std::deque<char>{ 'Z', 'N' });
  stacks.emplace_back(std::deque<char>{ 'M', 'C', 'D' });
  stacks.emplace_back(std::deque<char>{ 'P' });

  Orders orders;

  orders.push_back({ .nb_to_move_ = 1, .stack_src_ = 2 - 1, .stack_dest_ = 1 - 1});
  orders.push_back({ .nb_to_move_ = 3, .stack_src_ = 1 - 1, .stack_dest_ = 3 - 1});
  orders.push_back({ .nb_to_move_ = 2, .stack_src_ = 2 - 1, .stack_dest_ = 1 - 1});
  orders.push_back({ .nb_to_move_ = 1, .stack_src_ = 1 - 1, .stack_dest_ = 2 - 1});

  return { stacks, orders };
}

static std::string stack_to_string(std::stack<char> stack)
{
  std::string s{"{ "};
  s.reserve(stack.size());
  for (auto i = stack.size(); i > 0; --i) {
    s += stack.top() + std::string(", ");
    stack.pop();
  }
  s += "}";
  return s;
}

static Stacks rearrange_crates(Stacks stacks, Orders const & orders)
{
  for (auto const & order : orders) {
    auto & stack_src = stacks[order.stack_src_];
    auto & stack_dest = stacks[order.stack_dest_];

    // std::cout << "stack_src " << stack_to_string(stack_src) << "\n";

    for (auto i = order.nb_to_move_; i > 0; --i) {
      const auto crate = stack_src.top();
      stack_src.pop();
      stack_dest.push(crate);
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
    answer.push_back(stack.top());
  }
  return answer;
}

int main(int argc, char * argv[])
{
  const auto filename = use_cli_or_default_value(argc, argv);
  const auto [stacks, orders] = read_input();
  const auto rearrange_stacks = rearrange_crates(stacks, orders);
  const auto answer = get_first_crate_of_each_stack(rearrange_stacks);
  print_answer(answer);
  return 0;
}
