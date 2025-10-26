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
  std::vector<std::vector<bool>>& map = owner_->getMap();
  size_t height = map.size(), weight = map[0].size();
  for(size_t y = 2; y < height; y += 2)
    for(size_t x = 0; x < weight; x += 2) {
      map[y][x] = 1;
      if((double)rand() / RAND_MAX <= 0.5)
        map[y-1][x] = 1;
      else {
        if(x == weight - 1)
          map[y-1][x] = 1;
        else
          map[y][x+1] = 1;
      }
    }
  for(size_t n = 0; n < map[0].size(); ++n)
    map[0][n] = 1;

  return;
}