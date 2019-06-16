#include "Missile.h"

Missile::Missile()
{
   _damage = 0;
   _speed = 0;
   _x = 0;
   _y = 0;
   _playerType = "";
}

Missile::~Missile()
{
}

void Missile::SetMissileDetails(JsonHelper* jsonMissile)
{
   _damage = jsonMissile->GetInt("damage");
   _speed = jsonMissile->GetInt("speed");
   _x = jsonMissile->GetInt("x");
   _y = jsonMissile->GetInt("y");
   _playerType = jsonMissile->GetString("playerType");
}

