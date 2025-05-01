#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "card.h"
#include "player.h"
#include "json.hpp"
#include <vector>

// Forward declarations for to_json and from_json
void to_json(nlohmann::json& j, const Card& card);
void from_json(const nlohmann::json& j, Card& card);
void to_json(nlohmann::json& j, const Player& player);
void from_json(const nlohmann::json& j, Player& player);
vector<json> SerializeDeck(const vector<Card*> deck);
vector<Card*> DeserializeDeck(const vector<json> deckJson);
Card* CardFromJson(const json j);

#endif // SERIALIZE_H