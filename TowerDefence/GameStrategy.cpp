#include "GameStrategy.h"
#include <string>
#include <time.h>

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool CompareCoolDown(const Building *lhs, const Building *rhs)
{
   return (lhs->GetWeaponCooldownTimeLeft() > rhs->GetWeaponCooldownTimeLeft());//sort by descending 5,4,3,2,1
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- -
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- -

GameStrategy::GameStrategy(GameState* gameState)
{
   _xBuildPosition = 0;
   _yBuildPosition = 0;
   _buildingType = 0;
   _gameState = gameState;
   _flowToOtherBuildLogic = false;
   _energyBuildingsPlacedSoFar = 0;

   //caching map data
   _mapWidth = _gameState->GetGameDetails()->GetMapWidth();
   _mapHeight = _gameState->GetGameDetails()->GetMapHeight();
   _playerABuildings = _gameState->GetPlayerA()->GetPlayerBuildings();
   _playerBBuildings = _gameState->GetPlayerB()->GetPlayerBuildings();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

GameStrategy::~GameStrategy()
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GameStrategy::SetCommand(int16_t x, int16_t y, BuildingType buildType)
{
   _xBuildPosition = x;
   _yBuildPosition = y;

   if (buildType != BuildingType::DoNothing)
   {
      //should we play for points?
      bool playforPoints = PlayForPoints();
      if (playforPoints == true)
      {
         //it seems score is based on damage done to player b, constructing buildings, energy generated, and damage done to opponent buildings. so lets build energy? 
         for (int xPos = 0; xPos < (_mapWidth / 2); xPos++)
         {
            for (int yPos = 0; yPos < _mapHeight; yPos++)
            {
               if (CanBuildAtPosition(xPos, yPos))
               {
                  _xBuildPosition = xPos;
                  _yBuildPosition = yPos;
                  xPos = _mapWidth;//end the loop
                  break;
               }
            }
         }
              
         buildType = BuildingType::Energy;
         std::cout << "we need to play for points, DoNothing" << std::endl;
      }
   }

   _buildingType = static_cast<int16_t>(buildType);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GameStrategy::ExecuteCommand(std::string filePath)
{
   //Need To check energy cost
   std::string command = std::to_string(_xBuildPosition);
   command.append(",");
   command.append(std::to_string(_yBuildPosition));
   command.append(",");
   command.append(std::to_string(_buildingType));

   if (_buildingType == static_cast<int16_t>(BuildingType::DoNothing))
   {
      // empty command
      command = "";
   }
   FileHelper helper;
   helper.WriteInformation(filePath, command);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameStrategy::IsDefencePosition(int x)
{
   switch (x)
   {
   case 7: return true;
      break;
   }

   return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameStrategy::IsAttackPosition(int x)
{
   switch (x)
   {
   case 1: return true;
      break;
   case 2: return true;
      break;
   case 3: return true;
      break;
   case 4: return true;
      break;
   case 5: return true;
      break;
   case 6: return true;
      break;
   case 7: return true;
      break;
   }

   return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameStrategy::IsEnergyPosition(int x)
{
   switch (x)
   {
   case 0: return true;
      break;
   }
   return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
1 Energy Building   - 8
2 Energy Building   - 11
3 Energy Building   - 14
4 Energy Building   - 17
5 Energy Building   - 20
6 Energy Building   - 23
7 Energy Building   - 26
8 Energy Building   - 29
9 Energy Building   - 32
10 Energy Building  - 35
11 Energy Building  - 38
12 Energy Building  - 41
13 Energy Building  - 44
14 Energy Building  - 47
15 Energy Building  - 50
16 Energy Building  - 53
*/
bool GameStrategy::ExecuteStrategyV2()
{
   FileHelper helper;
   
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //int energyBuildingsRequiredToBuild = 10;
   //std::string energyBuildingsSoFar = helper.ReadInformation("previousGameState.txt");
   //if (energyBuildingsSoFar != "" && energyBuildingsSoFar.length() > 0)//Did we read any data
   //{
   //   _energyBuildingsPlacedSoFar = atoi(energyBuildingsSoFar.c_str());
   //}
   //if ((_energyBuildingsPlacedSoFar < energyBuildingsRequiredToBuild))
   //{
   //   bool havePlacedEnergy = PlaceEnergyBuilding();
   //   if (havePlacedEnergy == true)
   //   {
   //      return true;
   //   }
   //}
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   int energyRequiredPerTurn = 20;
   
   std::string energyGeneratePerTurn = helper.ReadInformation("overrideEnergy.txt");
   if (energyGeneratePerTurn != "" && energyGeneratePerTurn.length() > 0)//Did we read any data
   {
      energyRequiredPerTurn = atoi(energyGeneratePerTurn.c_str());
   }
   std::cout << "Energy Required Per Turn: " << energyRequiredPerTurn << std::endl;

   int energyPerTurn = _gameState->GetGameDetails()->GetRoundIncomeEnergy() + _gameState->GetNumberBuildings(_playerABuildings, BuildingType::Energy)*_gameState->GetGameDetails()->GetEnergyBuilding()->GetEnergyGeneratedPerTurn();
   int currentEnergy = _gameState->GetPlayerA()->GetAvailableEnergy();// have we reached a state where we have piles of energy but not enough energy buildings?
   int playerAHealth = _gameState->GetPlayerA()->GetHealth();
   if ((energyPerTurn < energyRequiredPerTurn) && (currentEnergy < 50) && (playerAHealth >= 60))
   {
      bool havePlacedEnergy = PlaceEnergyBuilding();
      if (havePlacedEnergy == true)
      {
         return true;
      }
   }
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   
   //int currentEnergy = _gameState->GetPlayerA()->GetAvailableEnergy();
   bool ironCurtainAvailable = _gameState->GetPlayerA()->GetIronCurtainAvailable();
   int energyRequiredForCurtain = _gameState->GetGameDetails()->GetIronCurtain()->GetPrice();

   if ((currentEnergy >= energyRequiredForCurtain) && (ironCurtainAvailable == true))
   {
      SetCommand(0, 0, BuildingType::IronCurtain);
      return true;
   }

   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   int playerB_EnergyBuild = _gameState->GetNumberBuildings(_playerBBuildings, BuildingType::Energy);
   if (playerB_EnergyBuild > 0)
   {
      bool haveAttacked = AttackPlayerBEnergyBuilding();
      if (haveAttacked == true)
      {
         return true;
      }
   }
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   int playerB_AttackBuild = _gameState->GetNumberBuildings(_playerBBuildings, BuildingType::Attack);
   if (playerB_AttackBuild > 0)
   {
      bool shouldAttackOrPreseveEnergy = AttackPlayerBAttackBuildings();
      if (shouldAttackOrPreseveEnergy == true)
      {
         return true;
      }
   }
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   int availableEnergy = _gameState->GetPlayerA()->GetAvailableEnergy();
   if ((availableEnergy + energyPerTurn) >= 50)
   {
      bool shouldPlaceEnergy = PlaceEnergyBuilding();
      if (shouldPlaceEnergy == true)
      {
         return true;
      }
   }
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   bool constructSupportAttack = AttackPlayerBWithSupportingAttackBuilding();
   if (constructSupportAttack == true)
   {
      return true;
   }
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   std::cout << "DoNothing End in ExecuteStrategyV2" << std::endl;
   SetCommand(0, 0, BuildingType::DoNothing);
   return true;
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameStrategy::AttackPlayerBWithSupportingAttackBuilding()
{
   //If we get here then we only have 1 attack building and cannot construct a defence because the attacking building is right at the front .ie at position x=7. Then we construct an attack to support it.
   AttackDetails* bestSingleSupportAttack = nullptr;
   bestSingleSupportAttack = ConstructSupportingAttackBuilding();
   if (bestSingleSupportAttack != nullptr)
   {
      if (_gameState->DoIHaveMinimumEnergy(BuildingType::Attack) == true)
      {
         std::cout << " BuildingType::Attack in ExecuteStrategyV2 in AttackPlayerBWithSupportingAttackBuilding" << std::endl;
         SetCommand(bestSingleSupportAttack->GetAttackFromX(), bestSingleSupportAttack->GetAttackFromY(), BuildingType::Attack);
         delete bestSingleSupportAttack;
         return true;
      }
      else
      {
         std::cout << " Not Enough Energy -  BuildingType::Attack in ExecuteStrategyV2 in AttackPlayerBWithSupportingAttackBuilding" << std::endl;
         SetCommand(0, 0, BuildingType::DoNothing);
         delete bestSingleSupportAttack;
         return true;
      }
   }
   return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

AttackDetails* GameStrategy::ConstructSupportingAttackBuilding()
{
   AttackDetails* bestAttack = nullptr;

   //***************************************************************************************************************************************************************************************************************************
   //Lets place an attack building behind my attack building that is attacking player b attack building
   std::vector<Building*> playerBAttackBuildings;
   playerBAttackBuildings = _gameState->GetBuildingList(_playerBBuildings, BuildingType::Attack);
   for (int index = 0; index < static_cast<int>(playerBAttackBuildings.size()); index++)
   {
      Building* buildingToAttack = playerBAttackBuildings.at(index);
      if (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == true)
      {
         AttackDetails* tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY());
         bestAttack = EvaluateBestAttack(tempAttack, bestAttack);
      }
   }
   if (bestAttack != nullptr)
   {
      bestAttack->SetBuildType(BuildingType::Attack);
      return bestAttack;
   }

   //***************************************************************************************************************************************************************************************************************************
   //Lets place an attack building behind my attack building
   std::vector<Building*> playerAAttackBuildings;
   playerAAttackBuildings = _gameState->GetBuildingList(_playerABuildings, BuildingType::Attack);
   for (int index = 0; index < static_cast<int>(playerAAttackBuildings.size()); index++)
   {
      Building* buildingToAttack = playerAAttackBuildings.at(index);
      if (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == true)
      {
         AttackDetails* tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY());
         bestAttack = EvaluateBestAttack(tempAttack, bestAttack);
      }
   }
   if (bestAttack != nullptr)
   {
      bestAttack->SetBuildType(BuildingType::Attack);
      return bestAttack;
   }
   //***************************************************************************************************************************************************************************************************************************
   return bestAttack;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameStrategy::PlaceEnergyBuilding()
{
   std::vector<AttackDetails> bestEnergyPlacements;
   bestEnergyPlacements = FindBestEnergyPlacements();
   if (bestEnergyPlacements.size() >= 1)
   {
      bool enableEnergyPlacementRandom = true;
      FileHelper helper;
      std::string enableEnergyPlacementRandomText = helper.ReadInformation("overrideEnergyPlacement.txt");
      if (enableEnergyPlacementRandomText != "" && enableEnergyPlacementRandomText.length() > 0)//Did we read any data
      {
         enableEnergyPlacementRandom = (bool)atoi(enableEnergyPlacementRandomText.c_str());
      }

      int energyBuildIndex = 0;
      if (enableEnergyPlacementRandom == true)
      {
         int size = bestEnergyPlacements.size();
         srand(static_cast<unsigned int>(time(NULL)));
         energyBuildIndex = (int)std::floor(rand() % size);
      }

      AttackDetails bestEnergyPlacement = bestEnergyPlacements.at(energyBuildIndex);
      if (bestEnergyPlacement.GetTimeTillMissileImpactPlayerA() > 4)
      {
         if (_gameState->DoIHaveMinimumEnergy(BuildingType::Energy) == true)
         {
            std::cout << "BuildingType::Energy in ExecuteStrategyV2 in PlaceEnergyBuilding" << std::endl;
            SetCommand(bestEnergyPlacement.GetAttackFromX(), bestEnergyPlacement.GetAttackFromY(), BuildingType::Energy);
         }
         else
         {
            std::cout << "Not Enough Energy BuildingType::Energy in ExecuteStrategyV2 in PlaceEnergyBuilding" << std::endl;
            SetCommand(0, 0, BuildingType::DoNothing);
         }
         return true;
      }
   }

   //std::cout << "Cannot find good place to construct energy building in ExecuteStrategyV2 in PlaceEnergyBuilding" << std::endl;
   //SetCommand(0, 0, BuildingType::DoNothing);
   return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameStrategy::AttackPlayerBAttackBuildings()
{
   AttackDetails* bestAttackBuildAttack = nullptr;
   bestAttackBuildAttack = FindBestAttackOnAttackBuilding();
   if (bestAttackBuildAttack != nullptr)
   {
      if (_gameState->DoIHaveMinimumEnergy(bestAttackBuildAttack->GetBuildType()) == true)
      {
         std::cout << static_cast<int>(bestAttackBuildAttack->GetBuildType()) << " BuildingType in ExecuteStrategyV2 in AttackPlayerBAttackBuildings" << std::endl;
         SetCommand(bestAttackBuildAttack->GetAttackFromX(), bestAttackBuildAttack->GetAttackFromY(), bestAttackBuildAttack->GetBuildType());
         delete bestAttackBuildAttack;
         return true;
      }
      else
      {
         std::cout << static_cast<int>(bestAttackBuildAttack->GetBuildType()) << " - Not Enough Energy - BuildingType in ExecuteStrategyV2 in AttackPlayerBAttackBuildings" << std::endl;
         SetCommand(0, 0, BuildingType::DoNothing);
         delete bestAttackBuildAttack;
         return true;
      }
   }

   // we have no possible attack on a single player b attack building. Perhaps lets flow to build energy? (single)
   // we cannot construct any more buildings in this row. We should check if we can build else where?     (multiple)  
   if (_flowToOtherBuildLogic == true)
   {
      return false;
   }

   std::cout << "No Attack in ExecuteStrategyV2 in AttackPlayerBAttackBuildings" << std::endl;
   SetCommand(0, 0, BuildingType::DoNothing);
   return true; //remember we might need to save up energy to attack multiple player b buildings
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameStrategy::AttackPlayerBEnergyBuilding()
{
   AttackDetails* bestEnergyBuildAttack = nullptr;
   bestEnergyBuildAttack = FindBestAttackOnEnergyBuilding();
   if (bestEnergyBuildAttack != nullptr)
   {
      if (_gameState->DoIHaveMinimumEnergy(BuildingType::Attack) == true)
      {
         std::cout << " BuildingType::Attack in ExecuteStrategyV2 in AttackPlayerBEnergyBuilding" << std::endl;
         SetCommand(bestEnergyBuildAttack->GetAttackFromX(), bestEnergyBuildAttack->GetAttackFromY(), BuildingType::Attack);
         delete bestEnergyBuildAttack;
         return true;
      }
      else
      {
         std::cout << " Not Enough Energy -  BuildingType::Attack in ExecuteStrategyV2 in AttackPlayerBEnergyBuilding" << std::endl;
         SetCommand(0, 0, BuildingType::DoNothing);
         delete bestEnergyBuildAttack;
         return true;
      }
   }
   return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

AttackDetails* GameStrategy::FindBestAttackOnEnergyBuilding()
{
   // We should attack player b energy generators
   std::vector<Building*> playerBEnergyBuildings;
   playerBEnergyBuildings = _gameState->GetBuildingList(_playerBBuildings, BuildingType::Energy);
   AttackDetails* bestAttack = nullptr;

   //***************************************************************************************************************************************************************************************************************************
   for (int index = 0; index < static_cast<int>(playerBEnergyBuildings.size()); index++)
   {
      Building* buildingToAttack = playerBEnergyBuildings.at(index);
      //Do I have an energy building in this row 
      //Lets not build attack building in rows that already have multiple attack buildings.
      if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Energy, buildingToAttack->GetY()) == true) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == false) &&
          _gameState->GetNumberBuildingsInRow(_playerBBuildings, BuildingType::Attack, buildingToAttack->GetY()) <= 1)
      {
         AttackDetails* tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY());
         bestAttack = EvaluateBestAttack(tempAttack, bestAttack);//store the new best attack
      }
   }
   if (bestAttack != nullptr)
   {
      bestAttack->SetBuildType(BuildingType::Attack);
      return bestAttack;
   }
   //***************************************************************************************************************************************************************************************************************************

   //***************************************************************************************************************************************************************************************************************************
   //Target opponent energy buildings that does not have my energy building in its row
   for (int index = 0; index < static_cast<int>(playerBEnergyBuildings.size()); index++)
   {
      Building* buildingToAttack = playerBEnergyBuildings.at(index);
      if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == false) && (_gameState->GetNumberBuildingsInRow(_playerBBuildings, BuildingType::Attack, buildingToAttack->GetY()) <= 1))
      {
         AttackDetails* tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY());
         bestAttack = EvaluateBestAttack(tempAttack, bestAttack);
      }
   }

   if (bestAttack != nullptr)
   {
      bestAttack->SetBuildType(BuildingType::Attack);
      return bestAttack;
   }
   //***************************************************************************************************************************************************************************************************************************

   return bestAttack;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

std::vector<AttackDetails> GameStrategy::FindBestEnergyPlacements()
{
   int x = 0;
   AttackDetails* bestAttack = nullptr;
  // for (int x = 0; x < _mapWidth/2 ; x++)
   //{
      for (int y = 0; y < _mapHeight; y++)
      {
         if (CanBuildAtPosition(x, y))
         {
            int impactTime = TimeTillMissileImpactPlayerABuilding(x, y);
            if (bestAttack == nullptr)
            {
               bestAttack = new AttackDetails(x, y, 0, impactTime);
            }
            else if (bestAttack->GetTimeTillMissileImpactPlayerA() < impactTime)// we want the longest time here
            {
               bestAttack->SetAttack(x, y, 0, impactTime);// the back most column has an empty slot for an energy building
               bestAttack->SetBuildType(BuildingType::Energy);
            }
         }
      }
  // }
   std::vector<AttackDetails> bestAttacks;
   if (bestAttack == nullptr)
   {
      return bestAttacks;
   }

  // for (int x = 0; x < _mapWidth / 2; x++)
  // {
      for (int y = 0; y < _mapHeight; y++)
      {
         if (CanBuildAtPosition(x, y))
         {
            int impactTime = TimeTillMissileImpactPlayerABuilding(x, y);
            if (bestAttack->GetTimeTillMissileImpactPlayerA() == impactTime)
            {
               AttackDetails attack(x, y, 0, impactTime);
               attack.SetBuildType(BuildingType::Energy);
               bestAttacks.push_back(attack);
            }
         }
      }
  // }

   delete bestAttack;

   return bestAttacks;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

AttackDetails* GameStrategy::FindBestAttackOnAttackBuilding()
{
   AttackDetails* bestAttack = nullptr;
   int rowWithMaxAttackBuildings = 0;
   int rowWithMaxAttackBuildingCount = 0;
   _gameState->GetRowWithHighestNumberAttackBuildings(_playerBBuildings, rowWithMaxAttackBuildingCount, rowWithMaxAttackBuildings);// player b row that has the highest number of attack buildings

   if (rowWithMaxAttackBuildingCount > 1)
   {
      std::vector<std::pair<int, int>> rowsWithAttackCount;
      rowsWithAttackCount = _gameState->GetNumberAttackBuildingsInRow(_playerBBuildings);
      for (int index = 0; index < static_cast<int>(rowsWithAttackCount.size()); index++)
      {
         int numAttackBuildingsInRow = rowsWithAttackCount.at(index).second;
         int row = rowsWithAttackCount.at(index).first;
         if (numAttackBuildingsInRow > 1)
         {
            AttackDetails* attack = FindBestAttackFromMultipleAttackBuilding(row);
            if (attack != nullptr)
            {
               bestAttack = attack;
               break;
            }
         }
      }
      if (bestAttack != nullptr)
      {
         return bestAttack;
      }
      // _flowToOtherBuildLogic = true;
   }

   else
   {
      //return FindBestAttackOnSingleAttackBuilding();
      std::vector<Building*> playerBAttackBuildingsWithCoolDownSort;
      playerBAttackBuildingsWithCoolDownSort = _gameState->GetBuildingList(_playerBBuildings, BuildingType::Attack);

      if (playerBAttackBuildingsWithCoolDownSort.size() >= 2)
      {
         std::sort(playerBAttackBuildingsWithCoolDownSort.begin(), playerBAttackBuildingsWithCoolDownSort.end(), CompareCoolDown);
      }

      bestAttack = FindBestAttackOnSingleAttackBuilding(playerBAttackBuildingsWithCoolDownSort);
   }

   return bestAttack;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
-------------
| A | D | R |
-------------
| 0 | 0 | D  |3
| 0 | 1 | A  |4
| 1 | 0 | D  |1
| 1 | 1 | A  |2
*/
AttackDetails*  GameStrategy::FindBestAttackFromMultipleAttackBuilding(int rowWithMaxAttackBuildings)
{
   AttackDetails* bestAttack = nullptr;

   //***************************************************************************************************************************************************************************************************************************
   //no defence and have attack -> build defence
   if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, rowWithMaxAttackBuildings) == true) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, rowWithMaxAttackBuildings) == false))
   {
      AttackDetails* tempAttack = BuildDefenceInRow(rowWithMaxAttackBuildings);
      if (tempAttack != nullptr)
      {
         bestAttack = new AttackDetails(tempAttack->GetAttackFromX(), tempAttack->GetAttackFromY(), tempAttack->GetTimeTillMissileImpactPlayerB(), tempAttack->GetTimeTillMissileImpactPlayerA());
         bestAttack->SetBuildType(BuildingType::Defence);
         delete tempAttack;
      }
   }
   //***************************************************************************************************************************************************************************************************************************

   //***************************************************************************************************************************************************************************************************************************
   //have attack and  defence -> Attack
   else if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, rowWithMaxAttackBuildings) == true) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, rowWithMaxAttackBuildings) == true))
   {
      AttackDetails* tempAttack = BuildAttackBehindDefense(rowWithMaxAttackBuildings);
      if (tempAttack != nullptr)
      {
         bestAttack = new AttackDetails(tempAttack->GetAttackFromX(), tempAttack->GetAttackFromY(), tempAttack->GetTimeTillMissileImpactPlayerB(), tempAttack->GetTimeTillMissileImpactPlayerA());
         bestAttack->SetBuildType(BuildingType::Attack);
         delete tempAttack;
      }
      else
      {
         // if we return no attack here then that means we cannot construct any more buildings in this row. We should check if we can build else where?
         _flowToOtherBuildLogic = true;
      }

   }
   //***************************************************************************************************************************************************************************************************************************

   //***************************************************************************************************************************************************************************************************************************
   //no attack and no defence -> Build defence
   else if (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, rowWithMaxAttackBuildings) == false && _gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, rowWithMaxAttackBuildings) == false)
   {
      int availableEnergy = _gameState->GetPlayerA()->GetAvailableEnergy();
      int energyPerTurn = _gameState->GetGameDetails()->GetRoundIncomeEnergy() + _gameState->GetNumberBuildings(_playerABuildings, BuildingType::Energy)*_gameState->GetGameDetails()->GetEnergyBuilding()->GetEnergyGeneratedPerTurn();
      if ((availableEnergy + 2*energyPerTurn) >= 50)//2* because it would take turns before the defence is built, and during that turns we generate energy
      {
         //build defence
         AttackDetails* tempAttack = BuildDefenceInRow(rowWithMaxAttackBuildings);
         if (tempAttack != nullptr)
         {
            bestAttack = new AttackDetails(tempAttack->GetAttackFromX(), tempAttack->GetAttackFromY(), tempAttack->GetTimeTillMissileImpactPlayerB(), tempAttack->GetTimeTillMissileImpactPlayerA());
            bestAttack->SetBuildType(BuildingType::Defence);
            delete tempAttack;
         }
      }
   }
   //***************************************************************************************************************************************************************************************************************************

   //***************************************************************************************************************************************************************************************************************************
   //no attack and have defence -> Build Attack
   else if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, rowWithMaxAttackBuildings) == true) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, rowWithMaxAttackBuildings) == false))
   {
      AttackDetails* tempAttack = BuildAttackBehindDefense(rowWithMaxAttackBuildings);
      if (tempAttack != nullptr)
      {
         bestAttack = new AttackDetails(tempAttack->GetAttackFromX(), tempAttack->GetAttackFromY(), tempAttack->GetTimeTillMissileImpactPlayerB(), tempAttack->GetTimeTillMissileImpactPlayerA());
         bestAttack->SetBuildType(BuildingType::Attack);
         delete tempAttack;
      }
   }
   //***************************************************************************************************************************************************************************************************************************

   return bestAttack;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AttackDetails* GameStrategy::BuildAttackBehindDefense(int rowWithMaxAttackBuildings)
{
   AttackDetails* bestAttack = nullptr;

   //where is the defence building?
   int xBuildingLocation = 2;// furtherest back we can build a defensive building
   int constructionTimeLeft = -1;

   for (int x = (_mapWidth / 2 - 1); x > 0; x--)
   {
      int key = _gameState->ConvertXY_To_Key(x, rowWithMaxAttackBuildings);
      std::unordered_map<int16_t, Building*> ::const_iterator it = _playerABuildings.find(key);
      if (it != _playerABuildings.end())
      {
         Building* building = it->second;
         if (building->GetBuildingType() == BuildingType::Defence)// check if this is defence building
         {
            xBuildingLocation = building->GetX();
            constructionTimeLeft = building->GetConstructionTimeLeft();
            break;
         }
      }
   }

   if (constructionTimeLeft <= 0)// Has our defence building been built?
   {
      for (int x = xBuildingLocation - 1; x >= 0; x--)// we want to build behind the building
      {
         bool canBuild = IsEnergyPosition(x) == false && CanBuildAtPosition(x, rowWithMaxAttackBuildings);
         if (canBuild)
         {
            bestAttack = new AttackDetails(x, rowWithMaxAttackBuildings, 0, 1000);
            bestAttack->SetBuildType(BuildingType::Attack);
            break;
         }
      }
   }
   return bestAttack;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

AttackDetails* GameStrategy::BuildDefenceInRow(int rowWithMaxAttackBuildings)
{
   AttackDetails* bestAttack = nullptr;
   //where is the attack building?
   int xAttackBuildingLocation = 1;// if there is no attack building then default the attack location to 1. the furtherest back an attack could have been built.
   for (int x = (_mapWidth / 2 - 1); x >= 0; x--)
   {
      int key = _gameState->ConvertXY_To_Key(x, rowWithMaxAttackBuildings);
      std::unordered_map<int16_t, Building*> ::const_iterator it = _playerABuildings.find(key);
      if (it != _playerABuildings.end())
      {
         Building* building = it->second;
         if (building->GetBuildingType() == BuildingType::Attack)// check if this is an attack building
         {
            xAttackBuildingLocation = building->GetX();
            break;
         }
      }
   }

   for (int x = (_mapWidth / 2 - 1); x > xAttackBuildingLocation; x--)// we want to build in front of the building.
   {
      bool canBuild = IsEnergyPosition(x) == false && CanBuildAtPosition(x, rowWithMaxAttackBuildings);
      if (canBuild)
      {
         bestAttack = new AttackDetails(x, rowWithMaxAttackBuildings, 0, 1000);
         bestAttack->SetBuildType(BuildingType::Defence);
         break;
      }
   }
   return bestAttack;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

AttackDetails* GameStrategy::EvaluateBestAttack(AttackDetails* tempAttack, AttackDetails* bestAttack)
{
   if (tempAttack != nullptr)
   {
      if (bestAttack == nullptr)
      {
         bestAttack = new AttackDetails(tempAttack->GetAttackFromX(), tempAttack->GetAttackFromY(), tempAttack->GetTimeTillMissileImpactPlayerB(), tempAttack->GetTimeTillMissileImpactPlayerA());
      }
      else if (bestAttack->GetTimeTillMissileImpactPlayerA() < tempAttack->GetTimeTillMissileImpactPlayerA())
      {
         bestAttack->SetAttack(tempAttack);
      }
      else if (bestAttack->GetTimeTillMissileImpactPlayerA() == tempAttack->GetTimeTillMissileImpactPlayerA())
      {
         if (bestAttack->GetTimeTillMissileImpactPlayerB() > tempAttack->GetTimeTillMissileImpactPlayerB())
         {
            bestAttack->SetAttack(tempAttack);
         }
         else if (bestAttack->GetTimeTillMissileImpactPlayerB() == tempAttack->GetTimeTillMissileImpactPlayerB())
         {
            int buildCountForBestAttack = _gameState->GetNumberBuildingsInRow(_playerABuildings, BuildingType::Attack, bestAttack->GetAttackFromY());
            int buildCountForTempAttack = _gameState->GetNumberBuildingsInRow(_playerABuildings, BuildingType::Attack, tempAttack->GetAttackFromY());
            if (buildCountForBestAttack > buildCountForTempAttack)// lets check the amount of buildings in the row
            {
               bestAttack->SetAttack(tempAttack);
            }
         }
      }
      delete tempAttack;
   }

   return bestAttack;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
-----------------
| E | A | D | R |
-----------------
| 0 | 0 | 0 | A/D |6.1
| 0 | 0 | 1 | A |6.2
| 0 | 1 | 0 | D |4
| 0 | 1 | 1 | A |5
| 1 | 0 | 0 | A/D |3.1 // attack/defend depending if player b has a defensive building
| 1 | 0 | 1 | A |3.2
| 1 | 1 | 0 | D |1
| 1 | 1 | 1 | A |2
*/
AttackDetails* GameStrategy::FindBestAttackOnSingleAttackBuilding(std::vector<Building*> playerBAttackBuildings)
{
   AttackDetails* bestAttack = nullptr;

   //***************************************************************************************************************************************************************************************************************************
   //have energy and attack building and no defense -> defense
   //for (int index = 0; index < static_cast<int>(playerBAttackBuildings.size()); index++)
   //{
   //   Building* buildingToAttack = playerBAttackBuildings.at(index);
   //   if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Energy, buildingToAttack->GetY()) == true) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == true) &&
   //      (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, buildingToAttack->GetY()) == false))
   //   {
   //      AttackDetails* tempAttack = BuildDefenceInRow(buildingToAttack->GetY());
   //      if (tempAttack != nullptr)
   //      {
   //         bestAttack = new AttackDetails(tempAttack->GetAttackFromX(), tempAttack->GetAttackFromY(), tempAttack->GetTimeTillMissileImpactPlayerB(), tempAttack->GetTimeTillMissileImpactPlayerA());
   //         delete tempAttack;
   //         break;
   //      }
   //   }
   //}
   //if (bestAttack != nullptr)
   //{
   //   bestAttack->SetBuildType(BuildingType::Defence);
   //   return bestAttack;
   //}
   //***************************************************************************************************************************************************************************************************************************

   //***************************************************************************************************************************************************************************************************************************
   //have energy and attack and defense -> attack
   for (int index = 0; index < static_cast<int>(playerBAttackBuildings.size()); index++)
   {
      Building* buildingToAttack = playerBAttackBuildings.at(index);
      if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Energy, buildingToAttack->GetY()) == true) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == true) &&
         (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, buildingToAttack->GetY()) == true))
      {
         AttackDetails* tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY(), false);
         bestAttack = EvaluateBestAttack(tempAttack, bestAttack);
      }
   }
   if (bestAttack != nullptr)
   {
      bestAttack->SetBuildType(BuildingType::Attack);
      return bestAttack;
   }
   //***************************************************************************************************************************************************************************************************************************

   //***************************************************************************************************************************************************************************************************************************
   //have energy and no attack and defence-> Attack/Defend
   for (int index = 0; index < static_cast<int>(playerBAttackBuildings.size()); index++)
   {
      Building* buildingToAttack = playerBAttackBuildings.at(index);
      if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Energy, buildingToAttack->GetY()) == true) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == false) &&
         (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, buildingToAttack->GetY()) == false))
      {
         bool playerBHasDefenceInFrontAttack = false;
         for (int x = buildingToAttack->GetX(); x >= (_mapWidth / 2); x--)
         {
            playerBHasDefenceInFrontAttack = _gameState->DoesPlayerHaveBuildingTypeAtPostion(_playerBBuildings, x, buildingToAttack->GetY(), BuildingType::Defence);
         }

         if (playerBHasDefenceInFrontAttack == true)
         {
            // we cannot attack because player b already has a defensive building up and attacking building. we must defend first
            AttackDetails* tempAttack = BuildDefenceInRow(buildingToAttack->GetY());
            if (tempAttack != nullptr)
            {
               bestAttack = new AttackDetails(tempAttack->GetAttackFromX(), tempAttack->GetAttackFromY(), tempAttack->GetTimeTillMissileImpactPlayerB(), tempAttack->GetTimeTillMissileImpactPlayerA());
               delete tempAttack;
               break;
            }
         }
      }
   }
   if (bestAttack != nullptr)
   {
      bestAttack->SetBuildType(BuildingType::Defence);
      return bestAttack;
   }

   // run similar logic if there are no defensive buildings defending player b attacking building
   for (int index = 0; index < static_cast<int>(playerBAttackBuildings.size()); index++)
   {
      Building* buildingToAttack = playerBAttackBuildings.at(index);
      if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Energy, buildingToAttack->GetY()) == true) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == false) &&
         (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, buildingToAttack->GetY()) == false))
      {
         {
            //player b has a single attack building with no defence in front of it
            AttackDetails* tempAttack = nullptr;
            //Does enemy have building  at the end?
            bool playerBEnergyBuildingEnd = _gameState->DoesPlayerHaveBuildingTypeAtPostion(_playerBBuildings, _mapWidth - 1, buildingToAttack->GetY(), BuildingType::Energy);
            if (playerBEnergyBuildingEnd == true)
            {
               tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY());
            }
            else
            {
               tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY(), false);// lets not build Attack at the front
            }

            bestAttack = EvaluateBestAttack(tempAttack, bestAttack);
         }
      }
   }

   if (bestAttack != nullptr)
   {
      bestAttack->SetBuildType(BuildingType::Attack);
      return bestAttack;
   }
   //***************************************************************************************************************************************************************************************************************************

   //***************************************************************************************************************************************************************************************************************************
   //have energy and defence and no attack -> Attack
   for (int index = 0; index < static_cast<int>(playerBAttackBuildings.size()); index++)
   {
      Building* buildingToAttack = playerBAttackBuildings.at(index);
      if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Energy, buildingToAttack->GetY()) == true) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == false) &&
         (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, buildingToAttack->GetY()) == true))
      {
         //AttackDetails* tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY());
         // bestAttack = EvaluateBestAttack(tempAttack, bestAttack);
         AttackDetails* tempAttack = BuildAttackBehindDefense(buildingToAttack->GetY());
         if (tempAttack != nullptr)
         {
            bestAttack = new AttackDetails(tempAttack->GetAttackFromX(), tempAttack->GetAttackFromY(), tempAttack->GetTimeTillMissileImpactPlayerB(), tempAttack->GetTimeTillMissileImpactPlayerA());
            bestAttack->SetBuildType(BuildingType::Attack);
            delete tempAttack;
            break;
         }
      }
   }
   if (bestAttack != nullptr)
   {
      bestAttack->SetBuildType(BuildingType::Attack);
      return bestAttack;
   }
   //***************************************************************************************************************************************************************************************************************************

   //***************************************************************************************************************************************************************************************************************************
   ////no energy and defense and have attack->defense
   //   for (int index = 0; index < static_cast<int>(playerBAttackBuildings.size()); index++)
   //   {
   //      Building* buildingToAttack = playerBAttackBuildings.at(index);
   //      if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Energy, buildingToAttack->GetY()) == false) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == true) &&
   //         (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, buildingToAttack->GetY()) == false))
   //      {
   //         AttackDetails* tempAttack = BuildDefenceInRow(buildingToAttack->GetY());
   //         if (tempAttack != nullptr)
   //         {
   //            bestAttack = new AttackDetails(tempAttack->GetAttackFromX(), tempAttack->GetAttackFromY(), tempAttack->GetTimeTillMissileImpactPlayerB(), tempAttack->GetTimeTillMissileImpactPlayerA());
   //            delete tempAttack;
   //            break;
   //         }
   //      }
   //   }
   //if (bestAttack != nullptr)
   //{
   //   bestAttack->SetBuildType(BuildingType::Defence);
   //   return bestAttack;
   //}
   //***************************************************************************************************************************************************************************************************************************

   //***************************************************************************************************************************************************************************************************************************
   //no energy and have attack and defense  -> Attack
   for (int index = 0; index < static_cast<int>(playerBAttackBuildings.size()); index++)
   {
      Building* buildingToAttack = playerBAttackBuildings.at(index);
      if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Energy, buildingToAttack->GetY()) == false) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == true) &&
         (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, buildingToAttack->GetY()) == true))
      {
         AttackDetails* tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY(), false);
         bestAttack = EvaluateBestAttack(tempAttack, bestAttack);
      }
   }
   if (bestAttack != nullptr)
   {
      bestAttack->SetBuildType(BuildingType::Attack);
      return bestAttack;
   }
   //***************************************************************************************************************************************************************************************************************************

   //***************************************************************************************************************************************************************************************************************************
   //no energy and no attack and defence-> Attack/Defend
   for (int index = 0; index < static_cast<int>(playerBAttackBuildings.size()); index++)
   {
      Building* buildingToAttack = playerBAttackBuildings.at(index);
      if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Energy, buildingToAttack->GetY()) == false) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == false) &&
         (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, buildingToAttack->GetY()) == false))
      {

         bool playerBHasDefenceInFrontAttack = false;
         for (int x = buildingToAttack->GetX(); x >= (_mapWidth / 2); x--)
         {
            playerBHasDefenceInFrontAttack = _gameState->DoesPlayerHaveBuildingTypeAtPostion(_playerBBuildings, x, buildingToAttack->GetY(), BuildingType::Defence);
         }

         if (playerBHasDefenceInFrontAttack == true)
         {
            // we cannot attack because player b already has a defensive building up and attacking building. we must defend first
            AttackDetails* tempAttack = BuildDefenceInRow(buildingToAttack->GetY());
            if (tempAttack != nullptr)
            {
               bestAttack = new AttackDetails(tempAttack->GetAttackFromX(), tempAttack->GetAttackFromY(), tempAttack->GetTimeTillMissileImpactPlayerB(), tempAttack->GetTimeTillMissileImpactPlayerA());
               delete tempAttack;
               break;
            }
         }
      }
   }
   if (bestAttack != nullptr)
   {
      bestAttack->SetBuildType(BuildingType::Defence);
      return bestAttack;
   }

   // run similar logic if there are no defensive buildings defending player b attacking building
   for (int index = 0; index < static_cast<int>(playerBAttackBuildings.size()); index++)
   {
      Building* buildingToAttack = playerBAttackBuildings.at(index);
      if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Energy, buildingToAttack->GetY()) == false) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == false) &&
         (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, buildingToAttack->GetY()) == false))
      {
         //player b has a single attack building with no defence in front of it
         AttackDetails* tempAttack = nullptr;
         //Does enemy have building  at the end?
         bool playerBEnergyBuildingEnd = _gameState->DoesPlayerHaveBuildingTypeAtPostion(_playerBBuildings, _mapWidth - 1, buildingToAttack->GetY(), BuildingType::Energy);
         if (playerBEnergyBuildingEnd == true)
         {
            tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY());
         }
         else
         {
            tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY(), false);// lets not build Attack at the front
         }

         bestAttack = EvaluateBestAttack(tempAttack, bestAttack);
      }
   }

   if (bestAttack != nullptr)
   {
      bestAttack->SetBuildType(BuildingType::Attack);
      return bestAttack;
   }
   //***************************************************************************************************************************************************************************************************************************

   //***************************************************************************************************************************************************************************************************************************
   //no energy and no attack and have defence-> Attack
   for (int index = 0; index < static_cast<int>(playerBAttackBuildings.size()); index++)
   {
      Building* buildingToAttack = playerBAttackBuildings.at(index);
      if ((_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Energy, buildingToAttack->GetY()) == false) && (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == false) &&
         (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Defence, buildingToAttack->GetY()) == true))
      {
         //AttackDetails* tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY());
        // bestAttack = EvaluateBestAttack(tempAttack, bestAttack);
         AttackDetails* tempAttack = BuildAttackBehindDefense(buildingToAttack->GetY());
         if (tempAttack != nullptr)
         {
            bestAttack = new AttackDetails(tempAttack->GetAttackFromX(), tempAttack->GetAttackFromY(), tempAttack->GetTimeTillMissileImpactPlayerB(), tempAttack->GetTimeTillMissileImpactPlayerA());
            bestAttack->SetBuildType(BuildingType::Attack);
            delete tempAttack;
            break;
         }
      }
   }
   if (bestAttack != nullptr)
   {
      bestAttack->SetBuildType(BuildingType::Attack);
      return bestAttack;
   }
   //***************************************************************************************************************************************************************************************************************************

   //***************************************************************************************************************************************************************************************************************************
   //// if we get here then, we have attack building on all rows that have enemy attack buildings. lets build attack buildings on rows that have other enemy buildings. aka energy buildings
   // this is already done higher up.
   //std::vector<Building*> playerBEnergyBuildings;
   //playerBEnergyBuildings = _gameState->GetBuildingList(_playerBBuildings, BuildingType::Energy);

   //for (int index = 0; index < static_cast<int>(playerBEnergyBuildings.size()); index++)
   //{
   //   Building* buildingToAttack = playerBEnergyBuildings.at(index);
   //   if (_gameState->DoesRowHaveBuildingType(_playerABuildings, BuildingType::Attack, buildingToAttack->GetY()) == false)
   //   {
   //      AttackDetails* tempAttack = nullptr;
   //      //Does player B have building  at the end?
   //      bool playerBEnergyBuildingEnd = _gameState->DoesPlayerHaveBuildingTypeAtPostion(_playerBBuildings, _mapWidth - 1, buildingToAttack->GetY(), BuildingType::Energy);
   //      if (playerBEnergyBuildingEnd == true)
   //      {
   //         tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY());
   //      }
   //      else
   //      {
   //         tempAttack = CanAttackPosition(buildingToAttack->GetX(), buildingToAttack->GetY(), false);// lets not build Attack at the front
   //      }
   //      bestAttack = EvaluateBestAttack(tempAttack, bestAttack);
   //   }
   //}
   //if (bestAttack != nullptr)
   //{
   //   bestAttack->SetBuildType(BuildingType::Attack);
   //   return bestAttack;
   //}
   //***************************************************************************************************************************************************************************************************************************
   if (bestAttack == nullptr)
   {
      _flowToOtherBuildLogic = true;
   }
   return bestAttack;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameStrategy::CanBuildAtPosition(int16_t x, int16_t y)
{
   int16_t key = _gameState->ConvertXY_To_Key(x, y);
   std::unordered_map<int16_t, Building*> ::const_iterator it = _playerABuildings.find(key);
   if (it == _playerABuildings.end())
   {
      return true;
   }

   return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//buildFront == true -> we are allowed to build at position (_mapWidth/2 -1) ie posiion x=7
AttackDetails* GameStrategy::CanAttackPosition(int16_t xPosToAttack, int16_t yPosToAttack, bool buildFront)
{
   AttackDetails* attack = nullptr;

   for (int buildX = (_mapWidth / 2) - 1; buildX >= 0; buildX--)//-1  -->> for zero index start.
   {
      if (buildFront == false)
      {
         if (buildX == ((_mapWidth / 2 - 1)))
         {
            continue; //lets not build at the front.
         }
      }

      if (IsAttackPosition(buildX) == true && CanBuildAtPosition(buildX, yPosToAttack))
      {
         int impactTimeToPlayerB = CalculateTimetoAttackPosition(buildX, xPosToAttack, yPosToAttack);
         int impactTimeToPlayerA = TimeTillMissileImpactPlayerABuilding(buildX, yPosToAttack);

         attack = new AttackDetails(buildX, yPosToAttack, impactTimeToPlayerB, impactTimeToPlayerA);
         break;
      }
   }
   return attack;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int GameStrategy::CalculateTimetoAttackPosition(int buildX, int16_t xPosToAttack, int16_t yPosToAttack)
{
   Building* attackBuild = _gameState->GetGameDetails()->GetAttackBuilding();
   int weaponSpeed = attackBuild->GetWeaponSpeed();

   int timeToDestroyOtherBuildings = 0;
   for (int x = buildX; x < xPosToAttack; x++)
   {
      int key = _gameState->ConvertXY_To_Key(x, yPosToAttack);
      std::unordered_map<int16_t, Building*> ::const_iterator it = _playerBBuildings.find(key);
      if (it != _playerBBuildings.end())
      {
         Building* building = it->second;
         int numMissilesToDestroyBuild = (int)std::ceil(building->GetHealth() / (double)attackBuild->GetWeaponDamage());
         timeToDestroyOtherBuildings = timeToDestroyOtherBuildings + numMissilesToDestroyBuild * attackBuild->GetWeaponCooldownPeriod();
      }
   }

   int distance = (xPosToAttack - buildX - weaponSpeed);//-weaponSpeed because once the building is built, the missile moves
   distance = distance < 0 ? 0 : distance;
   int travelTimeToMainBuilding = 0;
   travelTimeToMainBuilding = (int)std::ceil(distance / (double)weaponSpeed);

   int constructionTime = 0;
   constructionTime = attackBuild->GetConstructionTime();

   int impactTime = constructionTime + timeToDestroyOtherBuildings + travelTimeToMainBuilding;
   return impactTime;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int GameStrategy::TimeTillMissileImpactPlayerABuilding(int xLoc, int yLoc)
{
   int totalHealth = 0;
   for (int x = xLoc + 1; x < _mapWidth / 2; x++)//+1 because we want all buildings in front
   {
      int key = _gameState->ConvertXY_To_Key(x, yLoc);
      std::unordered_map<int16_t, Building*> ::const_iterator it = _playerABuildings.find(key);
      if (it != _playerABuildings.end())
      {
         Building* building = it->second;
         totalHealth += building->GetHealth();// need to consider constructing buildings
      }
   }
   //int totalHealthDup = totalHealth;

   int turnsTillImpact = 1000;// no impact
   std::vector<Missile *>playerBMissiles;
   playerBMissiles = _gameState->GetPlayerB()->GetPlayerMissiles();// we checking how long would player B missile reach player A 

   for (int index = 0; index < static_cast<int>(playerBMissiles.size()); index++)
   {
      Missile* missile = playerBMissiles.at(index);
      if (missile->GetY() == yLoc && missile->GetX() >= xLoc)// missile on the same row and missile is in front of building. if missile is behind building->  no impact
      {
         totalHealth = totalHealth - missile->GetDamage();
         if (totalHealth < 0)
         {
            int tempImpactTime = (int)std::ceil((missile->GetX() - xLoc) / (double)missile->GetSpeed());
            if (tempImpactTime < turnsTillImpact)
            {
               turnsTillImpact = tempImpactTime;
            }
         }
      }
   }

   //now need to check attack buildings
   //if (turnsTillImpact == 1000)
   //if (totalHealth > 0)
   //{
   std::vector<Building*>playerBAttackBuildings;
   playerBAttackBuildings = _gameState->GetBuildingListRow(_playerBBuildings, BuildingType::Attack, yLoc);

   for (int index = 0; index < static_cast<int>(playerBAttackBuildings.size()); index++)
   {
      Building* building = playerBAttackBuildings.at(index);

      totalHealth = totalHealth - building->GetWeaponDamage();
      if (totalHealth < 0)
      {
         int tempImpactTime = (int)std::ceil((building->GetX() - xLoc) / (double)building->GetWeaponSpeed()) + building->GetWeaponCooldownTimeLeft();
         if (tempImpactTime < turnsTillImpact)
         {
            turnsTillImpact = tempImpactTime;
         }
      }
   }
   //}

   return turnsTillImpact;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool GameStrategy::PlayForPoints()
{
   int playerAHealth = _gameState->GetPlayerA()->GetHealth();
   int playerBHealth = _gameState->GetPlayerB()->GetHealth();

   std::vector<Missile*> playerAMissiles;
   playerAMissiles = _gameState->GetPlayerA()->GetPlayerMissiles();
   std::vector<Missile*> playerBMissiles;
   playerBMissiles = _gameState->GetPlayerB()->GetPlayerMissiles();

   int potentialDirectDamageToPlayerB = CalculateDirectDamageToPlayer(_playerBBuildings, playerAMissiles, "playerB");// damage player a will deal to player b
   int potentialDirectDamageToPlayerA = CalculateDirectDamageToPlayer(_playerABuildings, playerBMissiles, "playerA");

   int playerAEnergyBuildingCount = _gameState->GetNumberBuildings(_playerABuildings, BuildingType::Energy);
   int playerBEnergyBuildingCount = _gameState->GetNumberBuildings(_playerBBuildings, BuildingType::Energy);

   int playerAAttackBuildingCount = _gameState->GetNumberBuildings(_playerABuildings, BuildingType::Attack);
   int playerBAttackBuildingCount = _gameState->GetNumberBuildings(_playerBBuildings, BuildingType::Attack);

   
   //Do I more health then player b and not going to lose soon
   bool shouldPlayForPoints = ((playerAHealth - potentialDirectDamageToPlayerA) > 0) && ((playerAHealth - playerBHealth) > 0);
   if (shouldPlayForPoints == true)
   {
      // Is player b about to lose
      if (playerBHealth - potentialDirectDamageToPlayerB <= 0)
      {
         shouldPlayForPoints = true;
      }
      // Is player b about to lose and Do i have more attack building then my opponent
      else if (playerBHealth - potentialDirectDamageToPlayerB <= 20 && (playerAAttackBuildingCount- playerBAttackBuildingCount) > 8)
      {
         shouldPlayForPoints = true;
      }
      else
      {
         shouldPlayForPoints = false;
      }
   }

   return shouldPlayForPoints;
}

int GameStrategy::CalculateDirectDamageToPlayer(std::unordered_map<int16_t, Building*> playerBuildings, std::vector<Missile*>oncomingMissiles, std::string damageToPlayer)
{
   int playerDamage = 0;
   if (damageToPlayer.compare("playerB") == 0)
   {
      for (int index = 0; index < static_cast<int>(oncomingMissiles.size()); index++)
      {
         Missile* missile = oncomingMissiles.at(index);
         for (int x = missile->GetX(); x < _mapWidth; x++)
         {
            int key = _gameState->ConvertXY_To_Key(x, missile->GetY());
            std::unordered_map<int16_t, Building*> ::const_iterator it = playerBuildings.find(key);
            if (it != playerBuildings.end())
            {
               Building* build = it->second;
               if (build->GetConstructionTimeLeft() < 0)
               {
                  // we found a building that is constructed. hence no damage
                  break; // lets go to the next missile
               }
            }
            if (x == _mapWidth - 1)
            {
               // we reached the end and found no building
               playerDamage += missile->GetDamage();
            }
         }
      }
   }
   else
   {
      for (int index = 0; index < static_cast<int>(oncomingMissiles.size()); index++)
      {
         Missile* missile = oncomingMissiles.at(index);
         for (int x = missile->GetX(); x >= 0; x--)
         {
            int key = _gameState->ConvertXY_To_Key(x, missile->GetY());
            std::unordered_map<int16_t, Building*> ::const_iterator it = playerBuildings.find(key);
            if (it != playerBuildings.end())
            {
               Building* build = it->second;
               if (build->GetConstructionTimeLeft() < 0)
               {
                  // we found a building that is constructed. hence no damage
                  break; // let go to the next missile
               }
            }
            if (x == 0)
            {
               // we reached the end and found no building
               playerDamage += missile->GetDamage();
            }
         }
      }
   }
   
   return playerDamage;

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GameStrategy::SaveGameData(std::string filePath)
{
   if (_buildingType == static_cast<int16_t>(BuildingType::Energy))
   {
      _energyBuildingsPlacedSoFar++;
      std::string dataToSave = std::to_string(_energyBuildingsPlacedSoFar);
      FileHelper helper;
      helper.WriteInformation(filePath, dataToSave);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------