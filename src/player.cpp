#include "player.h"
#include "monstercard.h"
#include "spellcard.h"
#include "trapcard.h"
#include <iostream>
using namespace std;

Player :: Player(){
    hp = 4000;
}

void Player :: drawCard(){
    if(!deck.empty()){
        hand.push_back(deck.back());
        deck.pop_back();
    }
    else{
        cout << "Unable to draw card" << endl;
    }
}

void Player :: playMonster(int handIndex){
    if(handIndex >= 0 && handIndex < hand.size()){
        if(dynamic_cast<MonsterCard*>(hand[handIndex])){
            field.push_back(hand[handIndex]);
            hand.erase(hand.begin() + handIndex);
        }
        else{
            cout << "This is not a monster card" << endl;
        }
    }
}

void Player :: activateSpell(int handIndex){
    if(handIndex >= 0 && handIndex < hand.size()){
        if(dynamic_cast<SpellCard*>(hand[handIndex])){
            field.push_back(hand[handIndex]);
            hand.erase(hand.begin() + handIndex);
        }
        else{
            cout << "This is not a spell card" << endl;
        }
    }
}

void Player :: setTrap(int handIndex){
    if(handIndex >= 0 && handIndex < hand.size()){
        if(dynamic_cast<TrapCard*>(hand[handIndex])){
            field.push_back(hand[handIndex]);
            hand.erase(hand.begin() + handIndex);
        }
        else{
            cout << "This is not a trap card" << endl;
        }
    }
}

void Player :: showHand() const{
    cout << "Hand: " << endl;
    for(int i = 0 ; i < hand.size() ; i++){
        cout << i << " ";
        hand[i] -> showInfo();
        cout << endl;
    }
}

void Player :: showField() const{
    cout << "Field: " << endl;
    for(int i = 0 ; i < field.size() ; i++){
        cout << i << " ";
        field[i] -> showInfo();
        cout << endl;
    }
}

void Player :: takeDamage(int amount){
    hp -= amount;
    if(hp < 0) hp = 0;
}

int Player :: getHp() const{
    return hp;
}

vector<Card*>& Player :: getDeck(){
    return deck;
}

vector<Card*>& Player :: getField(){
    return field;
}

void Player :: loadDeckDarkMagician(){
    deck.push_back(new MonsterCard("Dark Magician", 2500, 2100, "The ultimate wizard in term of attack and defense."));
    deck.push_back(new MonsterCard("Dark Magician", 2500, 2100, "The ultimate wizard in term of attack and defense."));
    deck.push_back(new MonsterCard("Dark Magician Girl", 2000, 1700, "Dark Magician favourite student."));
    deck.push_back(new MonsterCard("Kuriboh", 0, 0, "Little fluffy ball that like to help others."));
    deck.push_back(new MonsterCard("Giant Soldier of Stone", 1300, 2000, "Little fluffy ball that like to help others."));
    deck.push_back(new MonsterCard("Mammoth Graveyard", 1200, 800, "A mammoth that protects the graves of its pack and is absolutely merciless when facing grave-robbers."));
    deck.push_back(new MonsterCard("Curse of Dragon", 2000, 1500, "A wicked dragon that taps into dark forces to execute a powerful attack."));
    deck.push_back(new MonsterCard("Griffore", 1200, 1500, "This monster's tough hide deflects almost any attack."));
    deck.push_back(new MonsterCard("Beaver Warrior", 1200, 1500, "What this creature lacks in size it makes up for in defense when battling in the prairie."));
    deck.push_back(new MonsterCard("Mystical Elf", 800, 2000, "A delicate elf that lacks offense, but has a terrific defense backed by mystical power."));
    deck.push_back(new SpellCard("Re:End of a dream", "If and only if your lifepoint is lower or equal to 500 you can activate this card. When this card is activated, you can conduct anorther turn"));
    deck.push_back(new SpellCard("Disorted Fate", "Look at your full deck and place a card you like at the top of the deck."));
    deck.push_back(new SpellCard("Oshama Scramble", "Shuffle your opponent's deck when this card is activated."));
    deck.push_back(new SpellCard("Dark Magic", " If you controll a “Dark Magician” monster, destroy all spell and trap card your opponent controll."));
    deck.push_back(new SpellCard("Bond Between the teacher and student", "If you controll a “Dark Magician”, special summon from your deck a dark magician girl but skip your battle phase"));
    deck.push_back(new SpellCard("Dark Burning Magic", "If you control a “Dark Magician”, “Dark magician girl” you can destroy all of your oponent card but skip your 2 battle phase"));
    deck.push_back(new SpellCard("The Power of Friendship", "Combine all of your faceup monster atk to 1 single card in this turn."));
    deck.push_back(new TrapCard("Mirror force", "When an opponent's monster declares an attack: Destroy all your opponent's Attack Position monsters."));
    deck.push_back(new TrapCard("Tsunagite", "When an opponent's monster declares an attack: Negate the attack and end the battle phase."));
    deck.push_back(new TrapCard("Trrricksters!!", "When an opponent's monster declares an attack: Target the attacking monster, and if you do, inflict damage to your opponent equal to its atk."));
}

