#include "generator.hpp"
#include "labyrinth.hpp"
#include "player.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

int
main()
{
  // Setup logger
  spdlog::set_level(spdlog::level::debug);
  spdlog::set_pattern("[%H:%M:%S] [%n] [%^---%L---%$] [thread %t] %v");

  // making grid with const size, where cell_size means size of square in pixels
  const int length_field = 10, width_field = 10, cell_size = 25;
  Labyrinth::init(length_field, width_field);
  Labyrinth& lab = Labyrinth::getInstance();
  lab.setGenerator(new WilsonGenerator(&lab));
  lab.generateLabyrinth();
  Player::init(&lab, cell_size * 0.49f);
  lab.setPlayer(Player::getInstance());

  const auto& map_represent = lab.getReprOfMap();
  unsigned int rep_length = map_represent[0].size(), rep_width = map_represent.size();
  sf::RenderWindow window(
    sf::VideoMode({ rep_length * cell_size, rep_width * cell_size }),
    "Labyrinth",
    sf::Style::Close);

  // creating map
  std::vector<std::vector<sf::RectangleShape>> cells(map_represent.size());

  for (size_t y = 0; y < rep_width; ++y) {
    cells[y].resize(rep_length);
    for (size_t x = 0; x < rep_length; ++x) {
      cells[y][x] = sf::RectangleShape({ cell_size, cell_size });
      if (map_represent[y][x] == '#')
        cells[y][x].setFillColor(sf::Color(128, 128, 128));
      else if (map_represent[y][x] == 'X')
        cells[y][x].setFillColor(sf::Color::Magenta);
      else if (map_represent[y][x] == 'O')
        cells[y][x].setFillColor(sf::Color::Green);
      else
        cells[y][x].setFillColor(sf::Color::Transparent);
      cells[y][x].setPosition({ static_cast<float>(x * cell_size),
                                static_cast<float>(y * cell_size) });
    }
  }

  Player* p = Player::getInstance();

  while (window.isOpen()) {
    if (lab.checkWinner()) {
      std::cout << "You win!\n";
      window.close();
    }
    // check all the window's events that were triggered since the last
    // iteration of the loop
    while (const std::optional event = window.pollEvent()) {
      // "close requested" event: we close the window
      if (event->is<sf::Event::Closed>())
        window.close();
      else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::W)
          p->move(Direction::Up);
        if (keyPressed->scancode == sf::Keyboard::Scancode::A)
          p->move(Direction::Left);
        if (keyPressed->scancode == sf::Keyboard::Scancode::S)
          p->move(Direction::Down);
        if (keyPressed->scancode == sf::Keyboard::Scancode::D)
          p->move(Direction::Right);
      }
    }
    // clear the window with black color
    window.clear(sf::Color::Black);

    // Draw map
    for (const auto& row : cells) {
      for (const auto& cell : row) {
        window.draw(cell);
      }
    }
    window.draw(Player::getInstance()->getShape());
    // end the current frame
    window.display();
  }

  return 0;
}
