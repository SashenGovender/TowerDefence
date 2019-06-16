#include "Player.h"

Player::Player()
{
   _playerType = "";
   _energy = 0;
   _health = 0;
   _hitsTaken = 0;
   _score = 0;
   _activeIronCurtainLifetime = -1;
   _ironCurtainAvailable = false;

   _playerBuildings.reserve(12);
   _playerMissiles.reserve(10);
}

Player::~Player()
{
}

void Player::SetPlayerDetails(JsonHelper* jsonPlayer)
{
   _playerType = jsonPlayer->GetString("playerType");
   _energy = jsonPlayer->GetInt("energy");
   _health = jsonPlayer->GetInt("health");
   _hitsTaken = jsonPlayer->GetInt("hitsTaken");
   _score = jsonPlayer->GetInt("score");
   _activeIronCurtainLifetime = jsonPlayer->GetInt("activeIronCurtainLifetime");
   _ironCurtainAvailable = jsonPlayer->GetBool("ironCurtainAvailable");
}

void Player::AddPlayerMissile(Missile* missile)
{
   _playerMissiles.push_back(missile);
}

void Player::AddPlayerBuilding(int16_t xyIndex, Building* building)
{
   std::unordered_map<int16_t, Building*> ::const_iterator it = _playerBuildings.find(xyIndex);
   if (it == _playerBuildings.end())
   {
      _playerBuildings.insert(std::make_pair(xyIndex, building));
   }
}

