#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

class Labyrinth;

class Player
{
public:
  Player(Labyrinth* owner, float radius);
  ~Player() = default;
  static Player* getInstance();
  static void init(Labyrinth* owner, float radius);
  const sf::CircleShape& getShape() const; 
  sf::Vector2u getPos() const;
  void move(char direction);

private:
  sf::Vector2u pos_ = {1, 1};
  static Player* instance_;
  Labyrinth* owner_;
  sf::CircleShape shape_;
};