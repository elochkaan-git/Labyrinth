#include "player.hpp"
#include "labyrinth.hpp"
#include <cstddef>

Player* Player::instance_ = nullptr;

Player::Player(Labyrinth* owner)
{
  owner_ = owner;
}

Player*
Player::getInstance()
{
  return instance_;
}

void
Player::init(Labyrinth* owner)
{
  instance_ = new Player(owner);
}

void
Player::move(size_t x, size_t y)
{
  auto& map = owner_->getReprOfMap();
  if(map[y][x] != '#')
    pos_ = {x, y};
}

std::pair<size_t, size_t>
Player::getPos() const
{
  return pos_;
}