#ifndef MISSILE
#define MISSILE

#include "JsonHelper.h"

class Missile
{
private:
   int16_t _damage;
   int16_t _speed;
   int16_t _x;
   int16_t _y;
   std::string _playerType;

public:
   Missile();
   ~Missile();
   void SetMissileDetails(JsonHelper* jsonMissile);
   int16_t GetX() const { return _x; }
   int16_t GetY() const { return _y; }
   int16_t GetSpeed() const { return _speed; }
   std::string GetPlayerType() const { return _playerType; }
   int16_t GetDamage() const { return _damage; }
};

#endif