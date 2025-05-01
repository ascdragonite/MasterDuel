#include "card.h"
#include "json.hpp"
#include "serialize.h"
#include "monstercard.h"
#include "player.h"
#include "spellcard.h"
#include "trapcard.h"
#include <stdexcept>
#include <vector>

using json = nlohmann::json;
using namespace std;

vector<json> SerializeDeck(const vector<Card*> deck)
{
    vector<json> deckJson;
    for(const Card* cardPtr : deck)
    {
        if (cardPtr != nullptr)
        {
            deckJson.push_back(cardPtr->toJson());
        }
    }
    return deckJson;
}


Card* CardFromJson(const json j)
{
    string type = j.at("type");
    if(type == "Monster")
    {
        return new MonsterCard(j.at("name"), j.at("atk"), j.at("def"), j.at("description"));
    }
    else if(type == "Spell")
    {
        return new SpellCard(j.at("name"), j.at("description"));
    }
    else if(type == "Trap")
    {
        return new TrapCard(j.at("name"), j.at("description"));
    }
    else {
        throw runtime_error("what the fuck: " + type);
    }
}

vector<Card*> DeserializeDeck(const json deckJson)
{
    vector<Card*> deck;
    for(const auto& cardJson : deckJson)
    {   
        Card* card = CardFromJson(cardJson);
        if (card)
        {
            deck.push_back(card);
        }
    }
    return deck;
}


void to_json(json& j, const Card& c)
{
    j = {
        {"name", c.getName()},
        {"description", c.getDescription()},
        {"type", c.getType()}
    };
}

void from_json(const json& j, Card& c)
{
    c.SetName(j.at("name"));
    c.SetDescription(j.at("description"));
    c.SetType(j.at("type"));
}


void to_json(json& j, const Player& p)
{
    
    j = {
        {"hp", p.getHp()},
        {"deck", SerializeDeck(p.getDeck())},
        {"hand", SerializeDeck(p.getHand())},
        {"field", SerializeDeck(p.getField())}
    };
}
void from_json(json& j, Player& p)
{
    p.setHp(j.at("hp"));
    p.setDeck(DeserializeDeck(j.at("deck")));
    p.setHand(DeserializeDeck(j.at("hand")));
    p.setField(DeserializeDeck(j.at("field")));
}