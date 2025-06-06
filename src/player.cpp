#include "player.h"
#include "card.h"
#include "monstercard.h"
#include "spellcard.h"
#include "trapcard.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <vector>
#include <ctime>
#include "serialize.h"


using namespace std;

int Player::getIndex() const
{
    return index;
}

void DumpInfo(Player& player)
{
    json j;
    ifstream in("game_state.json");
    if (in) {
        in >> j;
        in.close();
    }
    j["Player" + to_string(player.getIndex())] = player;
    ofstream out("game_state.json");
    if (!out) {
        cerr << "Error opening file for writing.\n";
        return;
    }
    out << j.dump(4) << std::endl; // Pretty print with indent
    out.close();
}

void Player::setSkipBattlePhaseCount(int count)
{ 
    if (count < 0) {
        skipBattlePhaseCount = 0;
        return;
    }
    skipBattlePhaseCount = count; 
}

int Player::getSkipBattlePhaseCount() const
{ return skipBattlePhaseCount; }

Player :: Player(int i){
    index = i;
    hp = 8000;
    skipBattlePhaseCount = 0;
    canUsePowerOfFriendship = false; 
}

Player::~Player()
{
    for(Card* card : deck)
    {
        delete card;
    }
    for(Card* card : field)
    {
        delete card;
    }
    for(Card*card : hand)
    {
        delete card;
    }
}

void Player :: drawCard(){
    if(!deck.empty()){
        hand.push_back(deck.back());
        deck.pop_back();
    }
    DumpInfo(*this);
}

void Player::Summon(int handIndex) {
    if (handIndex >= 0 && handIndex < static_cast<int>(hand.size())) {
        Card* card = hand[handIndex];
        MonsterCard* monster = dynamic_cast<MonsterCard*>(card);

        if (monster) {

            monster->setJustSummoned(true);
            
        }

        card->PlayCard(field); // thêm vào sân (và hỏi attack/defense mode)
        hand.erase(hand.begin() + handIndex);
    }

    DumpInfo(*this);
}

void Player::resetAttackFlags() {
    attackedThisTurn = vector<bool>(5, false);
}

bool Player::hasAttacked(int index) const {
    return index >= 0 && index < attackedThisTurn.size() && attackedThisTurn[index];
}

void Player::setAttacked(int index, bool value) {
    if(index >= 0 && index < attackedThisTurn.size())
        attackedThisTurn[index] = value;
}



void Player::switchPosition(int fieldIndex) {
    if (fieldIndex < 0 || fieldIndex >= field.size()) {
        cout << "Invalid index.\n";
        return;
    }

    MonsterCard* m = dynamic_cast<MonsterCard*>(field[fieldIndex]);
    if (!m) {
        cout << "This is not a monster card.\n";
        return;
    }

    if (m->isFacedown()) {
        cout << "Please use function reveal monster to switch the position of a facedown monster.\n";
        return;
    }

    if (hasAttacked(fieldIndex)) {
        cout << "This monster already attacked this turn.\n";
        return;
    }

    if (m->isJustSummoned()) {
        cout << "You cannot switch the position of this monster right now.\n";
        return;
    }

    // Toggle position
    bool newPos = !m->isInDefense();
    m->setDefenseMode(newPos);

    cout << m->getName() << " switched to " << (newPos ? "Defense" : "Attack") << " position.\n";

    DumpInfo(*this);
}

void Player::revealMonster(int fieldIndex) {
    if (fieldIndex >= 0 && fieldIndex < field.size()) {
        MonsterCard* m = dynamic_cast<MonsterCard*>(field[fieldIndex]);
        if (m && m->isFacedown()) {
            m->reveal();                       
            m->setDefenseMode(false);          
            field[fieldIndex]->showInfo();
        }
    }
    DumpInfo(*this);
}




/* 
vector<string> Player :: getHandInfo() const{
    vector<string> info;
    for(auto card : hand){
        info.push_back(card -> getName());
    }
    return info;
}

vector<string> Player::getFieldInfo() const {
    vector<string> info;
    for (auto card : field) {
        info.push_back(card->getName());
    }
    return info;
} */

void Player :: takeDamage(int amount){
    hp -= amount;
    if(hp < 0) hp = 0;
}

int Player :: getHp() const{
    return hp;
}

vector<Card*> Player::getHand() const {
    return hand;
}

vector<Card*> Player :: getDeck() const {
    return deck;
}

vector<Card*> Player :: getField() const{
    return field;
}

void Player::setHand(vector<Card*> newHand)
{
    for(Card* card : hand)
    {
        //delete card;
    }
    hand.clear();

    hand = newHand;
}

void Player::setField(vector<Card*> newField)
{
    for(Card* card : field)
    {
        //delete card;
    }
    field.clear();

    field = newField;

    attackedThisTurn.resize(5, false);
}
void Player::setDeck(vector<Card*> newDeck)
{
    for(Card* card : deck)
    {
        //delete card;
    }
    deck.clear();

    deck = newDeck;
}

