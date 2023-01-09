#pragma once

#include <compare>
#include <cstdint>
#include <queue>
#include <string>
#include <tuple>

enum class Dir
{
  UP,
  RIGHT,
  DOWN,
  LEFT,
};

struct Move
{
  Dir dir_;
  uint32_t nb_move_;
};

struct Pos
{
  int64_t x_;
  int64_t y_;

  auto operator<=>(Pos const &) const = default;
};

std::queue<Move> read_input(std::string const &);
std::tuple<std::vector<Pos>, std::vector<Pos>> move_head_and_tail(std::queue<Move>);
uint64_t count_position_visited_at_least_once(std::vector<Pos>);

