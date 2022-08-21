#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <iostream>
#include <random>

static constexpr std::size_t window_width = 800;
static constexpr std::size_t window_height = 500;
static const std::string window_name = "Sudoku puzzles";
static const std::string font_name = "JetBrainsMonoNL-Bold.ttf";

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

/*random shuffle*/
void randSort(std::array<int, 9>& arr) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(arr.begin(), arr.end(), gen);
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

int main() {
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), window_name, sf::Style::Close);

    ImGui::SFML::Init(window);

    sf::Font font;

    /*Checks if font is loaded, if not quit*/
    if (!font.loadFromFile(font_name)) {
        std::cerr << "Font not found, try another path\n";
        return 1;
    }

    /*setting the font*/
    sf::Text text;
    text.setFont(font);

    /*grid*/
    int puzzle[9][9] = {};

    /*variable that will be used for ImGui::Chechbox()*/
    bool makePuzzle = false;

    /*self explanatory*/
    int difficulty = 30;

    /*space between lines of grid (how big the boxes are gonna be)*/
    int space_btw = 30;

    /*we shuffle this array to get random location to put zeros (0)*/
    std::array<int, 9> rArr = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    /*==============================================================*/

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) window.close();
        }

        /*Content of ImGui*/
        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::Begin("Sudoku");
        bool button = ImGui::Button("Solve", ImVec2(75, 25));
        ImGui::SliderInt("Difficulty", &difficulty, 0, 100);
        ImGui::Checkbox("Generate", &makePuzzle);
        ImGui::End();

        window.clear(sf::Color(18, 33, 43));

        drawGrid(window, space_btw);

        /*if "Generate" button is checked, generate new puzzle*/
        if (makePuzzle) {
            /*reset puzzle (make everything a zero (0))*/
            memset(puzzle, 0, sizeof(puzzle));

            gen_sudoku(puzzle);

            /*getting random locations to place set zero (0)*/
            randSort(rArr);
            for (int i = 0; i < difficulty; i++) {
                puzzle[rArr[rand() % 9]][rArr[rand() % 9]] = 0;
            }
        }

        displayPuzzle(window, text, puzzle);

        /*finnaly, solve the puzzle if solution exists*/
        if (button && !solve(puzzle)) {
            std::cerr << "puzzle has no solution\n";
        }

        /*render everything*/
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
