#pragma once
#include <SFML/Graphics.hpp>

/* draw the grid to the window */
void DrawGrid(sf::RenderWindow& window);

/* draw the numbers inside the cells of the grid */
void DisplayPuzzle(sf::RenderWindow& window, sf::Text& text, const std::array<std::array<int, 9>, 9>& puzzle);

/* checks if position of input number is valid */
bool Possible(int row, int col, int num, const std::array<std::array<int, 9>, 9>& puzzle);

/* random shuffle */
void ShuffleArray(std::array<int, 9>& arr);

/* generates a valid sudoku puzzles */
bool GenSudoku(std::array<std::array<int, 9>, 9>& puzzle);
