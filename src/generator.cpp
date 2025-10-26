#include "generator.hpp"
#include "labyrinth.hpp"
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <bits/stdc++.h>

Generator::Generator(Labyrinth* owner)
  : owner_(owner)
{
}

void
BinaryTreeGenerator::generate()
{
  srand(time(0));
  std::vector<std::vector<Cell>>& map = owner_->getMap();
  size_t length = map[0].size(), width = map.size();
  for(size_t y = 1; y < width; ++y)
    for(size_t x = 0; x < length; ++x) {
      if((double)rand() / RAND_MAX <= 0.5)
        map[y-1][x].bottom = 1;
      else {
        if(x == length - 1)
          map[y-1][x].bottom = 1;
        else
          map[y][x].right = 1;
      }
    }
  for(size_t n = 0; n < length - 1; ++n)
    map[0][n].right = 1;

  return;
}

void
SidewinderGenerator::generate()
{
  // srand(time(0));
  // std::vector<std::vector<bool>>& map = owner_->getMap();
  // size_t length = map[0].size(), width = map.size();
  // for(size_t y = 2; y < width; y += 2) {
  //   for(size_t x = 0; x < length; x += 2) {
  //     size_t slice = (rand() % (length - x)) + 1;
  //     slice > 1 ? slice /= 2 : slice = 1;
  //     size_t curCell = rand() % slice;
  //     for(size_t k = x; k < x + slice; ++k) {
  //       if(k == curCell) {
  //         map[y-1][k] = 1;
  //         map[y][k] = 1;
  //       }
  //       else {
  //         map[y][k] = 1;
  //       }
  //     }
  //     x += slice - 1;
  //   }
  // }

  // for(size_t n = 0; n < map[0].size(); ++n)
  //   map[0][n] = 1;
}