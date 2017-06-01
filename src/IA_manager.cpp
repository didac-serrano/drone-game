#ifndef IA_MANAGER_CPP
#define IA_MANAGER_CPP

#include "playercontroller.h"
#include "game.h"
#include "bulletmanager.h"
#include "IA.h"
#include "checkcollisions.h"


// IA Controller
IA_Manager::IA_Manager() {};

IA_Manager::~IA_Manager() {};

/*
void IA_Manager::addLockerEntity(IA_Locker* newLocker)
{
	lockerIAs.push_back(newLocker);
} */

void IA_Manager::addStaticEntity(IA_Turret* newDetector)
{
	staticIAs.push_back(newDetector);
}

void IA_Manager::addDynamicEntity(IA_Drone* newDrone)
{
	dynamicIAs.push_back(newDrone);
}

void IA_Manager::update(double seconds_elapsed)
{
	/*
	for (int i = 0; i < size(lockerIAs); i++)
	{
		lockerIAs[i]->update(seconds_elapsed);
	}*/

	for (int i = 0; i < size(staticIAs); i++)
	{
		staticIAs[i]->update(seconds_elapsed);
	}
	/*
	for (int i = 0; i < size(dynamicIAs); i++)
	{
		dynamicIAs[i]->update(seconds_elapsed);
	}*/
}

void IA_Manager::warnAll(Vector3 lastSeenEnemy)
{
	for (int i = 0; i < size(staticIAs); i++)
	{
		staticIAs[i]->targetDetected = TRUE;
	}

	for (int i = 0; i < size(dynamicIAs); i++)
	{
		dynamicIAs[i]->targetDetected = TRUE;
	}
}

#endif // !IA_MANAGER_CPP