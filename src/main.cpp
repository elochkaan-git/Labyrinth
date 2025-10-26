#include "labyrinth.hpp"
#include "generator.hpp"

int
main()
{
  Labyrinth lab(10);
  lab.setGenerator(new BinaryTreeGenerator(&lab));
  lab.generateLabyrinth();
  lab.print();

  return 0;
}