#ifndef PLAYGAME
#define PLAYGAME
#include "FileHelper.h"
#include "GameState.h"
#include "GameStrategy.h"

namespace
{
   const char* stateFilename = "state.json";
   const char* placeFilename = "place.txt";
   const char* commandFilename = "command.txt";
   const char* previousGameStateFilename = "previousGameState.txt";
}

class PlayGame
{
private:
   GameState* _gameState;
   std::string _playingDir;
   GameStrategy* _gameStrategy;

   std::string ReadGameState();

public:
   PlayGame(std::string playingDir);
   ~PlayGame();
   void Play();
};

#endif