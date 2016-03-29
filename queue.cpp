#include "queue.h"

template <typename T, typename L>
Queue<T, L>::Queue(): pQueue(nullptr), inPtr(0), outPtr(0), qSize(0)
{	
}

template <typename T, typename L>
Queue<T, L>::Queue(int size)
{
	qSize = size;
	pQueue = new T[qSize];
	inPtr = outPtr = 0;
}
template <typename T, typename L>
Queue<T, L>::~Queue()
{
	delete [] pQueue;
}
template <typename T, typename L>
void Queue<T, L>::setQSize(int size)
{
	qSize = size;
	pQueue = new T[qSize];
}
template <typename T, typename L>
int Queue<T, L>::getQSize()
{
	return qSize;
}
template <typename T, typename L>
bool Queue<T, L>::isQEmpty()
{
	bool isEmpty = false; 

	enter_critical();

	if (inPtr == outPtr)
	{
		isEmpty = true;
	}	

	exit_critical();

	return isEmpty;
}
template <typename T, typename L>
bool Queue<T, L>::isQFull()
{
	bool isFull = false;

	enter_critical();

	if (0 == getFreeSpace())
	{
		isFull = true;
	}
	
	exit_critical();

	return isFull;
}
template <typename T, typename L>
int Queue<T, L>::getUnreadElements()
{
	enter_critical();

	int diff = inPtr - outPtr;

	if (diff < 0)
	{
		diff = -diff;
	}
	if (inPtr < outPtr)
	{
		diff = qSize - diff;
	}

	exit_critical();

	return diff;
}
template <typename T, typename L>
int Queue<T, L>::getFreeSpace()
{
	return qSize - getUnreadElements();
}	
template <typename T, typename L>
bool Queue<T, L>::pushQ(T val)
{
	enter_critical();

	bool was_QEmpty = isQEmpty();

	bool isSuccess = false;

	int temp = inPtr + 1;

	if (temp >= qSize)
	{
		temp = 0;
	}
	if (temp == outPtr)
	{
		//Wait for semaphore here. It will be released by a pull from ISR
		exit_critical();
		qPushSemaphore.acquire();
		enter_critical();
	}
	if (temp != outPtr)
	{
		pQueue[inPtr] = val;
		inPtr = temp;
		if (true == was_QEmpty)
		{
			//Release semaphore if the queue was previously empty.
			qPullSemaphore.release();
		}
		isSuccess = true;
	}

	exit_critical();

	return isSuccess;
}

template <typename T, typename L>
bool Queue<T, L>::pushQ_isr(T val)
{	
	bool was_QEmpty = isQEmpty();

	bool isSuccess = false;

	int temp = inPtr + 1;

	if (temp >= qSize)
	{
		temp = 0;
	}	
	if (temp != outPtr)
	{
		pQueue[inPtr] = val;
		inPtr = temp;
		if (true == was_QEmpty)
		{
			//Release semaphore if the queue was previously empty.
			qPullSemaphore.release();
		}
		isSuccess = true;
	}

	return isSuccess;
}

template <typename T, typename L>
int Queue<T, L>::pushQ(T* const pVal, unsigned int numBytes)
{
	enter_critical();

	unsigned int numFreeSpace = getFreeSpace();

	if (numBytes > numFreeSpace)
	{
		numBytes = numFreeSpace;
	}
	for (unsigned int i = 0; i < numBytes; i++)
	{
		pushQ(pVal[i]);
	}

	exit_critical();

	return numBytes;
}


template <typename T, typename L>
bool Queue<T, L>::pullQ(T* const pVal)
{
	enter_critical();

	bool isSuccess = false;
	bool was_QFull = isQFull();

	if (inPtr == outPtr)
	{
		//Wait for someone to push an element to the queue
		exit_critical();
		qPullSemaphore.acquire();
		enter_critical();
	}
	if (inPtr != outPtr)
	{
		*pVal = pQueue[outPtr++];			
		if (outPtr >= qSize)
		{
			outPtr = 0;
		}
		if (true == was_QFull)
		{
			//Release semaphore if the queue was previously full
			qPushSemaphore.release();
		}
		isSuccess = true;
	}

	exit_critical();

	return isSuccess;
}	

template <typename T, typename L>
bool Queue<T, L>::pullQ_isr(T* const pVal)
{	
	bool isSuccess = false;
	bool was_QFull = isQFull();

	if (inPtr != outPtr)
	{
		*pVal = pQueue[outPtr++];			
		if (outPtr >= qSize)
		{
			outPtr = 0;
		}
		if (true == was_QFull)
		{
			//Release semaphore if the queue was previously full
			qPushSemaphore.release();
		}
		isSuccess = true;
	}

	return isSuccess;
}	
template <typename T, typename L>
int Queue<T, L>::pullQ(T* const pVal, unsigned int numBytes)
{
	enter_critical();

	unsigned int numUnreadElements = getUnreadElements();

	if (numBytes > numUnreadElements)
	{
		numBytes = numUnreadElements;
	}
	for (unsigned int i = 0; i < numBytes; i++)
	{
		pullQ(&pVal[i]);
	}

	exit_critical();

	return numBytes;		
}	