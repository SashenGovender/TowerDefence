#ifndef ATTACKDETAILS_H
#define ATTACKDETAILS_H
#include <stdint.h>

#include "Building.h"

class AttackDetails
{
private:
   int16_t _attackFromX;
   int16_t _attackFromY;
   int16_t _timeToReachPlayerB;
   int16_t _timeToReachPlayerA;
   BuildingType _buildType;
public:

   /*AttackDetails();*/
   AttackDetails(int16_t x, int16_t y, int16_t timeToReachPlayerB, int16_t timeToReachPlayerA);
   void SetAttack(int16_t x, int16_t y, int16_t timeToReachPlayerB, int16_t timeToReachPlayerA);
   void SetAttack(AttackDetails* attack);

   int16_t GetAttackFromX() const { return _attackFromX; }
   int16_t GetAttackFromY() const { return _attackFromY; }
   int16_t GetTimeTillMissileImpactPlayerB() const { return _timeToReachPlayerB; }
   int16_t GetTimeTillMissileImpactPlayerA() const { return _timeToReachPlayerA; }
   BuildingType GetBuildType() const { return _buildType; }
   void SetBuildType(BuildingType val) { _buildType = val; }
};

#endif