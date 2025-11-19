#include "generator.hpp"
#include "labyrinth.hpp"
#include "player.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <boost/any.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>
#include <new>

namespace po = boost::program_options;

int
main(int argc, char* argv[])
{
  // Setup logger
  spdlog::set_level(spdlog::level::debug);
  spdlog::set_pattern("[%H:%M:%S] [%n] [%^---%L---%$] [thread %t] %v");

  // CLI
  po::options_description desc("Labyrinth (without minotaur)");
  desc.add_options()("help,h", "Show help")(
    "length,l", po::value<unsigned long>()->default_value(10), "Length of map")(
    "width,w", po::value<unsigned long>()->default_value(10), "Width of map")(
    "algorythm,a",
    po::value<std::string>()->default_value("willson"),
    "Algorythm for generating maze")(
    "cell-size,c",
    po::value<unsigned long>()->default_value(25),
    "Size of square cell");
  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
  } catch (const std::bad_alloc& b) {
    spdlog::critical("Wrong type of argument!");
    return -1;
  } catch (
    const boost::wrapexcept<boost::program_options::invalid_option_value>& e) {
    spdlog::error(
      "Invalid argument for {}! Must be positive integer",
      e.get_option_name());
    return -1;
  }
  po::notify(vm);

  if (vm.count("help") || vm.count("h")) {
    std::cout << desc;
    return 0;
  }
  const unsigned long length_field = vm["length"].as<unsigned long>();
  const unsigned long width_field = vm["width"].as<unsigned long>();
  const unsigned long cell_size = vm["cell-size"].as<unsigned long>();

  // making grid with const size, where cell_size means size of square in pixels
  Labyrinth::init(length_field, width_field);
  Labyrinth& lab = Labyrinth::getInstance();
  if (vm["algorythm"].as<std::string>() == "willson")
    lab.setGenerator(new WilsonGenerator(&lab));
  else if (vm["algorythm"].as<std::string>() == "binary")
    lab.setGenerator(new BinaryTreeGenerator(&lab));
  else if (vm["algorythm"].as<std::string>() == "sidewinder")
    lab.setGenerator(new SidewinderGenerator(&lab));
  else {
    spdlog::error("Wrong algorythm! Using Willson algorythm instead");
    lab.setGenerator(new WilsonGenerator(&lab));
  }
  lab.generateLabyrinth();
  Player::init(&lab, cell_size * 0.49f);
  lab.setPlayer(Player::getInstance());

  const auto& map_represent = lab.getReprOfMap();
  unsigned long rep_length = map_represent[0].size(),
                rep_width = map_represent.size();
  sf::RenderWindow window(
    sf::VideoMode({ (unsigned int)(rep_length * cell_size),
                    (unsigned int)(rep_width * cell_size) }),
    "Labyrinth",
    sf::Style::Close);

  // creating map
  std::vector<std::vector<sf::RectangleShape>> cells(map_represent.size());

  for (size_t y = 0; y < rep_width; ++y) {
    cells[y].resize(rep_length);
    for (size_t x = 0; x < rep_length; ++x) {
      cells[y][x] = sf::RectangleShape({ (float)cell_size, (float)cell_size });
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
