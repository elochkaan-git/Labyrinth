#pragma once

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