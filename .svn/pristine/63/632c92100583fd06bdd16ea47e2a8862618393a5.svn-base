#include "GameDetails.h"

GameDetails::GameDetails()
{
   _round = 0;
   _mapWidth = 0;
   _mapHeight = 0;
   _roundIncomeEnergy = 0;
   _defenceBuilding = new Building();
   _energyBuilding = new Building();
   _attackBuilding = new Building();
   _teslaBuilding = new Building();
   _ironCurtain = new IronCurtain();
}

GameDetails::~GameDetails()
{
   if (_defenceBuilding != nullptr)
   {
      delete _defenceBuilding;
      _defenceBuilding = nullptr;
   }

   if (_energyBuilding != nullptr)
   {
      delete _energyBuilding;
      _energyBuilding = nullptr;
   }

   if (_attackBuilding != nullptr)
   {
      delete _attackBuilding;
      _attackBuilding = nullptr;
   }

   if (_teslaBuilding != nullptr)
   {
      delete _teslaBuilding;
      _teslaBuilding = nullptr;
   }

   if (_ironCurtain != nullptr)
   {
      delete _ironCurtain;
      _ironCurtain = nullptr;
   }
}

void GameDetails::SetGameDetails(JsonHelper* gameDetails)
{
   _round = gameDetails->GetInt("round");
   _mapWidth = gameDetails->GetInt("mapWidth");
   _mapHeight = gameDetails->GetInt("mapHeight");
   _roundIncomeEnergy = gameDetails->GetInt("roundIncomeEnergy");
   _maxRounds = gameDetails->GetInt("maxRounds");

   JsonHelper* buildingStats = gameDetails->GetJsonObject("buildingsStats");

   JsonHelper* defence = buildingStats->GetJsonObject("DEFENSE");
   _defenceBuilding->SetBuildingDetails(defence);
   _defenceBuilding->SetBuildingType(BuildingType::Defence);
   defence->Release();

   JsonHelper* energy = buildingStats->GetJsonObject("ENERGY");
   _energyBuilding->SetBuildingDetails(energy);
   _energyBuilding->SetBuildingType(BuildingType::Energy);
   energy->Release();

   JsonHelper* attack = buildingStats->GetJsonObject("ATTACK");
   _attackBuilding->SetBuildingDetails(attack);
   _attackBuilding->SetBuildingType(BuildingType::Attack);
   attack->Release();

   JsonHelper* tesla = buildingStats->GetJsonObject("TESLA");
   _teslaBuilding->SetBuildingDetails(tesla);
   _teslaBuilding->SetBuildingType(BuildingType::Tesla);
   tesla->Release();

   buildingStats->Release();

   JsonHelper* ironCurtainJson = gameDetails->GetJsonObject("ironCurtainStats");
   _ironCurtain->SetIronCurtainDetails(ironCurtainJson);
   ironCurtainJson->Release();
}

