#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <vector>

class GameOfLife {
public:
    GameOfLife(int width, int height);

    void update(); // обновление региона
    void toggleCell(int x, int y); // изменение клетки
    void clear(); // рестарт
    bool isAlive(int x, int y) const; // проверка клетки

    int getGeneration() const {return generationCount;}
    int getAliveCells() const;

private:
    int width, height;
    int generationCount = 0;
    std::vector<std::vector<bool>> grid;
    std::vector<std::vector<bool>> nextGrid;
    
    int countNeighbors(int x, int y) const;
};

#endif