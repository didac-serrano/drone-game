#ifndef IA_MANAGER_CPP
#define IA_MANAGER_CPP

#include "playercontroller.h"
#include "game.h"
#include "bulletmanager.h"
#include "IA.h"
#include "checkcollisions.h"

IA_Manager::IA_Manager() {};
IA_Manager::~IA_Manager() {};

IA_Manager* IA_Manager::myIAManager = 0;

void IA_Manager::addStaticEntity(IA_Turret* newTurret)
{
	staticIAs.push_back(newTurret);
}

void IA_Manager::addDynamicEntity(IA_Drone* newDrone)
{
	dynamicIAs.push_back(newDrone);
}

void IA_Manager::deleteStaticEntity(IA_Turret* turret)
{
	auto it = std::find(staticIAs.begin(), staticIAs.end(), turret);
	if (it == staticIAs.end())
	{
		std::cout << "Some real bad happened" << std::endl;
		return;
	}
	staticIAs.erase(it);
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

IA_Manager* IA_Manager::getInstance() {
	if (myIAManager == 0)
		myIAManager = new IA_Manager();

	return myIAManager;
}
#endif // !IA_MANAGER_CPP