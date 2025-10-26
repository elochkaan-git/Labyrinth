#pragma once
#include "generator.hpp"
#include <cstddef>
#include <vector>

class Labyrinth
{
public:
  Labyrinth(size_t n);
  Labyrinth(size_t x, size_t y);
  ~Labyrinth();
  std::vector<std::vector<bool>>& getMap();
  void setGenerator(Generator* generator);
  void generateLabyrinth();
  void print();

private:
  std::vector<std::vector<bool>> map_;
  Generator* generator_;
};