#ifndef PLAYER
#define PLAYER

#include "Building.h"
#include "Missile.h"
#include  <vector>
#include <unordered_map>

class Player
{
private:
   std::string _playerType;
   int16_t  _energy;
   int16_t _health;
   int16_t _hitsTaken;
   int16_t _score;
   std::unordered_map<int16_t, Building*> _playerBuildings;
   std::vector<Missile*> _playerMissiles;
   int32_t _activeIronCurtainLifetime;
   bool _ironCurtainAvailable;

public:
   Player();
   ~Player();
   void SetPlayerDetails(JsonHelper* jsonPlayer);
   void AddPlayerBuilding(int16_t xyIndex, Building*);
   void AddPlayerMissile(Missile*);
   std::unordered_map<int16_t, Building *> GetPlayerBuildings() const { return _playerBuildings; }
   std::vector<Missile *> GetPlayerMissiles() const { return _playerMissiles; }
   int16_t GetAvailableEnergy() const { return _energy; }
   int16_t GetHealth() const { return _health; }
   int32_t GetActiveIronCurtainLifetime() const { return _activeIronCurtainLifetime; }
   bool GetIronCurtainAvailable() const { return _ironCurtainAvailable; }
};

#endif