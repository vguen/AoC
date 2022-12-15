#include <algorithm>
#include <array>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

struct Oracle
{
  char opponent_move_;
  char game_outcome_;
};

struct GamePoints
{
  char sign_;
  uint64_t points_;
};

struct Node
{
  Node(uint64_t point, char value) :
    point_(point),
    value_(value)
  { }

  Node * win_against_ = nullptr;
  Node * lose_against_ = nullptr;
  uint64_t point_;
  char value_;
};

enum class GameOutcome
{
  Draw,
  Lost,
  Won
};

static GameOutcome sign_to_game_outcome(char sign)
{
  switch (sign) {
    
    case 'X':
      return GameOutcome::Lost;
      break;

    case 'Y':
      return GameOutcome::Draw;
      break;

    case 'Z':
      return GameOutcome::Won;
      break;

    default:
      throw std::runtime_error("could not find outcome for sign " + std::to_string(sign));

  }
}

static std::vector<Oracle> read_full_file(std::string const & filename)
{
  std::ifstream input(filename);
  if (!input) {
    throw std::runtime_error("could not open file [" + filename + "]");
  }
  std::vector<Oracle> oracles;
  std::string line{""};
  while (std::getline(input, line) && !line.empty()) {
    // line looks like
    // "${opponent_oracle} ${game_outcome}"
    oracles.push_back({ .opponent_move_ = line[0], .game_outcome_ = line[2] });
  }
  return oracles;
}

static std::array<Node, 3> create_graph()
{
  std::array<Node, 3> graph = {
    Node(1u, 'A'),
    Node(2u, 'B'),
    Node(3u, 'C')
  };

  graph[0].win_against_ = &graph[2];
  graph[0].lose_against_ = &graph[1];

  graph[1].win_against_ = &graph[0];
  graph[1].lose_against_ = &graph[2];

  graph[2].win_against_ = &graph[1];
  graph[2].lose_against_ = &graph[0];

  return graph;
}

static Node const * find_my_move(char opponent_move,
    GameOutcome oracle,
    std::array<Node, 3> const & graph)
{
  auto opponent_node = std::find_if(std::begin(graph), std::end(graph), [opponent_move](Node const & node) {
    return node.value_ == opponent_move;
  });

  if (oracle == GameOutcome::Draw) {

    return opponent_node;

  } else if (oracle == GameOutcome::Lost) {

    // If *I* loose, it means *my oppenent* won
    return opponent_node->win_against_;

  } else if (oracle == GameOutcome::Won) {

    // If *I* win, it means *my oppenent* loose
    return opponent_node->lose_against_;

  }

  return nullptr;
}

static uint64_t calculate_game_points(char game_outcome,
  std::array<GamePoints, 3> possibles_outcome)
{
  return (std::find_if(
    std::begin(possibles_outcome), std::end(possibles_outcome),
    [game_outcome](GamePoints const & game_points) { return game_points.sign_ == game_outcome; })
  )->points_;
}

static uint64_t calculate_points_from_oracles(std::vector<Oracle> const & oracles,
    std::array<Node, 3> const & graph)
{
  std::array<GamePoints, 3> possibles_outcome = {{
    { .sign_ = 'X', .points_ = 0 },
    { .sign_ = 'Y', .points_ = 3 },
    { .sign_ = 'Z', .points_ = 6 }
  }};

  uint64_t total_points{0};
  for (auto const & oracle : oracles) {
    Node const * my_move = find_my_move(oracle.opponent_move_, sign_to_game_outcome(oracle.game_outcome_), graph);
    const auto game_points = calculate_game_points(oracle.game_outcome_, possibles_outcome);
    total_points += my_move->point_ + game_points;
  }
  return total_points;
}

static void print_response(uint64_t total_points)
{
  std::cout << total_points << "\n";
}

int main()
{
  const auto oracles = read_full_file("input");
  const auto graph = create_graph();
  const auto total_points = calculate_points_from_oracles(oracles, graph);
  print_response(total_points);
  return 0;
}

