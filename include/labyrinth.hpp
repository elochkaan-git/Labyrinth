#pragma once
#include "generator.hpp"
#include <cstddef>
#include <vector>

struct Cell
{
  size_t id = 0;
  size_t x = 0;
  size_t y = 0;
  bool bottom = false;
  bool right = false;
};

class Labyrinth
{
public:
  Labyrinth(size_t x, size_t y);
  ~Labyrinth();
  static Labyrinth& getInstance();
  static void init(size_t x, size_t y);
  std::vector<std::vector<Cell>>& getMap();
  void setGenerator(Generator* generator);
  void generateLabyrinth();
  void print();

private:
  std::vector<std::vector<Cell>> map_;
  Generator* generator_ = nullptr;
  static Labyrinth* instance_;
};
