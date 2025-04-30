#include "game.h"
#include <algorithm>
#include <random>
#include <ctime>

void startGame(Player& p1, Player& p2) {
    p1.shuffleDeck();
    p2.shuffleDeck();

    for (int i = 0; i < 5; ++i) {
        p1.drawCard();
        p2.drawCard();
    }
}

void playerTurn(Player& self, Player& opponent, bool isFirstTurn, const vector<int>& actionPlan) {
    bool hasSummoned = false;
    int actionIndex = 0;

    if (!isFirstTurn) {
        self.drawCard();
    }

    // Main Phase 1
    while (actionIndex < actionPlan.size() && actionPlan[actionIndex] != 3) {
        int action = actionPlan[actionIndex];
        if (action == 1 && !hasSummoned) {
            int handIndex = actionPlan[++actionIndex];
            self.playMonster(handIndex);
            hasSummoned = true;
        } else if (action == 2) {
            int handIndex = actionPlan[++actionIndex];
            self.activateSpell(handIndex);
        }
        ++actionIndex;
    }
    ++actionIndex;
}