vector<Card*>& Player::getDeckRef() {
    return deck;
}

vector<Card*>& Player::getFieldRef() {
    return field;
}

void Player::setHp(int hp)
{
    this -> hp = hp; 
}

vector<Card*>& Player::getHandRef() {
    return hand;
}

bool Player::getCanUsePowerOfFriendship() const {
    return canUsePowerOfFriendship;
}

void Player::setCanUsePowerOfFriendship(bool value) {
    canUsePowerOfFriendship = value;
}

bool Player::getCanUseReEnd() const {
    return canUseReEnd;
}

void Player::setCanUseReEnd(bool value) {
    canUseReEnd = value;
}


void Player :: loadDeckDarkMagician(){
    cout << "Loading deck for player " << index << endl;
    deck.push_back(new MonsterCard("Dark Magician", 2500, 2100, "Dark Magician the ultimate wizard in term of attack and defense.", index));
    deck.push_back(new MonsterCard("Dark Magician", 2500, 2100, "Dark Magician the ultimate wizard in term of attack and defense.", index));
    deck.push_back(new MonsterCard("Dark Magician", 2500, 2100, "Dark Magician the ultimate wizard in term of attack and defense.", index));
    deck.push_back(new MonsterCard("Magicians Apprentice", 2000, 1700, "A devoted young sorcerer who studies every incantation recorded by the legendary Dark Magician. Though her power is still blooming, her will to protect her master is unwavering.", index));
    deck.push_back(new MonsterCard("Dark Magician Girl", 2000, 1700, "Dark Magician favourite student.", index));
    deck.push_back(new MonsterCard("Dark Magician Girl", 2000, 1700, "Dark Magician favourite student.", index));
    deck.push_back(new MonsterCard("Magicians Soul", 0, 0, "The remnant spirit of the first disciple who ever trained under the Dark Magician. Though he possesses no form of his own, his essence amplifies the will of every magician he supports.", index));
    deck.push_back(new MonsterCard("Black Luster Soldier", 3200, 3000, "A noble warrior summoned through ancient chaos rituals inspired by the teachings of the Dark Magician. Though his path diverges, their souls resonate through battle and destiny.", index));
    deck.push_back(new MonsterCard("Dark Magician of Chaos", 2800, 2600, "A dark reincarnation born when the powers of the Dark Magician merge with the forbidden magic of chaos. His mastery over destruction and revival surpasses mortal boundaries.", index));
    deck.push_back(new MonsterCard("Magicians Rod", 1600, 100, "This mystical staff has chosen its wielder and resonates only with those who follow the path of the Dark Magician. Channeling ancient magic, it guides spellcasters to the secrets long forgotten.", index));
    deck.push_back(new SpellCard("Dark Magic Veil", "Special summon a Dark Magician in the deck in defend position."));
    deck.push_back(new SpellCard("Dark Magic Veil", "Special summon a Dark Magician in the deck in defend position."));
    deck.push_back(new SpellCard("Oshama Scramble", "Shuffle your opponent's deck when this card is activated."));
    deck.push_back(new SpellCard("Disorted Fate", "Look at your full deck and place a card you like at the top of the deck."));
    deck.push_back(new SpellCard("World Vanquisher", "Buff a monster attack by 1000 when this card is activated."));
    deck.push_back(new SpellCard("Cruel Pact", "Pay 500 Hp and tribute 1 monster on the field to search 1 Dark Magician from your deck and increase 1000 ATK for the Dark Magician summon next turn."));
    deck.push_back(new SpellCard("Thousand Knifes", "If you control faceup Dark Magician: Target 1 monster your opponent controls; destroy that target.."));
    deck.push_back(new SpellCard("Thousand Knifes", "If you control faceup Dark Magician: Target 1 monster your opponent controls; destroy that target.."));
    deck.push_back(new SpellCard("Dark Magic", " If you controll faceup a Dark Magician monster, destroy all monster your opponent controll and skip your battle phase."));
    deck.push_back(new SpellCard("Bond Between The Teacher and Student", "If you controll a faceup Dark Magician, special summon from your deck a Dark Magician Girl in defend position"));
    deck.push_back(new SpellCard("Bond Between The Teacher and Student", "If you controll a faceup Dark Magician, special summon from your deck a Dark Magician Girl in defend position"));
    deck.push_back(new SpellCard("Dark Burning Magic", "If you control a face up Dark Magician, Dark magician girl you can destroy all of your oponent cards but your battle phase this turn."));
    deck.push_back(new SpellCard("The Power of Friendship", "Combine all of your faceup monster atk to 1 single attack in this turn, you can not use this card on your first turn and if you have battle this turn you cannot activate this card and if you activate this card, you cannot battle for the rest of this turn."));
    deck.push_back(new TrapCard("Mirror Force", "When an opponent's monster declares an attack: Destroy all your opponent's Attack Position monsters your opponent cannot attack you this turn."));
    deck.push_back(new TrapCard("Tsunagite", "When an opponent's monster declares an attack: Negate the attack and end the battle phase."));
    deck.push_back(new TrapCard("Trrricksters!!", "When an opponent's monster declares an attack: Target the attacking monster, and if you do, inflict damage to your opponent equal to its atk your opponent cannot attack you this turn."));
    deck.push_back(new TrapCard("Trrricksters!!", "When an opponent's monster declares an attack: Target the attacking monster, and if you do, inflict damage to your opponent equal to its atk and your opponent cannot attack you this turn."));
}

