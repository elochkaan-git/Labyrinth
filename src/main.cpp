#include "labyrinth.hpp"
#include "generator.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int
main()
{ 
  // making grid with const size, where cell_size means size of square in pixels
  const int length_field = 30, width_field = 10, cell_size = 25;
  Labyrinth::init(length_field, width_field);
  Labyrinth& lab = Labyrinth::getInstance();
  lab.setGenerator(new WilsonGenerator(&lab));
  lab.generateLabyrinth();
  Player::init(&lab);
  lab.setPlayer(Player::getInstance());
  
  const auto& map_represent = lab.getReprOfMap();
  size_t rep_length = map_represent[0].size(), rep_width = map_represent.size();
  sf::RenderWindow window(sf::VideoMode({rep_length*cell_size, rep_width*cell_size}), "Labyrinth", sf::Style::Close);

  // creating map
  
  std::vector<std::vector<sf::RectangleShape>> cells(map_represent.size());
  
  for(size_t y = 0; y < rep_width; ++y){
    cells[y].resize(rep_length);
    for(size_t x = 0; x < rep_length; ++x){
      cells[y][x] = sf::RectangleShape({cell_size, cell_size});
      if (map_represent[y][x] == '#')
        cells[y][x].setFillColor(sf::Color(128, 128, 128));
      else if(map_represent[y][x] == 'X')
        cells[y][x].setFillColor(sf::Color::Magenta);
      else if(map_represent[y][x] == 'O')
        cells[y][x].setFillColor(sf::Color::Green);
      else
        cells[y][x].setFillColor(sf::Color::Transparent);
      cells[y][x].setPosition({static_cast<float>(x*cell_size),static_cast<float>(y*cell_size)});
    }
  }

  Player* p = Player::getInstance();
  
  while (window.isOpen())
  {
    if(lab.checkWinner())
      window.close();
    auto [x, y] = p->getPos();
    // check all the window's events that were triggered since the last iteration of the loop
    while (const std::optional event = window.pollEvent())
    {
        // "close requested" event: we close the window
        if (event->is<sf::Event::Closed>())
            window.close();
        else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
          if (keyPressed->scancode == sf::Keyboard::Scancode::W)
            p->move(x, y-1);
          if (keyPressed->scancode == sf::Keyboard::Scancode::A)
            p->move(x-1, y);
          if (keyPressed->scancode == sf::Keyboard::Scancode::S)
            p->move(x, y+1);
          if (keyPressed->scancode == sf::Keyboard::Scancode::D)
            p->move(x+1, y);
        }
    }
    // clear the window with black color
    window.clear(sf::Color::Black);

    //Draw map
    for(const auto& row: cells){
      for(const auto& cell: row){
        window.draw(cell);
      }
    }
    
    //Create and draw player
    sf::CircleShape player(cell_size*0.49f);

    
    player.setPosition({x*cell_size, y*cell_size});
    player.setFillColor(sf::Color::White);
    window.draw(player);
    
    // end the current frame
    window.display();
  }
  std::cout << "You win!\n";
  return 0;
}