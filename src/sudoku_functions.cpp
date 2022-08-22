#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <random>
/*draw the grid to the window*/
void drawGrid(sf::RenderWindow& window, int space_btw) {
    for (int i = 1; i <= 9; i++) {
        sf::Vertex line[] = {sf::Vertex(sf::Vector2f(space_btw * i, 0)), sf::Vertex(sf::Vector2f(space_btw * i, space_btw * 9))};
        window.draw(line, 2, sf::Lines);
    }

    for (int i = 1; i <= 9; i++) {
        sf::Vertex line[] = {sf::Vertex(sf::Vector2f(0, space_btw * i)), sf::Vertex(sf::Vector2f(space_btw * 9, space_btw * i))};
        window.draw(line, 2, sf::Lines);
    }
}

/*draw the numbers inside the cells of the grid*/
void displayPuzzle(sf::RenderWindow& window, sf::Text& text, int (&puzzle)[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (puzzle[i][j] != 0) {
                text.setString(std::to_string(puzzle[i][j]));
                text.setPosition(i * text.getCharacterSize() + 5, j * text.getCharacterSize());
                window.draw(text);
            } else {
                /*if puzzle[i][j] is 0 then display ' ' (empty cell)*/
                text.setString(' ');
                window.draw(text);
            }
        }
    }
}

/*checks if position of input number is valid*/
bool possible(int row, int col, int num, int (&puzzle)[9][9]) {
    for (int i = 0; i < 9; i++) {
        if (puzzle[row][i] == num || puzzle[i][col] == num) {
            return false;
        }
    }

    row = (row / 3) * 3;
    col = (col / 3) * 3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (puzzle[row + i][col + j] == num) {
                return false;
            }
        }
    }

    return true;
}

/*solves the puzzle, technically we don't really need this since we have already generated a valid "solved" puzzle*/
bool solve(int (&puzzle)[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (puzzle[i][j] == 0) {
                for (int n = 1; n <= 9; n++) {
                    if (possible(i, j, n, puzzle)) {
                        puzzle[i][j] = n;
                        if (solve(puzzle)) {
                            return true;
                        }
                        puzzle[i][j] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

/*generates a valid sudoku puzzles*/
bool gen_sudoku(int (&puzzle)[9][9]) {
    std::array<int, 9> rArr = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (puzzle[i][j] == 0) {
                randSort(rArr);
                for (int n = 0; n < 9; n++) {
                    if (possible(i, j, rArr[n], puzzle)) {
                        puzzle[i][j] = rArr[n];
                        if (gen_sudoku(puzzle)) {
                            return true;
                        }
                        puzzle[i][j] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

/*random shuffle*/
void randSort(std::array<int, 9>& arr) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(arr.begin(), arr.end(), gen);
}
