#include <SFML/Graphics.hpp>
#include <array>
#include <random>

void DrawGrid(sf::RenderWindow& window) {
    int padding = 30;
    int offset = 10;
    for (int i = 0; i < 10; i++) {
        // Draw vertical line
        sf::Vertex verticalLine[] = {sf::Vector2f(padding * i + offset, offset),
                                     sf::Vector2f(padding * i + offset, padding * 9 + offset)};
        window.draw(verticalLine, 2, sf::Lines);

        // Draw horizontal line
        sf::Vertex horizontalLine[] = {sf::Vector2f(offset, padding * i + offset),
                                       sf::Vector2f(padding * 9 + offset, padding * i + offset)};
        window.draw(horizontalLine, 2, sf::Lines);
    }
}

void DisplayPuzzle(sf::RenderWindow& window, sf::Text& text, const std::array<std::array<int, 9>, 9>& puzzle) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (puzzle[i][j] == 0) {
                text.setString(' ');
                window.draw(text);
            } else {
                text.setString(std::to_string(puzzle[i][j]));
                /*  shift 15 pixels to right and 6 pixels down so they appear inside the cells  */
                text.setPosition(j * text.getCharacterSize() + 15, i * text.getCharacterSize() + 6);
                window.draw(text);
            }
        }
    }
}

bool Possible(int row, int col, int num, const std::array<std::array<int, 9>, 9>& puzzle) {
    for (int i = 0; i < 9; i++) {
        if (puzzle[row][i] == num || puzzle[i][col] == num)
            return false;
    }

    row = (row / 3) * 3;
    col = (col / 3) * 3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (puzzle[row + i][col + j] == num)
                return false;
        }
    }

    return true;
}

void ShuffleArray(std::array<int, 9>& arr) {
    std::mt19937 gen(std::random_device{}());
    std::shuffle(arr.begin(), arr.end(), gen);
}

bool GenSudoku(std::array<std::array<int, 9>, 9>& puzzle) {
    std::array<int, 9> values = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (puzzle[i][j] != 0)
                continue;
            ShuffleArray(values);
            for (int n = 0; n < 9; n++) {
                if (!Possible(i, j, values[n], puzzle))
                    continue;
                puzzle[i][j] = values[n];
                if (GenSudoku(puzzle))
                    return true;
                puzzle[i][j] = 0;
            }
            return false;
        }
    }
    return true;
}
