#include "GameOfLife.h"

GameOfLife::GameOfLife(int width, int height) 
    : width(width), height(height), grid(height, std::vector<bool>(width, false)) {}

void GameOfLife::update() {
    nextGrid = grid;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int neighbors = countNeighbors(x, y);

            if (grid[y][x] and (neighbors < 2 or neighbors > 3)) {
                nextGrid[y][x] = false;
            }
            else {
                if (neighbors == 3)
                    nextGrid[y][x] = true;
            }
        }
    }
    grid = nextGrid;
    generationCount++;
}

void GameOfLife::toggleCell(int x, int y) {
    if (x >= 0 and x < width and y >= 0 and y < height)
        grid[y][x] = !grid[y][x];
}

void GameOfLife::clear() {
    for (auto& row : grid)
        row.assign(width, false);
    generationCount = 0;
}

bool GameOfLife::isAlive(int x, int y) const {
    return grid[y][x];
}

int GameOfLife::countNeighbors(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 and dy == 0)
                continue;

            int nx = x + dx, ny = y + dy;
            if (nx >= 0 and nx < width and ny >= 0 and ny < height and grid[ny][nx])
                count++;
        }
    }
    return count;
}

int GameOfLife::getAliveCells() const {
    int count = 0;
    for (const auto& row : grid)
        for (bool cell : row)
            if (cell)
                count++;
    return count;
}

void GameOfLife::setPattern(int x, int y, const std::vector<std::vector<int>>& pattern) {
    for (int i = 0; i < pattern.size(); i++) {
        for (int j = 0; j < pattern[i].size(); j++) {
            int nx = x + j;
            int ny = y + i;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                grid[ny][nx] = pattern[i][j];
            }
        }
    }
}