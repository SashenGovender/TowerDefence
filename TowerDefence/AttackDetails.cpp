#include "AttackDetails.h"

//AttackDetails::AttackDetails()
//{
//   _attackFromX = 0;
//   _attackFromY = 0;
//   _timeToReachPlayerB = 1000;
//   _timeToReachPlayerA = 1000;// this indicates no impact
//}
AttackDetails::AttackDetails(int16_t x, int16_t y, int16_t timeToReachPlayerB, int16_t timeToReachPlayerA)
{
   SetAttack(x, y, timeToReachPlayerB, timeToReachPlayerA);
}

void AttackDetails::SetAttack(int16_t x, int16_t y, int16_t timeToReachPlayerB, int16_t timeToReachPlayerA)
{
   _attackFromX = x;
   _attackFromY = y;
   _timeToReachPlayerB = timeToReachPlayerB;
   _timeToReachPlayerA = timeToReachPlayerA;
}

void AttackDetails::SetAttack(AttackDetails* attack)
{
   _attackFromX = attack->_attackFromX;
   _attackFromY = attack->_attackFromY;
   _timeToReachPlayerB = attack->_timeToReachPlayerB;
   _timeToReachPlayerA = attack->_timeToReachPlayerA;//time player b missile takes to reach this new attack building
}
