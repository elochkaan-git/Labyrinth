#include "generator.hpp"
#include "labyrinth.hpp"
#include <bits/stdc++.h>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <math.h>

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
}

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
}