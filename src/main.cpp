#include "labyrinth.hpp"
#include "generator.hpp"
#include <iostream>

int
main()
{ 
  Labyrinth::init(30, 5);
  Labyrinth& lab = Labyrinth::getInstance();
  lab.setGenerator(new WilsonGenerator(&lab));
  lab.generateLabyrinth();
  Player::init(&lab);
  lab.setPlayer(Player::getInstance());
  lab.print();

  Player* p = Player::getInstance();
  while (true) {
    auto [x, y] = p->getPos();
    int choice;
    std::cin >> choice;

    if(choice == 6)
      p->move(x+1, y);
    else if(choice == 8)
      p->move(x, y-1);
    else if(choice == 4)
      p->move(x-1, y);
    else if(choice == 2)
      p->move(x, y+1);
    lab.print();
    if(lab.checkWinner())
      break;
  }

  return 0;
}