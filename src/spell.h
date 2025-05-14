#include "player.h"

class Spell{
public:
  virtual bool ActivateEffect(Player& self, Player& opponent) = 0;
};

class OshamaScramble : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

/*class DarkMagic : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};*/

class ReEndOfADream : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

/*class Destroyer : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

class RageOfTheBlueEyes : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

//cursed technique: Strategy Pattern
//Step 1:Declare Spell here
*/