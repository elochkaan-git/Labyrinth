#include "labyrinth.hpp"
#include "generator.hpp"
#include <cstddef>
#include <iostream>
#include <vector>

Labyrinth::Labyrinth(size_t n)
  : map_(n * 2 - 1, std::vector<bool>(n * 2 - 1, 0))
{
}

Labyrinth::Labyrinth(size_t x, size_t y)
  : map_(y * 2 - 1, std::vector<bool>(x * 2 - 1, 0))
{
}

Labyrinth::~Labyrinth()
{
  delete generator_;
}

std::vector<std::vector<bool>>&
Labyrinth::getMap()
{
  return map_;
}

void
Labyrinth::setGenerator(Generator* generator)
{
  generator_ = generator;
}

void
Labyrinth::generateLabyrinth()
{
  generator_->generate();
}

void
Labyrinth::print()
{
  for(size_t n = 0; n < map_[0].size()+2; ++n)
    std::cout << '#';
  std::cout << std::endl;

  for(auto& row : map_) {
    std::cout << '#';
    for(bool cell : row) {
      if(cell)
        std::cout << ' ';
      else
       std::cout << '#';
    }
    std::cout << '#';
    std::cout << std::endl;
  }

  for(size_t n = 0; n < map_[0].size()+2; ++n)
    std::cout << '#';
  std::cout << std::endl;
}