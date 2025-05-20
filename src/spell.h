#ifndef SPELL_H
#define SPELL_H

#include "player.h"

class Spell{
public:
  virtual bool ActivateEffect(Player& self, Player& opponent) = 0;
  virtual bool ActivateEffect(Player& self, Player& opponent, int attackerIndex);
};

class OshamaScramble : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

class DarkMagic : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

class ReEndOfADream : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};
class WorldVanquisher : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

class FlowerSnowDrumNBass : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};
class DragonUnited : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};


class Destr0yer : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

class DisortedFate : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

class BurstStreamofDestruction : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

class RageofTheBlueEyes : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

class BondBetweenTheTeacherandStudent : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

class DarkBurningMagic : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

class ThePowerofFriendship : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

//Trap Card
class MirrorForce : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

class Tsunagite : public Spell{
  bool ActivateEffect(Player& self, Player& opponent) override;
};

class Trrricksters : public Spell{
  bool ActivateEffect(Player& self, Player& opponent, int attackerIndex) override;
  bool ActivateEffect(Player& self, Player& opponent) override;
};
//cursed technique: Strategy Pattern
//Step 1:Declare Spell here



#endif
