#include "util.h"

#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>

static Pos move_head(Pos pos, Dir dir);
static Pos move_tail(Pos const & head_pos, Pos tail_pos, Dir dir);

std::queue<Move> read_input(std::string const & filename)
{
  std::ifstream stream(filename);
  if (!stream) {
    throw std::runtime_error("could not open file [" + filename + "]");
  }

  std::queue<Move> movements;

  std::string line;
  while (std::getline(stream, line) && !line.empty()) {

    Dir dir{Dir::UP};
    switch (line[0]) {
      case 'U':
        dir = Dir::UP;
        break;
      case 'R':
        dir = Dir::RIGHT;
        break;
      case 'D':
        dir = Dir::DOWN;
        break;
      case 'L':
        dir = Dir::LEFT;
        break;
    }

    uint32_t nb_move;
    auto [ptr, ec] { std::from_chars(line.data() + 2, line.data() + line.size(), nb_move) };
 
        if (ec == std::errc::invalid_argument)
        {
            throw std::runtime_error("That isn't a number: " + line);
        }
        else if (ec == std::errc::result_out_of_range)
        {
            throw std::runtime_error("This number is larger than an uint32_t: " + line);
        } else if (ec != std::errc()) {
            throw std::runtime_error("Unkown error when parsing: " + line);
        }

    movements.push({ dir, nb_move });
  }

  return movements;
}

std::tuple<std::vector<Pos>, std::vector<Pos>> move_head_and_tail(std::queue<Move> movements)
{
  std::vector<Pos> head;
  std::vector<Pos> tail;

  Pos head_pos{0, 0};
  Pos tail_pos{0, 0};

  head.push_back(head_pos);
  tail.push_back(tail_pos);

  while (!movements.empty()) {
    auto move = movements.front();

    while (move.nb_move_-- > 0) {

      head_pos = move_head(head_pos, move.dir_);
      head.push_back(head_pos);

      tail_pos = move_tail(head_pos, tail_pos, move.dir_);
      tail.push_back(tail_pos);

    }

    movements.pop();
  }

  return { head, tail };
}

uint64_t count_position_visited_at_least_once(std::vector<Pos> positions)
{
  std::ranges::sort(positions);
  const auto ret = std::unique(positions.begin(), positions.end());
  positions.erase(ret, positions.end());
  return positions.size();
}


static Pos move_head(Pos pos, Dir dir)
{
  switch (dir) {

    case Dir::UP:
      pos.y_++;
      break;

    case Dir::RIGHT:
      pos.x_++;
      break;

    case Dir::DOWN:
      pos.y_--;
      break;

    case Dir::LEFT:
      pos.x_--;
      break;

    default:
      throw std::runtime_error("movement not handled");
  }

  return pos;
}

static Pos move_tail(Pos const & head_pos, Pos tail_pos, Dir dir)
{
  const auto dist_x = head_pos.x_ - tail_pos.x_;
  const auto dist_y = head_pos.y_ - tail_pos.y_;

  if ((std::abs(dist_x) > 1 && std::abs(dist_y) == 1) ||
      (std::abs(dist_y) > 1 && std::abs(dist_x) == 1)) {
    tail_pos.x_ += dist_x;
    if (std::abs(dist_x) > 1) {
        if (dist_x < 0) {
        tail_pos.x_ += 1;
      } else {
        tail_pos.x_ -= 1;
      }
    }

    tail_pos.y_ += dist_y;
    if (std::abs(dist_y) > 1) {
        if (dist_y < 0) {
        tail_pos.y_ += 1;
      } else {
        tail_pos.y_ -= 1;
      }
    }
  }
  else if (std::abs(dist_x) > 1 || std::abs(dist_y) > 1) {
    tail_pos = move_head(tail_pos, dir);
  }

  return tail_pos;
}

