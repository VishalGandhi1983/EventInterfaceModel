#ifndef __iUart_h__
#define __iUart_h__

#include "queue.h"
#include "EventsInterface.h"


class iUart
{
	Queue<char> txQueue;
	Queue<char> rxQueue;
protected:	
	
public:	
	typedef enum
	{
		BAUDRATE_9600,
		BAUDRATE_19200,
		BAUDRATE_38400,

	}BAUDRATE;

	typedef enum
	{
		PARITY_NONE,
		PARITY_ODD,
		PARITY_EVEN,
	}PARITY;

	events newCharAvailEvent;
	events txBuffEmptyEvent;

	iUart(int txQSize, int rxQSize);
	bool isTxDataAvail();
	bool isRxDataAvail();	

	int getNumRxBytes();	
	bool readRxByte(char* const pRxByte); 
	int readRxBuffer(char* const pRxBuffer, int numRxBytes);
	bool writeTxByte(char txByte);
	int writeTxBuffer(char* const pTxBuffer, int numTxBytes);

	bool readTxByte_isr(char* const pTxByte);		/*<--used by tx isr*/
	bool writeRxByte_isr(char rxByte);				/*<--used by rx isr*/
	

	virtual void config(BAUDRATE baudRate, PARITY parityBits) = 0;
	virtual void rx_enable() = 0;
	virtual void rx_disable() = 0;
	virtual void tx_enable() = 0;
	virtual void tx_disable() = 0;
	virtual void enable() = 0;
	virtual void disable() = 0;

};





















#endif