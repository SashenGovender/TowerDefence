#include <iostream>
#include "PlayGame.h"

PlayGame::PlayGame(std::string playingDir)
{
   _gameState = nullptr;
   _gameStrategy = nullptr;
   _playingDir = playingDir;
}

PlayGame::~PlayGame()
{
   if (_gameState != nullptr)
   {
      delete _gameState;
      _gameState = nullptr;
   }
   if (_gameStrategy != nullptr)
   {
      delete _gameStrategy;
      _gameStrategy = nullptr;
   }
}

void PlayGame::Play()
{
   std::string dataRead = ReadGameState();
   if (dataRead.compare("") != 0)//Did we successfully read in data
   {
      std::cout << "Before GameState Initialise" << std::endl;
      _gameState = new GameState(dataRead);
      _gameState->PopulateGameState();
      std::cout << "GameState Initialise Complete" << std::endl;

      if (_gameState->GetGameDetails()->GetRound() == 0)
      {
         FileHelper helper;
         helper.WriteInformation(previousGameStateFilename, "0");
      }

      std::cout << "ExecuteStrategy" << std::endl;
      _gameStrategy = new GameStrategy(_gameState);
      _gameStrategy->ExecuteStrategyV2();
      std::cout << "Complete Strategy" << std::endl;

      //std::string filePathCommand = _playingDir + "\\" + commandFilename;
      std::string filePathCommand = commandFilename;
      std::cout << "Before ExecuteCommand" << std::endl;
      _gameStrategy->ExecuteCommand(filePathCommand);// Save to command.txt
      std::cout << "Complete ExecuteCommand" << std::endl;
      
      std::string filePathSave = previousGameStateFilename;
      _gameStrategy->SaveGameData(filePathSave);

   }
}

std::string PlayGame::ReadGameState()
{
   // std::string filePathGameState= _playingDir + "\\" + stateFilename;
   std::string filePathGameState = stateFilename;

   FileHelper helper;
   std::string jsonStringGameState = helper.ReadInformation(filePathGameState);
   if (jsonStringGameState == "" || jsonStringGameState.length() == 0)//Did we read any data
   {
      std::cout << "Error Reading GameState Data at:" << filePathGameState << std::endl;
      return "";
   }

   return jsonStringGameState;
}
