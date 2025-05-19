#ifndef GAME_H
#define GAME_H
#include "player.h"

class GameState {
private:
  static GameState *instance;
  Player *player1;
  Player *player2;

public:
  static GameState *getInstance();
  Player *getPlayer(int id);
  GameState(Player &p1, Player &p2);
  void startGame();
  void ConsoleClear();
  void playerTurn(Player &self, Player &opponent, bool isFirstTurn);
  void battlePhase(Player &self, Player &opponent, int index, bool& hasBattled);
  bool checkVictory(const Player &p1, const Player &p2);
};

#endif