void Player :: loadDeckBlueEyes(){
    cout << "Loading deck for player " << index << endl;
    deck.push_back(new MonsterCard("Blue-Eyes White Dragon", 3000, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.", index));
    deck.push_back(new MonsterCard("Blue-Eyes White Dragon", 3000, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.", index));
    deck.push_back(new MonsterCard("Blue-Eyes White Dragon", 3000, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.", index));
    deck.push_back(new MonsterCard("T+pazolite", 1750, 200, "A crystal dragon born from solidified molten lava. T+pazolite's scales shimmer like gemstones, absorbing and reflecting light to confuse enemies on the battlefield.", index));
    deck.push_back(new MonsterCard("Morimori Atsushi", 1800, 0, "A nomadic dragon god wandering through the eastern forests. With no defensive armor, it relies on immense speed and broad wings to outmaneuver and strike down any who dare approach.", index));
    deck.push_back(new MonsterCard("Feryquitous", 1500, 400, "A young flame dragon constantly yearning to prove its might. Its immature blue flames often ignite unexpected wildfires across the land.", index));
    deck.push_back(new MonsterCard("BLue-Eyes Alternative White Dragon", 3000, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.", index));
    deck.push_back(new MonsterCard("Deep-Eyes White Dragon", 3500, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.", index));
    deck.push_back(new MonsterCard("Kuroma", 2100, 700, "A shadow dragon, slender yet deadly. Kuroma hides within dark clouds, striking without warning and bringing silent destruction to its prey.", index));
    deck.push_back(new MonsterCard("Laur", 1450, 1000, "An ancient dragon that once guarded sacred lands. Though its strength has waned with time, Laurs unwavering spirit remains, ready to sacrifice itself to halt any invader’s advance.", index));
    deck.push_back(new SpellCard("Re:End of a Dream", "If and only if your lifepoint is lower or equal to 1000 you can activate this card. When this card is activated, you can conduct anorther turn"));
    deck.push_back(new SpellCard("Disorted Fate", "Look at your full deck and place a card you like at the top of the deck."));
    deck.push_back(new SpellCard("Oshama Scramble", "Shuffle your opponent's deck when this card is activated."));
    deck.push_back(new SpellCard("Flower Snow Drum n Bass", "Flip coin, winner gets draw 2 cards"));
    deck.push_back(new SpellCard("Flower Snow Drum n Bass", "Flip coin, winner gets draw 2 cards"));
    deck.push_back(new SpellCard("World Vanquisher", "Buff monster attack by 1000 when this card is activated."));
    deck.push_back(new SpellCard("Call of the Sky", "Tribute 2 monster on the field to search 1 BLue Eyes White Dragon and Majesty of The White Dragons from your deck to your hand."));
    deck.push_back(new SpellCard("Dragon United", "Buff a monster attack and defense by 500 for each faceup monster you control when this card is activated."));
    deck.push_back(new SpellCard("Burst Stream of Destruction", "If you controll a faceup Blue Eyes White Dragon destroy all card your opponent controll but skip your next 1 battle phase."));
    deck.push_back(new SpellCard("Rage of The Blue Eyes", "Sacrifice 2/3 of your lifepoint, Blue Eyes White Dragon you control can now attack again in this turn.")); 
    deck.push_back(new SpellCard("Destr0yer",("Can destroy a monster card in defense mode")));
    deck.push_back(new SpellCard("Roar of The Blue-Eyed Dragons", "Special summon a Blue Eyes White Dragon in the deck in defend position."));
    deck.push_back(new SpellCard("Roar of The Blue-Eyed Dragons", "Special summon a Blue Eyes White Dragon in the deck in defend position."));
    deck.push_back(new SpellCard("Majesty of The White Dragons","Destroy random card in your opponent field up to the number of faceup Blue Eyes White Dragon you control"));
    deck.push_back(new SpellCard("Majesty of The White Dragons","Destroy random card in your opponent field up to the number of faceup Blue Eyes White Dragon you control"));
    deck.push_back(new SpellCard("Ash Again","Destroy all trap card in your opponent field if you control faceup Blue Eyes White Dragon"));
    deck.push_back(new SpellCard("Ash Again","Destroy all trap card in your opponent field if you control faceup Blue Eyes White Dragon"));
}

void Player::shuffleDeck(){
    auto rng = default_random_engine(time(0));
    shuffle(deck.begin(), deck.end(), rng);
}





