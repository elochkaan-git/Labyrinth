#include "labyrinth.hpp"
#include "generator.hpp"
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>


Labyrinth* Labyrinth::instance_ = nullptr;

/**
 * @brief Rectangular Labyrinth construct
 * 
 * @param x length
 * @param y width
 */
Labyrinth::Labyrinth(size_t x, size_t y)
  : map_(y, std::vector<Cell>(x, {0, false, false}))
{
  size_t i = 0;
  for(size_t y = 0; y < map_.size(); ++y)
    for(size_t x = 0; x < map_[0].size(); ++x) {
      map_[y][x].id = i;
      ++i;
    }
}

Labyrinth::~Labyrinth()
{
  delete generator_;
}

/**
 * @brief Return instance of labyrinth
 * 
 * @return Labyrinth& 
 */
Labyrinth&
Labyrinth::getInstance()
{
  if(!instance_)
    throw std::runtime_error("Singleton not initialized!");
  return *instance_;
}

/**
 * @brief Initialize labyrinth of a given size
 * 
 * @param x length
 * @param y width
 */
void
Labyrinth::init(size_t x, size_t y)
{
  instance_ = new Labyrinth(x, y);
}

/**
 * @brief Return reference to map_ member
 * 
 * @return std::vector<std::vector<bool>>& 
 */
std::vector<std::vector<Cell>>&
Labyrinth::getMap()
{
  return map_;
}

/**
 * @brief Set generator of labyrinth
 * 
 * @param generator derived class of abstract class Generator
 */
void
Labyrinth::setGenerator(Generator* generator)
{
  generator_ = generator;
}

/**
 * @brief Generate labyrinth using generate method of generator
 * 
 */
void
Labyrinth::generateLabyrinth()
{
  generator_->generate();
}

void Labyrinth::print() {
  size_t w = map_.size();
  size_t l = map_[0].size();

  std::vector<std::vector<bool>> map(w*2+1, std::vector<bool>(l*2+1, false));

  for(size_t y=0; y<w; ++y) {
    for(size_t x=0; x<l; ++x) {
      map[y*2+1][x*2+1] = true;

      if(map_[y][x].right)
        map[y*2+1][x*2+2] = true;

      if(map_[y][x].bottom)
        map[y*2+2][x*2+1] = true;
    }
  }

  for(size_t y=0; y<map.size(); ++y) {
    for(size_t x=0; x<map[0].size(); ++x)
      std::cout << (map[y][x] ? ' ' : '#');
    std::cout << '\n';
  }
}
