#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <random>

#include "sudoku_functions.hpp"

static constexpr std::size_t window_width = 800;
static constexpr std::size_t window_height = 500;
static const std::string window_name = "Sudoku generator";
static const std::string font_name = "../font/JetBrainsMonoNL-Bold.ttf";

int main() {
    std::mt19937 gen(std::random_device{}());

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), window_name, sf::Style::Close);

    ImGui::SFML::Init(window);

    sf::Font font;

    if (!font.loadFromFile(font_name)) {
        std::cerr << "Font not found, try another path\n";
        return 1;
    }

    sf::Text text;
    text.setFont(font);

    std::array<std::array<int, 9>, 9> puzzle = {};
    std::vector<std::pair<int, int>> coords;
    std::vector<int> values;
    bool generate = false;
    int difficulty = 30;

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        /* ImGui window */
        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::Begin("Sudoku");
        bool solve_bt = ImGui::Button("Solve", ImVec2(75, 25));
        ImGui::SliderInt("Difficulty", &difficulty, 0, 100);
        ImGui::Checkbox("Generate", &generate);
        ImGui::End();

        window.clear(sf::Color(18, 33, 43));

        DrawGrid(window);

        /* if generate button is checked, generate new puzzle */
        if (generate) {
            coords.clear();
            values.clear();

            for (auto& row : puzzle) {
                std::fill(row.begin(), row.end(), 0);
            }

            GenSudoku(puzzle);

            /* getting random locations to appear empty */
            for (int i = 0; i < difficulty; i++) {
                const int row = std::uniform_int_distribution<int>{0, 8}(gen);
                const int col = std::uniform_int_distribution<int>{0, 8}(gen);
                if (puzzle[row][col] != 0) {
                    coords.emplace_back(std::make_pair(row, col));
                    values.emplace_back(puzzle[row][col]);
                    puzzle[row][col] = 0;
                }
            }
            /* else if solve button is pressed, solve the puzzle */
        } else if (solve_bt) {
            for (size_t i = 0; i < coords.size(); i++) {
                puzzle[coords[i].first][coords[i].second] = values[i];
            }
        }

        /* write the stuff into the window buffer */
        DisplayPuzzle(window, text, puzzle);

        /* render everything */
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
