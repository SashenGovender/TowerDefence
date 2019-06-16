#ifndef GAMESTATE
#define GAMESTATE
#include <unordered_map>

#include "GameDetails.h"
#include "Player.h"
#include "GameMap.h"
#include <algorithm>//sort

namespace
{
   const char* kPlayerA = "playA";
   const char* kPlayerB = "playB";
}

class GameState
{
private:
   std::string _jsonStringCurrentGameState;
   JsonHelper* _jsonGameState;
   GameDetails* _gameDetails;
   Player* _playerA;
   Player* _playerB;
   std::unordered_map<int16_t, GameCell*> _gameMap;
   int16_t _mapWidth;
   int16_t _mapHeight;

   bool AddMapDetails(int16_t cellIndex, GameCell* mapCell);
   void SetMapDetails();
   void SetPlayerDetails();
   void SetGameDetails();
   void SetIronCurtainHitList();
   void PopulatePlayerMissileAndBuildingVector(GameCell* gameMapCell);

public:
   GameState(std::string jsonGameState);
   ~GameState();
   bool DoesRowHaveAnyBuildings(std::unordered_map<int16_t, Building *> mapOfBuildings, int yRow);
   bool DoesRowHaveBuildingType(std::unordered_map<int16_t, Building *> mapOfBuildings, BuildingType buildType, int yRow);
   bool DoesPlayerHaveBuildingTypeAtPostion(std::unordered_map<int16_t, Building *> mapOfBuildings, int x, int y, BuildingType playerBuildingTypeAtPosition);
   int16_t GetNumberBuildingsInRow(std::unordered_map<int16_t, Building *> mapOfBuildings, BuildingType buildType, int yRow);
   int GetNumberBuildings(std::unordered_map<int16_t, Building *> mapOfBuildings, BuildingType type);
   std::vector<Building*> GetBuildingList(std::unordered_map<int16_t, Building *>listOfPlayerBuildings, BuildingType typeToSearchFor);
   std::vector<Building*> GetBuildingListRow(std::unordered_map<int16_t, Building *>listOfPlayerBuildings, BuildingType typeToSearchFor, int row);
   void GetRowWithHighestNumberAttackBuildings(std::unordered_map<int16_t, Building *>listOfPlayerBuildings, int &highestMaxAttackCount, int &rowWithMaxAttackBuildings);
   bool DoIHaveMinimumEnergy(BuildingType buildingType);
   int16_t  ConvertXY_To_Key(int16_t x, int16_t y);
   std::tuple<int16_t, int16_t> ConvertKey_To_XY(int16_t key);
   void PopulateGameState();
   Player* GetPlayerB() const { return _playerB; };
   Player* GetPlayerA() const { return _playerA; };
   std::unordered_map<int16_t, GameCell *> GetGameMap() const { return _gameMap; }
   GameDetails* GetGameDetails() const { return _gameDetails; }
   std::vector<std::pair<int, int>> GetNumberAttackBuildingsInRow(std::unordered_map<int16_t, Building *> _playerBBuildings);

};

#endif