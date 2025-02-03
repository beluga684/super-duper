#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "GameOfLife.h"

const int CELL_SIZE = 10;
const int GRID_WIDTH = 80;
const int GRID_HEIGHT = 60;

int main()
{
    sf::RenderWindow window(sf::VideoMode({GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE}), "клеточный автомат");
    tgui::Gui gui{window};

    GameOfLife game(GRID_WIDTH, GRID_HEIGHT);
    bool isRunning = false;
    
    auto startButton = tgui::Button::create("Старт");
    startButton->setSize({"15%","5%"});
    startButton->setPosition({"5%","5%"});
    gui.add(startButton);

    startButton->onPress([&](){
        isRunning = !isRunning;
        startButton->setText(isRunning ? "Стоп" : "Старт");
    });

    auto resetButton = tgui::Button::create("Перезапуск");
    resetButton->setSize({"15%","5%"});
    resetButton->setPosition({"25%","5%"});
    gui.add(resetButton);

    resetButton->onPress([&](){
        game.clear();
    });

    sf::Clock clock;

    while(window.isOpen()){
        while (auto eventO = window.pollEvent()) {
            sf::Event event = *eventO;
            gui.handleEvent(event);

            if (event.is<sf::Event::Closed>())
                window.close();

            if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                    if (mouseButtonPressed->position.y < GRID_HEIGHT * CELL_SIZE) {
                        int x = mouseButtonPressed->position.x / CELL_SIZE;
                        int y = mouseButtonPressed->position.y / CELL_SIZE;
                        game.toggleCell(x, y);
                    }
            }
        }

        if (isRunning and clock.getElapsedTime().asMilliseconds() > 100) {
            game.update();
            clock.restart();
        }

        window.clear();

        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE));
                cell.setFillColor(game.isAlive(x, y) ? sf::Color::White : sf::Color::Black);
                window.draw(cell);
            }
        }

        gui.draw();
        window.display();
    }
}