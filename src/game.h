#ifndef GAME_H
#define GAME_H
#include "player.h"

class GameState{
    public:
        Player player1;
        Player player2;
        GameState(Player& p1, Player& p2);
        void startGame(Player& p1, Player& p2);
        void playerTurn(Player& self, Player& opponent, bool isFirstTurn, const vector<pair<int, int>>& actionPlan);
        void battlePhase(Player& attacker, Player& defender, int attackIndex, int defendIndex);
        bool checkVictory(const Player& p1, const Player& p2);
};

#endif
