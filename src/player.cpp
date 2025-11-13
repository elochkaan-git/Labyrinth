#include "player.hpp"
#include "labyrinth.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

Player* Player::instance_ = nullptr;

Player::Player(Labyrinth* owner, float radius)
  : shape_(radius)
{
  owner_ = owner;
  shape_.setPosition(
    { radius / 0.49f * getPos().x, radius / 0.49f * getPos().y });
}

Player*
Player::getInstance()
{
  return instance_;
}

void
Player::init(Labyrinth* owner, float radius)
{
  instance_ = new Player(owner, radius);
}

void
Player::move(char direction)
{
  auto& map = owner_->getReprOfMap();
  sf::Vector2u pos = getPos();
  if (direction == 'r')
    pos.x++;
  if (direction == 'l')
    pos.x--;
  if (direction == 'd')
    pos.y++;
  if (direction == 'u')
    pos.y--;

  if (map[pos.y][pos.x] != '#') {
    pos_ = pos;
    shape_.setPosition(
      { shape_.getRadius() / 0.49f * pos.x, shape_.getRadius() / 0.49f * pos.y });
  }
}

sf::Vector2u
Player::getPos() const
{
  return pos_;
}

const sf::CircleShape&
Player::getShape() const
{
  return shape_;
}