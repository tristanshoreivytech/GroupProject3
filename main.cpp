#include <SDL.h>
#include <time.h>
#include "main.h"
#include "board.h"
#include "game.h"
#include "gui.h"
#include <iostream>
using namespace std;


int main() {
    SDL_Window* window = GUI::createSDLWindow();
    SDL_Renderer* renderer = GUI::createSDLRenderer(window);

    SDL_Event event;

    Board board;

    board.readFen(STARTING_FEN);
    board.printBoard();

    bool running = true;
    bool gameover = false;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONUP && !gameover) {
                GUI::handleMouseClicked(event.button, &board);
            }
            else if (event.type == SDL_KEYUP && !gameover) {
                GUI::handleKeyPressed(event.key, &board);
            }
        }

        if (gameover) {
            continue;
        }

        if (Game::isInCheckMate(&board, DIAMOND)) {
            std::cout << "DIAMOND IS CHECKMATED" << std::endl;
            gameover = true;
        }
        if (Game::isInCheckMate(&board, PEARL)) {
            std::cout << "PEARL HAS BEEN CHECKMATED" << std::endl;
            gameover = true;
        }

        SDL_RenderClear(renderer);
        GUI::drawChessboard(renderer, &board);

        SDL_RenderPresent(renderer);
    }

    GUI::cleanupSDL(renderer, window);

    return 0;
}