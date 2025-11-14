#include "player.hpp"
#include "labyrinth.hpp"
#include "spdlog/spdlog.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

Player* Player::instance_ = nullptr;

/**
 * @brief Construct a new Player:: Player object
 * 
 * @param owner instance of Labyrinth
 * @param radius radius of player circle
 */
Player::Player(Labyrinth* owner, float radius)
  : shape_(radius)
{
  owner_ = owner;
  shape_.setPosition(
    { radius / 0.49f * getPos().x, radius / 0.49f * getPos().y });
}

/**
 * @brief Return instance of player
 * 
 * @return Player* instance
 */
Player*
Player::getInstance()
{
  if (!instance_) {
    spdlog::critical("Player not initialized! Shutdown program");
    throw std::runtime_error("Singleton not initialized!");
  }
  return instance_;
}

/**
 * @brief Construct new singletone instance
 * 
 * @param owner instance of Labyrinth
 * @param radius radius of player circle
 */
void
Player::init(Labyrinth* owner, float radius)
{
  if(owner)
    instance_ = new Player(owner, radius);
  else {
    spdlog::critical("'owner' must be not nullptr! Shutdown program");
    throw std::runtime_error("'owner' is nullptr");
  }
}

/**
 * @brief Change player and circle positions
 * 
 * @param direction 
 */
void
Player::move(char direction)
{ 
  if(0 <= direction && direction <= 3) {
    auto& map = owner_->getReprOfMap();
    sf::Vector2u pos = getPos();
    if (direction == Direction::Right)
      pos.x++;
    if (direction == Direction::Left)
      pos.x--;
    if (direction == Direction::Down)
      pos.y++;
    if (direction == Direction::Up)
      pos.y--;

    if (map[pos.y][pos.x] != '#') {
      spdlog::info("Moving player to ({}, {})", pos.x, pos.y);
      pos_ = pos;
      shape_.setPosition(
        { shape_.getRadius() / 0.49f * pos.x, shape_.getRadius() / 0.49f * pos.y });
    }
  } else {
    spdlog::warn("Wrong direction! Must be between 0 and 4 inclusive, got {} instead", (int)direction);
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