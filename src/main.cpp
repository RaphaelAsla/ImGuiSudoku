#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>

#include "sudoku_functions.hpp"

static constexpr std::size_t window_width = 800;
static constexpr std::size_t window_height = 500;
static const std::string window_name = "Sudoku puzzles";
static const std::string font_name = "../font/JetBrainsMonoNL-Bold.ttf";

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
