#include "GameMap.h"

GameCell::GameCell()
{
   _x = 0;
   _y = 0;
   _building = nullptr;
   _cellOwner = "";

   _missiles.reserve(4);//not more than 4 missile in one position right?
}

GameCell::~GameCell()
{
   if (_building != nullptr)
   {
      delete _building;
      _building = nullptr;
   }

   for (int index = 0; index < static_cast<int>(_missiles.size()); index++)
   {
      Missile* missile = _missiles.at(index);
      if (missile)
      {
         delete missile;
      }
   }
}

void GameCell::SetGameCellDetails(JsonHelper* mapCell)
{
   _x = mapCell->GetInt("x");
   _y = mapCell->GetInt("y");

   //Buildings
   SetBuildingsDetails(mapCell);

   //missiles
   SetMissilesDetails(mapCell);

   //cellOwner
   _cellOwner = mapCell->GetString("cellOwner");
}

void GameCell::SetMissilesDetails(JsonHelper* mapCell)
{
   JsonHelper* jsonMissiles = mapCell->GetArray("missiles");
   if (jsonMissiles)
   {
      int16_t missileIndex = 0;
      JsonHelper* jsonMissile = jsonMissiles->GetJsonObject(missileIndex);//get a single missile
      while (jsonMissile != nullptr)
      {
         Missile* missile = new Missile();
         missile->SetMissileDetails(jsonMissile);
         _missiles.push_back(missile);

         jsonMissile->Release();
         missileIndex++;
         jsonMissile = jsonMissiles->GetJsonObject(missileIndex);//get the next missile
      }
      jsonMissiles->Release();
   }
}

void GameCell::SetBuildingsDetails(JsonHelper* mapCell)
{
   JsonHelper* jsonBuildings = mapCell->GetArray("buildings");
   if (jsonBuildings)
   {
      JsonHelper* jsonBuilding = jsonBuildings->GetJsonObject(0);
      if (jsonBuilding)
      {
         _building = new Building();
         _building->SetBuildingDetails(jsonBuilding);
         jsonBuilding->Release();
      }
      jsonBuildings->Release();
   }
}
