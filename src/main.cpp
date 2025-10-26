#include "labyrinth.hpp"
#include "generator.hpp"

int
main()
{
  Labyrinth::init(5, 5);
  Labyrinth& lab = Labyrinth::getInstance();
  lab.setGenerator(new BinaryTreeGenerator(&lab));
  lab.generateLabyrinth();
  lab.print();

  return 0;
}