#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Move
{
  // Either 'A', 'B', 'C'
  char their_;
  // Either 'X', 'Y', 'Z'
  char mine_;

  // To go to 'X' -> 'A' : 'X' - 'A' - 23
  // To go to 'Y' -> 'B' : 'Y' - 'A' - 23
  // To go to 'Z' -> 'C' : 'Z' - 'A' - 23
};

static std::vector<Move> read_full_file(std::string const & filename)
{
  std::ifstream input(filename);
  if (!input) {
    throw std::runtime_error("could not open file [" + filename + "]");
  }
  std::vector<Move> moves;
  std::string line;
  while (std::getline(input, line) && !line.empty()) {
    // line looks like
    // "${their_move} ${my_move}"
    moves.push_back({ .their_ = line[0], .mine_ = line[2] });
    // std::cout << "line[0] = " << line[0] << " | line[2] = " << line[2] << "\n";
  }
  return moves;
}

struct Game
{
  Move move_;
  uint64_t score_;
};

static int64_t play(Move const & move)
{
  // A == Rock    = 0 + 1 => 1
  // B == Paper   = 1 + 1 => 2
  // C == Scisors = 2 + 1 => 3
  constexpr auto MAX_ALPHABET_DISTANCE{23};
  int64_t outcome = (move.mine_ - MAX_ALPHABET_DISTANCE) - move.their_;
  // Go to the idx in the alphabet
  // outcome = move_to_point(outcome);
  // | outcome | == 2 when Rock vs Paper
  if (std::abs(outcome) == 2) {
    outcome *= -1;
  }
  return outcome;
}

static uint64_t game_result_to_points(int64_t game_result)
{
  // constexpr enum class GAME_RESULT
  // {
  //   LOST  = 0,
  //   DRAW  = 3,
  //   WON   = 6
  // };

  if (game_result < 0) {
    // it's a lost
    return 0;
  } else if (game_result == 0) {
    // it's a draw
    return 3;
  } else {
    // game_result > 0
    // it's a win
    return 6;
  }
}

static uint64_t calculate_game_score(int64_t game_result,
  char my_move,
  std::vector<char> const & move_points)
{
  const auto score_point = game_result_to_points(game_result);
  uint64_t move_point = *(std::find(std::cbegin(move_points), std::cend(move_points), (my_move - 23)));
  move_point -= 'A';
  move_point += 1; // off by one !
  // std::cout << my_move << " <=> " << std::to_string(move_point) << " + " << std::to_string(score_point) << " = " << std::to_string(move_point + score_point) << "\n";
  return move_point + score_point;
}
static std::vector<Game> play_the_games(std::vector<Move> const & moves,
    std::vector<char> const & move_points)
{
  std::vector<Game> games(moves.size());
  // auto i{0u};
  for (auto const & move : moves) {
    const auto game_result = play(move);
    // std::cout << "#" << i++ << ": mine = [" << move.mine_ << "] | their = [" << move.their_ << "] | outcome = " << game_result << "\n";
    assert(game_result >= -2 && game_result <= 2);
    const auto game_score = calculate_game_score(game_result, move.mine_, move_points);
    games.push_back({ move, game_score });
  }
  return games;
}

static void part1(std::vector<Game> const & games)
{
  const auto total_score = std::accumulate(std::cbegin(games), std::cend(games), 0,
    [](uint64_t total, Game const & game) { return total + game.score_; }
  );
  std::cout << "=== part 1 ===" << "\n";
  std::cout << total_score << "\n";
}
int main()
{
  const auto moves = read_full_file("input");
  std::vector<char> move_points = { 'A', 'B', 'C' };
  const auto games = play_the_games(moves, move_points);
  part1(games);
  return 0;
}
