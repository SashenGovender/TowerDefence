#ifndef GAMECELL
#define GAMECELL

#include "Building.h"
#include "Missile.h"
#include <vector>

class GameCell
{
private:
   int16_t _x;
   int16_t _y;
   Building* _building;//should this be a vector? can we have more than 1 building pe cell?
   std::vector<Missile*> _missiles;
   std::string _cellOwner;

   void SetMissilesDetails(JsonHelper* mapCell);
   void SetBuildingsDetails(JsonHelper* mapCell);

public:
   GameCell();
   ~GameCell();
   void SetGameCellDetails(JsonHelper* mapCell);
   int16_t GetX() const { return _x; }
   int16_t GetY() const { return _y; }
   std::vector<Missile *> GetMissiles() const { return _missiles; }
   Building* GetBuilding() const { return _building; }

};

#endif