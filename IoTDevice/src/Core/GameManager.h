#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

enum GameState {
    GAME_NONE,
    GAME_BIRDIE_SAYS,
    GAME_WACK_A_BIRD,
    GAME_RED_BIRD_GREEN_BIRD
};

void setGame(GameState newGame);
void updateGame();

#endif