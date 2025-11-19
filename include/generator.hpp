#pragma once
#include <random>
#include <vector>

class Labyrinth;

struct Generator
{
  Generator(Labyrinth* owner);
  virtual ~Generator() = default;
  virtual void generate() = 0;
  Labyrinth* owner_;
};

struct BinaryTreeGenerator : public Generator
{
  BinaryTreeGenerator(Labyrinth* owner)
    : Generator(owner)
  {
  }
  void generate() override;
};

struct SidewinderGenerator : public Generator
{
  SidewinderGenerator(Labyrinth* owner)
    : Generator(owner)
  {
  }
  void generate() override;
};

struct WilsonGenerator : public Generator
{
  WilsonGenerator(Labyrinth* owner)
    : Generator(owner)
  {
  }
  void generate() override;

private:
  std::mt19937 rng_{ std::random_device{}() };
  size_t width_;
  size_t height_;

  size_t getCellIndex(size_t x, size_t y) const;
  void indexToCell(size_t index, size_t& x, size_t& y) const;
  std::vector<std::pair<size_t, size_t>> getNeighbors(size_t x, size_t y) const;
};