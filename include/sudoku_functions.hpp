#pragma once
#include <SFML/Graphics.hpp>
/*draw the grid to the window*/
void drawGrid(sf::RenderWindow& window, int space_btw);

/*draw the numbers inside the cells of the grid*/
void displayPuzzle(sf::RenderWindow& window, sf::Text& text, int (&puzzle)[9][9]);

/*checks if position of input number is valid*/
bool possible(int row, int col, int num, int (&puzzle)[9][9]);

/*solves the puzzle, technically we don't really need this since we have already generated a valid "solved" puzzle*/
bool solve(int (&puzzle)[9][9]);

/*random shuffle*/
void randSort(std::array<int, 9>& arr);

/*generates a valid sudoku puzzles*/
bool gen_sudoku(int (&puzzle)[9][9]);
