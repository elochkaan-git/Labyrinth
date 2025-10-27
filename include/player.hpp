#pragma once
#include <cstddef>
#include <utility>

class Labyrinth;

class Player
{
public:
  Player(Labyrinth* owner);
  ~Player() = default;
  static Player* getInstance();
  static void init(Labyrinth* owner);
  void move(size_t x, size_t y);
  std::pair<size_t, size_t> getPos() const;

private:
  std::pair<size_t, size_t> pos_ = {1, 1};
  static Player* instance_;
  Labyrinth* owner_;
};