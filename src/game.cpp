#include "game.h"
#include "player.h"
#include <algorithm>
#include <random>
#include <ctime>

void GameState::startGame(Player& p1, Player& p2) {
    p1.shuffleDeck();
    p2.shuffleDeck();

    for (int i = 0; i < 5; ++i) {
        p1.drawCard();
        p2.drawCard();
    }
}


GameState::GameState(Player& p1, Player& p2)
{
    player1 = p1;
    player2 = p2;
}
