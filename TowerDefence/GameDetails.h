#ifndef GAMEDETAILS
#define GAMEDETAILS
#include "Building.h"

class GameDetails
{
private:
   int16_t _round;
   int16_t _mapWidth;
   int16_t _mapHeight;
   int16_t _roundIncomeEnergy;
   int16_t _maxRounds;

   Building* _defenceBuilding;
   Building* _attackBuilding;
   Building* _energyBuilding;
   Building* _teslaBuilding;
   IronCurtain* _ironCurtain;

public:
   GameDetails();
   ~GameDetails();
   void SetGameDetails(JsonHelper* gameDetails);
   int16_t GetMapWidth() const { return _mapWidth; }
   int16_t GetMapHeight() const { return _mapHeight; }
   Building* GetAttackBuilding() const { return _attackBuilding; }
   Building* GetDefenceBuilding() const { return _defenceBuilding; }
   Building* GetEnergyBuilding() const { return _energyBuilding; }
   Building* GetTeslaBuilding() const { return _teslaBuilding; }
   IronCurtain* GetIronCurtain() const { return _ironCurtain; }
   int16_t GetRound() const { return _round; }
   int16_t GetRoundIncomeEnergy() const { return _roundIncomeEnergy; }

};

#endif