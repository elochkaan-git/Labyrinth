#include "generator.hpp"
#include "labyrinth.hpp"
#include "spdlog/spdlog.h"
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <random>
#include <vector>

/**
 * @brief Construct a new Generator:: Generator object
 * 
 * @param owner instance of Labyrinth
 */
Generator::Generator(Labyrinth* owner)
{
  if(owner)
    owner_ = owner;
  else {
    spdlog::critical("'owner' must be not nullptr! Shutdown program");
    throw std::runtime_error("'owner' is nullptr");
  }
}

/**
 * @brief Fill map of owner using Binary Tree algorythm
 * 
 */
void
BinaryTreeGenerator::generate()
{
  srand(time(0));
  std::vector<std::vector<Cell>>& map = owner_->getMap();
  size_t length = map[0].size(), width = map.size();

  for (size_t y = 1; y < width; ++y)
    for (size_t x = 0; x < length; ++x) {
      if ((double)rand() / RAND_MAX <= 0.5)
        map[y - 1][x].bottom = 1;
      else {
        if (x == length - 1)
          map[y - 1][x].bottom = 1;
        else
          map[y][x].right = 1;
      }
    }

  for (size_t n = 0; n < length - 1; ++n)
    map[0][n].right = 1;
  map[map.size() - 1][0].isEnd = 1;
}

/**
 * @brief Fill map of owner using Sidewinder algorythm
 * 
 */
void
SidewinderGenerator::generate()
{
  srand(time(0));
  std::vector<std::vector<Cell>>& map = owner_->getMap();
  size_t length = map[0].size(), width = map.size();

  for (size_t y = 1; y < width; ++y) {
    for (size_t x = 0; x < length; ++x) {
      size_t slice = (rand() % (size_t)ceil(sqrt(map[0].size()))) + 1;
      x + slice > map[0].size() ? slice = map[0].size() - x : 0;
      size_t curCell = x + rand() % slice;
      for (size_t k = x; k < x + slice; ++k) {
        if (k == curCell) {
          map[y - 1][k].bottom = 1;
          if (k != x + slice - 1)
            map[y][k].right = 1;
        } else if (k != x + slice - 1)
          map[y][k].right = 1;
      }
      x += slice - 1;
    }
  }

  for (size_t n = 0; n < length - 1; ++n)
    map[0][n].right = 1;
  map[map.size() - 1][0].isEnd = 1;
}

/**
 * @brief Fill map of owner Wilson algorythm
 * 
 */
void
WilsonGenerator::generate()
{
  auto& map = owner_->getMap();
  height_ = map.size();
  width_ = map[0].size();

  std::vector<bool> inMaze(width_ * height_, false);
  std::uniform_int_distribution<size_t> dist(0, width_ * height_ - 1);

  size_t startIdx = dist(rng_);
  inMaze[startIdx] = true;

  for (size_t idx = 0; idx < width_ * height_; ++idx) {
    if (inMaze[idx])
      continue;

    std::vector<size_t> path;
    std::vector<bool> inPath(width_ * height_, false);

    size_t current = idx;
    path.push_back(current);
    inPath[current] = true;

    while (!inMaze[current]) {
      auto [x, y] = std::make_pair(current % width_, current / width_);
      auto neighbors = getNeighbors(x, y);
      std::uniform_int_distribution<size_t> ndist(0, neighbors.size() - 1);
      auto [nx, ny] = neighbors[ndist(rng_)];

      size_t next = getCellIndex(nx, ny);

      if (inPath[next]) {
        while (path.back() != next) {
          inPath[path.back()] = false;
          path.pop_back();
        }
      } else {
        path.push_back(next);
        inPath[next] = true;
      }
      current = next;
    }

    for (size_t i = 0; i < path.size() - 1; ++i) {
      size_t curIdx = path[i];
      size_t nextIdx = path[i + 1];

      auto [cx, cy] = std::make_pair(curIdx % width_, curIdx / width_);
      auto [nx, ny] = std::make_pair(nextIdx % width_, nextIdx / width_);

      if (nx == cx + 1)
        map[cy][cx].right = true;
      else if (nx == cx - 1)
        map[ny][nx].right = true;
      else if (ny == cy + 1)
        map[cy][cx].bottom = true;
      else if (ny == cy - 1)
        map[ny][nx].bottom = true;

      inMaze[curIdx] = true;
    }
    inMaze[path.back()] = true;
  }

  map[height_ - 1][width_ - 1].isEnd = 1;
}

/**
 * @brief Auxiliary method for Wilson algorythm
 * 
 * @param x x coordinate of cell
 * @param y y coordinate of cell
 * @return size_t cell index
 */
size_t
WilsonGenerator::getCellIndex(size_t x, size_t y) const
{
  return y * width_ + x;
}

/**
 * @brief Auxiliary method for Wilson algorytm
 * 
 * @param index index of cell
 * @param x x coordinate of cell
 * @param y y coordinate of cell
 */
void
WilsonGenerator::indexToCell(size_t index, size_t& x, size_t& y) const
{
  x = index % width_;
  y = index / width_;
}

/**
 * @brief Auxiliary method for Wilson algorytm
 * 
 * @param x x coordinate of cell
 * @param y y coordinate of cell
 * @return std::vector<std::pair<size_t, size_t>> neighbours of cell
 */
std::vector<std::pair<size_t, size_t>>
WilsonGenerator::getNeighbors(size_t x, size_t y) const
{
  std::vector<std::pair<size_t, size_t>> neighbors;
  if (x > 0)
    neighbors.emplace_back(x - 1, y); // left
  if (x + 1 < width_)
    neighbors.emplace_back(x + 1, y); // right
  if (y > 0)
    neighbors.emplace_back(x, y - 1); // up
  if (y + 1 < height_)
    neighbors.emplace_back(x, y + 1); // down
  return neighbors;
}