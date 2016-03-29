#ifndef __queue_h__
#define __queue_h__
#include "locks.h"

/*	
	Disable the global interrupt in enter_critical() and increment the nested lock counter.
*/
#define enter_critical()
/*
	Decrement the nested lock counter in exit_critical() and check if it is equal to zero. If zero, enable the global interrupt. If not zero, do not
	enable the interrupt.
*/
#define exit_critical()

template <typename T, typename L = semalock>
class Queue
{
	T* pQueue;
	L qPushSemaphore;			//make sure the semaphore has an OS specific implementation supplied
	L qPullSemaphore;			//make sure the semaphore has an OS specific implementation supplied
	int qSize;
	int inPtr;
	int outPtr;
public:
	Queue();
	Queue(int size);
	~Queue();	
	void setQSize(int size);
	int getQSize();
	bool isQFull();
	bool isQEmpty();
	int getUnreadElements();	
	int getFreeSpace();	
	bool pushQ(T val);	
	bool pushQ_isr(T val);	
	int pushQ(T* const pVal, unsigned int numBytes);	
	bool pullQ(T* const pVal);	
	bool pullQ_isr(T* const pVal);
	int pullQ(T* const pVal, unsigned int numBytes);	
};

















#endif