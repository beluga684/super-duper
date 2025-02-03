#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <vector>

class GameOfLife {
public:
    GameOfLife(int width, int height);

    void update();
    void toggleCell(int x, int y);
    void clear();
    bool isAlive(int x, int y) const;
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    int width, height;
    std::vector<std::vector<bool>> grid;
    std::vector<std::vector<bool>> nextGrid;
    
    int countNeighbors(int x, int y) const;
};

#endif