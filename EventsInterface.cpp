#include "EventsInterface.h"
#include <iostream>

events::events(event_type evType, char maxListners):eventType(evType)
{
	evHandlerList.reserve(maxListners);
}

event_type events::type()
{
	return eventType;
}

events::~events()
{
	for (std::vector<eventHandlerInterface*>::iterator it = evHandlerList.begin(); it != evHandlerList.end(); it++)
	{
		delete (*it);
	}
	evHandlerList.clear();
}

void events::registerEvHandler(eventHandlerInterface* pEvHandler)
{
	//critical section starts -->
	if (evHandlerList.size() < evHandlerList.capacity() )
	{
		evHandlerList.push_back(pEvHandler);
	}
	else
	{
		std::cout <<"error" << std::endl;
	}
	//<-- critical section ends
}

void events::fire()
{
	//critical section starts -->
	for (std::vector<eventHandlerInterface*>::iterator it = evHandlerList.begin(); it != evHandlerList.end();)
	{
		(*it)->notify(this);
		if ((*it)->getNotifyType() == eventHandlerInterface::ONE_SHOT)
		{
			delete (*it);
			it = evHandlerList.erase(it);
		}
		else
		{
			it++;
		}
	}
	//<-- critical section ends 
}