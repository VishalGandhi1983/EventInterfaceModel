#ifndef __EventsInterface_h__
#define __EventsInterface_h__

#include <vector>
#include "Queue.h"
#include <utility>

typedef enum
{
	evTimerTimeout,
	evUartRxAvail,
	evUartTxBuffEmpty,	
}event_type;

class events;
class observer;

class eventHandlerInterface
{		
public:	
	typedef enum
	{
		ONE_SHOT,
		PERIODIC,
	}NOTIFY_TYPE;

	eventHandlerInterface(NOTIFY_TYPE notifyType):type(notifyType)
	{}

	NOTIFY_TYPE getNotifyType()
	{
		return type;
	}

	virtual void notify(events* pEvent) = 0;
	virtual void* getListner() = 0;
private:
	NOTIFY_TYPE type;	
};


template <typename Listner>
class eventHandler: public eventHandlerInterface
{	
	typedef void (Listner::*pEventCallback)(events* );
	Listner* pEvListner;
	pEventCallback pEvCallback;	
	
public:
	eventHandler(Listner* pListner, pEventCallback pCallback, NOTIFY_TYPE notifyType ): 	
		eventHandlerInterface(notifyType),
		pEvListner(pListner), 
		pEvCallback(pCallback)
	{}
	void* getListner()
	{
		return (void*)pEvListner;
	}
	std::pair<Listner*, pEventCallback> getHandler()
	{
		return std::make_pair(pEvListner, pEvCallback);
	}
	void notify(events* pEvent)
	{
		(pEvListner->*pEvCallback)(pEvent);
	}
};

class events
{
	std::vector<eventHandlerInterface*> evHandlerList;
	event_type eventType;	
public:
	~events();
	events(event_type evType, char maxListners = 1);
	event_type type();
	void registerEvHandler(eventHandlerInterface* pEvHandler);	
	template <typename Listner>
	void unregisterEvHandler(Listner* pListner)
	{
		for (std::vector<eventHandlerInterface*>::iterator it = evHandlerList.begin(); it != evHandlerList.end(); it++)
		{
			if ((*it)->getListner() == (void*)pListner)
			{
				evHandlerList.erase(it);
				break;
			}
		}
	}
	void fire();
};



template <typename Src, typename Dest>
void connect(Src* pSrc, events (Src::*pEvents), Dest* pDes, void (Dest::*pCallback)(events*))
{
	(pSrc->*pEvents).registerEvHandler(new eventHandler<Dest>(pDes, pCallback, eventHandlerInterface::PERIODIC));
}

template <typename Src, typename Dest>
void disconnect(Src* pSrc, events (Src::*pEvents), Dest* pDes)
{
	(pSrc->*pEvents).unregisterEvHandler<Dest>(pDes);
}



















#endif