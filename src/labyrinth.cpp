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
  : map_(y, std::vector<Cell>(x, {0, 0, 0, false, false}))
  , reprOfmap_(y*2+1, std::vector<char>(x*2+1, '#'))
{
  size_t i = 0;
  for(size_t y = 0; y < map_.size(); ++y)
    for(size_t x = 0; x < map_[0].size(); ++x) {
      map_[y][x].id = i;
      ++i;
    }
  map_[0][0].isStart = true;
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
 * @brief Return representation of map_ for labyrinth with thick
 * walls. Exmaple:
 * 
 * |‾‾‾‾‾|
 * | |‾‾ |
 * |___|_|
 *
 * become
 *
 * #######
 * #     #
 * # ### #
 * # #   #
 * # # # #
 * #   # #
 * #######
 * 
 * @return std::vector<std::vector<char>>& representation of map_
 */
std::vector<std::vector<char>>&
Labyrinth::getReprOfMap()
{
  return reprOfmap_;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool
Labyrinth::checkWinner()
{
  return player_->getPos() == end_;
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

void
Labyrinth::setPlayer(Player* player)
{
  player_ = player;
}

void
Labyrinth::setEnd(std::pair<size_t, size_t> end)
{
  end_ = end;
}

/**
 * @brief Generate labyrinth using generate method of generator
 * 
 */
void
Labyrinth::generateLabyrinth()
{
  generator_->generate();
  update();
}

void
Labyrinth::update()
{
  size_t w = map_.size();
  size_t l = map_[0].size();

  for(size_t y=0; y<w; ++y) {
    for(size_t x=0; x<l; ++x) {
      if(map_[y][x].isStart)
        reprOfmap_[y*2+1][x*2+1] = 'X';
      else if(map_[y][x].isEnd) {
        reprOfmap_[y*2+1][x*2+1] = 'O';
        setEnd({x*2+1, y*2+1});
      }
      else 
        reprOfmap_[y*2+1][x*2+1] = ' ';

      if(map_[y][x].right)
        reprOfmap_[y*2+1][x*2+2] = ' ';

      if(map_[y][x].bottom)
        reprOfmap_[y*2+2][x*2+1] = ' ';
    }
  }
}

void
Labyrinth::print()
{
  for(size_t y=0; y<reprOfmap_.size(); ++y) {
    for(size_t x=0; x<reprOfmap_[0].size(); ++x) {
      if(player_->getPos().first == x && player_->getPos().second == y)
        std::cout << '@';
      else
        std::cout << reprOfmap_[y][x];
    }
    std::cout << '\n';
  }
}