void Player :: loadDeckBlueEyes(){
    deck.push_back(new MonsterCard("Blue-Eyes White Dragon", 3000, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale."));
    deck.push_back(new MonsterCard("Blue-Eyes White Dragon", 3000, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale."));
    deck.push_back(new MonsterCard("T+pazolite", 1750, 200, "A crystal dragon born from solidified molten lava. T+pazolite’s scales shimmer like gemstones, absorbing and reflecting light to confuse enemies on the battlefield."));
    deck.push_back(new MonsterCard("Morimori Atsush", 1800, 0, "A nomadic dragon god wandering through the eastern forests. With no defensive armor, it relies on immense speed and broad wings to outmaneuver and strike down any who dare approach."));
    deck.push_back(new MonsterCard("Feryquitous", 1500, 400, "A young flame dragon constantly yearning to prove its might. Its immature blue flames often ignite unexpected wildfires across the land."));
    deck.push_back(new MonsterCard("Kuroma", 2100, 700, "A shadow dragon, slender yet deadly. Kuroma hides within dark clouds, striking without warning and bringing silent destruction to its prey."));
    deck.push_back(new MonsterCard("Laur", 1450, 1000, "An ancient dragon that once guarded sacred lands. Though its strength has waned with time, Laur’s unwavering spirit remains, ready to sacrifice itself to halt any invader’s advance."));
    deck.push_back(new MonsterCard("Sasakure", 1900, 0, "A razorwind dragon whose translucent body leaves only trails of slashing winds. Without any defenses, it relies entirely on blinding speed and lethal precision."));
    deck.push_back(new MonsterCard("BlackY", 1500, 500, "A black dragon born from the ashes of fallen stars. Though not powerful in offense, BlackY possesses the stubborn will to rise from ruins and survive every battle."));
    deck.push_back(new MonsterCard("Camellia", 2000, 1600, "A divine flower dragon, rare and mesmerizing. Camellia commands deadly blossoms, blending beauty with fatal poison in a delicate yet deadly dance."));
    deck.push_back(new MonsterCard("Sakuzyo", 2300, 900, "A sonic dragon traveling across dimensions through ultrasonic waves. Sakuzyo’s roar can shatter metal, and those who hear it often lose their sense of direction completely."));
    deck.push_back(new SpellCard("Re:End of a dream", "If and only if your lifepoint is lower or equal to 500 you can activate this card. When this card is activated, you can conduct anorther turn"));
    deck.push_back(new SpellCard("Disorted Fate", "Look at your full deck and place a card you like at the top of the deck."));
    deck.push_back(new SpellCard("Oshama Scramble", "Shuffle your opponent's deck when this card is activated."));
    deck.push_back(new SpellCard("Flower Snow Drum'n'Bass", "Flip coin, winner gets draw 2 cards"));
    deck.push_back(new SpellCard("WorldVanquisher", "Buff a monster attack by 200 when this card is activated."));
    deck.push_back(new SpellCard("Dragon United", "Buff a monster attack and defense by 100 for each faceup monster you control when this card is activated."));
    deck.push_back(new SpellCard("Burst Stream of Destruction", "If you controll a Blue eye white dragon destroy all card your opponent controll but skip your next 2 battle phase."));
    deck.push_back(new SpellCard("Rage of the blue eyes", "Sacrifice 2/3 of your lifepoint, “Blue Eyes White Dragon” you controll can now attack twice in a turn ( can not active “Re:End of a dream” the turn you activate this card).")); 
}




