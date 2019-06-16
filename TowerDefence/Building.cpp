#include "Building.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Building::Building()
{
   _health = 0;
   _constructionTimeLeft = 0;
   _price = 0;
   _weaponDamage = 0;
   _weaponSpeed = 0;
   _weaponCooldownTimeLeft = 0;
   _weaponCooldownPeriod = 0;
   _destroyMultiplier = 0;
   _constructionScore = 0;
   _energyGeneratedPerTurn = 0;
   _buildingType = BuildingType::Defence;
   _x = 0;
   _y = 0;
   _playerType = "";
   _constructionTime = 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Building::~Building()
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Building::SetBuildingDetails(JsonHelper* building)
{
   _health = building->GetInt("health");
   _price = building->GetInt("price");
   _weaponDamage = building->GetInt("weaponDamage");
   _weaponSpeed = building->GetInt("weaponSpeed");
   _weaponCooldownTimeLeft = building->GetInt("weaponCooldownTimeLeft");
   _weaponCooldownPeriod = building->GetInt("weaponCooldownPeriod");
   _destroyMultiplier = building->GetInt("destroyMultiplier");
   _constructionScore = building->GetInt("constructionScore");
   _energyGeneratedPerTurn = building->GetInt("energyGeneratedPerTurn");


   std::string buildingType = building->GetString("buildingType");
   if (buildingType.compare("ATTACK") == 0)
   {
      _buildingType = BuildingType::Attack;
   }
   else if (buildingType.compare("DEFENSE") == 0)
   {
      _buildingType = BuildingType::Defence;
   }
   else if (buildingType.compare("ENERGY") == 0)
   {
      _buildingType = BuildingType::Energy;
   }
   else if (buildingType.compare("TESLA") == 0)
   {
      _buildingType = BuildingType::Attack;// lets set a tesla building to attack, i dont want to implement the tesla
   }

   _x = building->GetInt("x");
   _y = building->GetInt("y");
   _playerType = building->GetString("playerType");

   _constructionTimeLeft = building->GetInt("constructionTimeLeft");
   _constructionTime = building->GetInt("constructionTime");

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

IronCurtain::IronCurtain()
{
   _activeRounds = 0;
   _resetPeriod = 0;
   _price = 0;
   _constructionScore = 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void IronCurtain::SetIronCurtainDetails(JsonHelper* curtain)
{
   _activeRounds = curtain->GetInt("activeRounds");
   _resetPeriod = curtain->GetInt("resetPeriod");
   _price = curtain->GetInt("price");
   _constructionScore = curtain->GetInt("constructionScore");

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
