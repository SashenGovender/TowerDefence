#ifndef GAMESTRATEGY
#define GAMESTRATEGY
#include <iostream>
#include "FileHelper.h"
#include "GameState.h"
#include "AttackDetails.h"
#include <math.h> 

class GameStrategy
{
private:
   int16_t _xBuildPosition;
   int16_t _yBuildPosition;
   int16_t _buildingType;
   GameState* _gameState;
   bool _flowToOtherBuildLogic; //allow other processing logic
   int _energyBuildingsPlacedSoFar;

   //For Quick Reference we cache the data
   int16_t _mapWidth;
   int16_t _mapHeight;
   std::unordered_map<int16_t, Building *> _playerABuildings;
   std::unordered_map<int16_t, Building *> _playerBBuildings;


   void SetCommand(int16_t x, int16_t y, BuildingType buildType);
   bool IsDefencePosition(int x);
   bool IsAttackPosition(int x);
   bool IsEnergyPosition(int x);
   AttackDetails* CanAttackPosition(int16_t x, int16_t y, bool buildFront=true);
   int CalculateTimetoAttackPosition(int buildX, int16_t xPosToAttack, int16_t yPosToAttack);
   int TimeTillMissileImpactPlayerABuilding(int xLoc, int yLoc);
   bool PlayForPoints();
   int CalculateDirectDamageToPlayer(std::unordered_map<int16_t, Building*> playerBuildings, std::vector<Missile*>onComingMissiles, std::string damageToPlayer);
   bool CanBuildAtPosition(int16_t x, int16_t y);
   bool AttackPlayerBWithSupportingAttackBuilding();
   bool AttackPlayerBAttackBuildings();
   bool AttackPlayerBEnergyBuilding();
   bool PlaceEnergyBuilding();
   std::vector<AttackDetails> FindBestEnergyPlacements();
   AttackDetails* FindBestAttackOnEnergyBuilding();
   AttackDetails* FindBestAttackOnAttackBuilding();
   AttackDetails* FindBestAttackFromMultipleAttackBuilding(int rowWithMaxAttackBuildings);
   AttackDetails* FindBestAttackOnSingleAttackBuilding(std::vector<Building*> playerBAttackBuildings);
   AttackDetails* BuildAttackBehindDefense(int rowWithMaxAttackBuildings);
   AttackDetails* BuildDefenceInRow(int rowWithMaxAttackBuildings);
   AttackDetails* EvaluateBestAttack(AttackDetails* tempAttack, AttackDetails* bestAttack);
   AttackDetails* ConstructSupportingAttackBuilding();

 
public:
   GameStrategy(GameState* gameState);
   ~GameStrategy();
   bool ExecuteStrategyV2();
   void ExecuteCommand(std::string filePath);
   void SaveGameData(std::string filePath);
};


#endif