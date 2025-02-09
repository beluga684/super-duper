#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "src/GameOfLife.h"

const int CELL_SIZE = 10;
const int GRID_WIDTH = 80;
const int GRID_HEIGHT = 60;

sf::Vector2i hoveringPos({-1, -1});
std::vector<std::vector<int>> selectedPat;
bool isPatternChecked = false;

const std::vector<std::vector<int>> PAT_GLIDER = {
    {0, 1, 0},
    {0, 0, 1},
    {1, 1, 1}
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE}), "game of life");
    tgui::Gui gui{window};

    GameOfLife game(GRID_WIDTH, GRID_HEIGHT);
    bool isRunning = false;
    int updateDelay = 100;

    // Кнопки
    auto startButton = tgui::Button::create("▶");
    startButton->setSize({"5%","5%"});
    startButton->setPosition({"5%","5%"});
    gui.add(startButton);

    auto resetButton = tgui::Button::create("x");
    resetButton->setSize("5%","5%");
    resetButton->setPosition("12%","5%");
    gui.add(resetButton);

    auto gliderButton = tgui::Button::create("глайдер");
    gliderButton->setSize({"10%","5%"});
    gliderButton->setPosition({"50%","50%"});
    gui.add(gliderButton);

    // Слайдер скорости
    auto speedSlider = tgui::Slider::create(10, 200);
    speedSlider->setSize("15%", "2%");
    speedSlider->setPosition("20%", "5%");
    speedSlider->setValue(updateDelay);
    gui.add(speedSlider);

    // Надписи
    auto speedLabel = tgui::Label::create("Задержка (мс): " + std::to_string(updateDelay));
    speedLabel->setPosition("19.5%", "8%");
    speedLabel->getRenderer()->setTextColor(tgui::Color::White);
    gui.add(speedLabel);

    auto genLabel = tgui::Label::create("Поколение: 0");
    genLabel->setPosition("80%", "5%");
    genLabel->getRenderer()->setTextColor(tgui::Color::White);
    gui.add(genLabel);

    auto aliveLabel = tgui::Label::create("Население: 0");
    aliveLabel->setPosition("80%", "10%");
    aliveLabel->getRenderer()->setTextColor(tgui::Color::White);
    gui.add(aliveLabel);

    // Функции для кнопок и слайдера
    startButton->onPress([&](){
        isRunning = !isRunning;
        startButton->setText(isRunning ? "||" : "▶");
    });

    resetButton->onPress([&](){
        game.clear();

        isRunning = false;
        startButton->setText("▶");
    });

    gliderButton->onPress([&]() {
        isPatternChecked = true;
        selectedPat = PAT_GLIDER;
    });

    speedSlider->onValueChange([&](float value) {
        updateDelay = static_cast<int>(value);
        speedLabel->setText("Задержка (мс): " + std::to_string(updateDelay));
    });

    sf::Clock clock;

    while(window.isOpen()){
        while (auto eventO = window.pollEvent()) {
            sf::Event event = *eventO;
            gui.handleEvent(event);

            if (event.is<sf::Event::Closed>())
                window.close();

            if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
                hoveringPos.x = mouseMoved->position.x / CELL_SIZE;
                hoveringPos.y = mouseMoved->position.y / CELL_SIZE;
            }

            if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    if (isPatternChecked) {
                        game.setPattern(hoveringPos.x, hoveringPos.y, selectedPat);
                        isPatternChecked = false;
                    }
                    else if (mouseButtonPressed->position.y < GRID_HEIGHT * CELL_SIZE) {
                        int x = mouseButtonPressed->position.x / CELL_SIZE;
                        int y = mouseButtonPressed->position.y / CELL_SIZE;
                        game.toggleCell(x, y);
                    }
                }
                else if(mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    isPatternChecked = false;
                }
            }
        }

        if (isRunning and clock.getElapsedTime().asMilliseconds() > updateDelay) {
            game.update();
            clock.restart();
        }

        genLabel->setText("Поколение: " + std::to_string(game.getGeneration()));
        aliveLabel->setText("Население: " + std::to_string(game.getAliveCells()));

        window.clear();

        sf::VertexArray gridLines(sf::PrimitiveType::Lines);
        sf::Color gridColor(25, 25, 25);

        gridLines.resize((GRID_WIDTH + 1 + GRID_HEIGHT + 1) * 2);
        size_t index = 0;

        // Вертикальные линии
        for (int x = 0; x <= GRID_WIDTH; x++) {
            gridLines[index].position = sf::Vector2f(x * CELL_SIZE, 0);
            gridLines[index].color = gridColor;
            index++;

            gridLines[index].position = sf::Vector2f(x * CELL_SIZE, GRID_HEIGHT * CELL_SIZE);
            gridLines[index].color = gridColor;
            index++;
        }

        // Горизонтальные линии
        for (int y = 0; y <= GRID_HEIGHT; y++) {
            gridLines[index].position = sf::Vector2f(0, y * CELL_SIZE);
            gridLines[index].color = gridColor;
            index++;

            gridLines[index].position = sf::Vector2f(GRID_WIDTH * CELL_SIZE, y * CELL_SIZE);
            gridLines[index].color = gridColor;
            index++;
        }

        window.draw(gridLines);

        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE));
                cell.setFillColor(game.isAlive(x, y) ? sf::Color::White : sf::Color::Black);
                window.draw(cell);
            }
        }

        if (isPatternChecked) {
            for (int i = 0; i < selectedPat.size(); i++){
                for (int j = 0; j < selectedPat[i].size(); j++){
                    if (selectedPat[i][j]){
                        sf::RectangleShape ghost(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                        ghost.setPosition({static_cast<float>((hoveringPos.x + j) * CELL_SIZE), static_cast<float>((hoveringPos.y + i) * CELL_SIZE)});
                        ghost.setFillColor(sf::Color(50, 50, 50));
                        window.draw(ghost);
                    }
                }
            }
        }

        gui.draw();
        window.display();
    }
    return 0;
}