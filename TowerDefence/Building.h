#ifndef BUILDING
#define BUILDING
#include "JsonHelper.h"

enum class BuildingType
{
   Defence = 0,
   Attack = 1,
   Energy = 2,
   Deconstruct = 3,
   Tesla = 4,
   IronCurtain = 5,
   AllBuildings,
   DoNothing,
};

class Building
{
private:
   int16_t _health;
   int16_t _constructionTimeLeft;
   int16_t _constructionTime;
   int16_t _price;
   int16_t _weaponDamage;
   int16_t _weaponSpeed;
   int16_t _weaponCooldownTimeLeft;
   int16_t _weaponCooldownPeriod;
   int16_t _destroyMultiplier;
   int16_t _constructionScore;
   int16_t _energyGeneratedPerTurn;
   BuildingType _buildingType;
   int16_t _x;
   int16_t _y;
   std::string _playerType;

public:
   Building();
   ~Building();
   void SetBuildingDetails(JsonHelper* building);
   std::string GetPlayerType() const { return _playerType; }
   BuildingType GetBuildingType() const { return _buildingType; }
   void SetBuildingType(BuildingType val) { _buildingType = val; }
   int16_t GetWeaponCooldownTimeLeft() const { return _weaponCooldownTimeLeft; }
   int16_t GetConstructionTimeLeft() const { return _constructionTimeLeft; }
   int16_t GetConstructionTime() const { return _constructionTime; }
   int16_t GetPrice() const { return _price; }
   int16_t GetX() const { return _x; }
   int16_t GetY() const { return _y; }
   int16_t GetWeaponSpeed() const { return _weaponSpeed; }
   int16_t GetHealth() const { return _health; }
   int16_t GetWeaponDamage() const { return _weaponDamage; }
   int16_t GetWeaponCooldownPeriod() const { return _weaponCooldownPeriod; }
   int16_t GetEnergyGeneratedPerTurn() const { return _energyGeneratedPerTurn; }
   
};


class IronCurtain
{
private:
   int32_t _activeRounds;
   int32_t _resetPeriod;
   int32_t _price;
   int32_t _constructionScore;

public:
   IronCurtain();

   ~IronCurtain() {};
   void SetIronCurtainDetails(JsonHelper* building);
   int32_t GetActiveRounds() const { return _activeRounds; }
   int32_t GetResetPeriod() const { return _resetPeriod; }
   int32_t GetPrice() const { return _price; }
   int32_t GetConstructionScore() const { return _constructionScore; }
};

#endif