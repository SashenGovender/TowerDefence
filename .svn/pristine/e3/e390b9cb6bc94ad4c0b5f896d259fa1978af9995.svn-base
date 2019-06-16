#include "GameState.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool CompareRowAttacks(const std::pair<int, int> &lhs, const std::pair<int, int>  &rhs)
{
   return (lhs.second > rhs.second);//sort by descending 5,4,3,2,1
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- -
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- -

GameState::GameState(std::string jsonStringGameState)
{
   _jsonStringCurrentGameState = jsonStringGameState;

   _jsonGameState = new JsonHelper();
   const char* cstringCurrentGameState = jsonStringGameState.c_str();
   _jsonGameState->Deserialise(cstringCurrentGameState);

   _gameDetails = new GameDetails();
   _playerA = new Player();
   _playerB = new Player();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

GameState::~GameState()
{
   if (_gameDetails != nullptr)
   {
      delete _gameDetails;
      _gameDetails = nullptr;
   }
   if (_jsonGameState != nullptr)
   {
      delete _jsonGameState;
      _jsonGameState = nullptr;
   }
   if (_playerA != nullptr)
   {
      delete _playerA;
      _playerA = nullptr;
   }
   if (_playerB != nullptr)
   {
      delete _playerB;
      _playerB = nullptr;
   }

   for (std::unordered_map<int16_t, GameCell*> ::const_iterator it = _gameMap.begin(); it != _gameMap.end(); ++it)
   {
      GameCell* mapCell = it->second;
      if (mapCell)
      {
         delete mapCell;
         mapCell = nullptr;
      }
   }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GameState::PopulateGameState()
{
   //Game Details
   SetGameDetails();
   _mapWidth = _gameDetails->GetMapWidth();
   _mapHeight = _gameDetails->GetMapHeight();

   //Players
   SetPlayerDetails();

   //Game Map
   int res = (_mapWidth*_gameDetails->GetMapHeight());
   _gameMap.reserve(res);
   SetMapDetails();

   //set IronCurtain HitList
   SetIronCurtainHitList();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

std::vector<std::pair<int, int>> GameState::GetNumberAttackBuildingsInRow(std::unordered_map<int16_t, Building *> listOfPlayerBuildings)
{
   std::vector<std::pair<int, int>> attacksInRow;
   attacksInRow.reserve(8);

   for (int yRow = 0; yRow < _mapHeight; yRow++)
   {
      int numAttackBuild = GetNumberBuildingsInRow(listOfPlayerBuildings, BuildingType::Attack, yRow);
      attacksInRow.push_back(std::make_pair(yRow, numAttackBuild));
   }

   if (attacksInRow.size() >= 2)
   {
      std::sort(attacksInRow.begin(), attacksInRow.end(), CompareRowAttacks);
   }

   return attacksInRow;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameState::AddMapDetails(int16_t cellIndex, GameCell* mapCell)
{
   std::unordered_map<int16_t, GameCell*> ::const_iterator it = _gameMap.find(cellIndex);
   if (it == _gameMap.end())
   {
      _gameMap.insert(std::make_pair(cellIndex, mapCell));
      return true;
   }

   return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GameState::SetMapDetails()
{
   //int16_t mapWidth = _gameDetails->GetMapWidth();
   JsonHelper* gameMap = _jsonGameState->GetArray("gameMap");
   int16_t mapRow = 0;
   JsonHelper* gameMapRow = gameMap->GetArray(mapRow); // map row

   while (gameMapRow != nullptr)
   {
      int16_t gameMapRowCell = 0;
      JsonHelper* jsonGameMapRowCell = gameMapRow->GetJsonObject(gameMapRowCell);//map cell location
      while (jsonGameMapRowCell != nullptr)
      {
         GameCell* gameMapCell = new GameCell();
         gameMapCell->SetGameCellDetails(jsonGameMapRowCell);
         //int cell_XY_Key = (mapRow)*(mapWidth)+gameMapRowCell;
         int16_t  cell_XY_Key = ConvertXY_To_Key(gameMapCell->GetX(), gameMapCell->GetY());
         if (AddMapDetails(cell_XY_Key, gameMapCell) == false)// Is the cell a duplicate
         {
            delete gameMapCell;
         }
         else
         {
            PopulatePlayerMissileAndBuildingVector(gameMapCell);
         }

         //Get the next cell in the row
         jsonGameMapRowCell->Release();
         gameMapRowCell++;
         jsonGameMapRowCell = gameMapRow->GetJsonObject(gameMapRowCell);//map row cell location
      }

      //Get the next row in the map
      gameMapRowCell = 0;
      gameMapRow->Release();
      mapRow++;
      gameMapRow = gameMap->GetArray(mapRow); // map row
   }

   gameMap->Release();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GameState::SetPlayerDetails()
{
   JsonHelper* players = _jsonGameState->GetArray("players");

   JsonHelper* jsonPlayer1 = players->GetJsonObject(0);//player1
   std::string player1 = jsonPlayer1->GetString("playerType");
   JsonHelper* jsonPlayer2 = players->GetJsonObject(1);//player2

   if (player1.compare("A") == 0)
   {
      _playerA->SetPlayerDetails(jsonPlayer1);
      _playerB->SetPlayerDetails(jsonPlayer2);
   }
   else
   {
      _playerA->SetPlayerDetails(jsonPlayer2);
      _playerB->SetPlayerDetails(jsonPlayer1);
   }
   jsonPlayer1->Release();
   jsonPlayer2->Release();

   players->Release();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GameState::SetGameDetails()
{
   JsonHelper* gameDetails = _jsonGameState->GetJsonObject("gameDetails");
   _gameDetails->SetGameDetails(gameDetails);
   gameDetails->Release();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GameState::SetIronCurtainHitList()
{
   JsonHelper* ironCurtainHitListJson = _jsonGameState->GetArray("ironcurtainHitList");
   ironCurtainHitListJson->Release();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GameState::PopulatePlayerMissileAndBuildingVector(GameCell* gameMapCell)
{
   std::vector<Missile*> missiles;
   missiles = gameMapCell->GetMissiles();

   for (int index = 0; index < static_cast<int>(missiles.size()); index++)
   {
      Missile* missile = missiles.at(index);
      if (missile->GetPlayerType() == "A")
      {
         _playerA->AddPlayerMissile(missile);//remember there can be multiple missiles in a cell
      }
      else
      {
         _playerB->AddPlayerMissile(missile);
      }
   }

   Building* building = gameMapCell->GetBuilding();
   if (building)
   {
      int16_t XY_Key = ConvertXY_To_Key(building->GetX(), building->GetY());
      if (building->GetPlayerType() == "A")
      {
         _playerA->AddPlayerBuilding(XY_Key, building);
      }
      else
      {
         _playerB->AddPlayerBuilding(XY_Key, building);
      }
   }
}

int16_t  GameState::ConvertXY_To_Key(int16_t x, int16_t y)
{
   return x + y*(_mapWidth);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

std::tuple<int16_t, int16_t > GameState::ConvertKey_To_XY(int16_t key)
{
   int16_t x, y;
   y = key / _mapWidth;
   x = key - y*_mapWidth;

   return std::tuple<int16_t, int16_t>(x, y);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameState::DoesRowHaveAnyBuildings(std::unordered_map<int16_t, Building *> mapOfBuildings, int yRow)
{
   for (std::unordered_map<int16_t, Building*> ::const_iterator it = mapOfBuildings.begin(); it != mapOfBuildings.end(); ++it)
   {
      Building* building = it->second;
      if (building->GetY() == yRow)
      {
         return true;
      }
   }
   return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameState::DoesRowHaveBuildingType(std::unordered_map<int16_t, Building *> mapOfBuildings, BuildingType buildType, int yRow)
{
   for (std::unordered_map<int16_t, Building*> ::const_iterator it = mapOfBuildings.begin(); it != mapOfBuildings.end(); ++it)
   {
      Building* building = it->second;
      if (building->GetY() == yRow && building->GetBuildingType() == buildType)
      {
         return true;
      }
   }
   return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameState::DoesPlayerHaveBuildingTypeAtPostion(std::unordered_map<int16_t, Building *> mapOfBuildings, int x, int y, BuildingType playerBuildingTypeAtPosition)
{
   int key = ConvertXY_To_Key(x, y);
   std::unordered_map<int16_t, Building*> ::const_iterator it = mapOfBuildings.find(key);
   if (it != mapOfBuildings.end())
   {
      // we found a building
      Building* build = it->second;
      if (build->GetBuildingType() == playerBuildingTypeAtPosition)
      {
         return true;
      }
   }
   return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int16_t GameState::GetNumberBuildingsInRow(std::unordered_map<int16_t, Building *> mapOfBuildings, BuildingType buildType, int yRow)
{
   int count = 0;
   for (std::unordered_map<int16_t, Building*> ::const_iterator it = mapOfBuildings.begin(); it != mapOfBuildings.end(); ++it)
   {
      Building* building = it->second;
      if (building->GetY() == yRow)
      {
         if (buildType == BuildingType::AllBuildings)
         {
            count++;
         }
         else if (building->GetBuildingType() == buildType)
         {
            count++;
         }
      }
   }
   return count;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int GameState::GetNumberBuildings(std::unordered_map<int16_t, Building *> mapOfBuildings, BuildingType type)
{
   int count = 0;
   for (std::unordered_map<int16_t, Building*> ::const_iterator it = mapOfBuildings.begin(); it != mapOfBuildings.end(); ++it)
   {
      Building* build = it->second;
      if (build->GetBuildingType() == type)
      {
         count++;
      }
   }
   return count;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

std::vector<Building*> GameState::GetBuildingList(std::unordered_map<int16_t, Building *>listOfPlayerBuildings, BuildingType typeToSearchFor)
{
   std::vector<Building*> filteredBuildingList;
   filteredBuildingList.reserve(20);

   for (std::unordered_map<int16_t, Building*> ::const_iterator it = listOfPlayerBuildings.begin(); it != listOfPlayerBuildings.end(); ++it)
   {
      Building* build = it->second;
      if (build->GetBuildingType() == typeToSearchFor)
      {
         filteredBuildingList.push_back(build);
      }
   }

   return filteredBuildingList;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

std::vector<Building*> GameState::GetBuildingListRow(std::unordered_map<int16_t, Building *>listOfPlayerBuildings, BuildingType typeToSearchFor, int row)
{
   std::vector<Building*> filteredBuildingList;
   filteredBuildingList.reserve(8);

   for (std::unordered_map<int16_t, Building*> ::const_iterator it = listOfPlayerBuildings.begin(); it != listOfPlayerBuildings.end(); ++it)
   {
      Building* build = it->second;
      if (build->GetBuildingType() == typeToSearchFor && build->GetY() == row)
      {
         filteredBuildingList.push_back(build);
      }
   }

   return filteredBuildingList;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GameState::GetRowWithHighestNumberAttackBuildings(std::unordered_map<int16_t, Building *>listOfPlayerBuildings,int &highestMaxAttackCount, int &rowWithMaxAttackBuildings)
{
   for (int y = 0; y < _mapHeight; y++)
   {
      int numAttackBuild = GetNumberBuildingsInRow(listOfPlayerBuildings, BuildingType::Attack, y);
      if (numAttackBuild > highestMaxAttackCount)
      {
         highestMaxAttackCount = numAttackBuild;
         rowWithMaxAttackBuildings = y;
      }
   }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameState::DoIHaveMinimumEnergy(BuildingType buildingType)
{
   int16_t energyAvailable = GetPlayerA()->GetAvailableEnergy();

   switch (buildingType)
   {
   case BuildingType::Energy:
   {
      int16_t energyBuildPrice = GetGameDetails()->GetEnergyBuilding()->GetPrice();
      if (energyAvailable >= energyBuildPrice)
      {
         return true;
      }
   }
   break;

   case BuildingType::Attack:
   {
      int16_t attackBuildPrice = GetGameDetails()->GetAttackBuilding()->GetPrice();
      if (energyAvailable >= attackBuildPrice)
      {
         return true;
      }
   }
   break;

   case BuildingType::Defence:
   {
      int16_t defenceBuildPrice = GetGameDetails()->GetDefenceBuilding()->GetPrice();
      if (energyAvailable >= defenceBuildPrice)
      {
         return true;
      }
   }
   break;

   case BuildingType::Tesla:
   {
      int16_t teslaBuildPrice = GetGameDetails()->GetTeslaBuilding()->GetPrice();
      if (energyAvailable >= teslaBuildPrice)
      {
         return true;
      }
   }
   break;
   }
   return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
