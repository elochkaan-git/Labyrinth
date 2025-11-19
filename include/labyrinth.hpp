#pragma once
#include "generator.hpp"
#include "player.hpp"
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <vector>

struct Cell
{
  size_t id = 0;
  bool bottom = false;
  bool right = false;
  bool isStart = false;
  bool isEnd = false;
};

class Labyrinth
{
public:
  Labyrinth(unsigned int x, unsigned int y);
  ~Labyrinth();
  static Labyrinth& getInstance();
  static void init(unsigned int x, unsigned int y);
  std::vector<std::vector<Cell>>& getMap();
  std::vector<std::vector<char>>& getReprOfMap();
  bool checkWinner();
  void setGenerator(Generator* generator);
  void setPlayer(Player* player);
  void generateLabyrinth();
  void print();

private:
  void update();
  void setEnd(sf::Vector2u end);
  std::vector<std::vector<Cell>> map_;
  std::vector<std::vector<char>> reprOfmap_;
  Generator* generator_ = nullptr;
  Player* player_ = nullptr;
  static Labyrinth* instance_;
  sf::Vector2u end_;
};
