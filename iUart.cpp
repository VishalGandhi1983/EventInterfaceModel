#include "iUart.h"
#include "queue.cpp"

iUart::iUart(int txQSize, int rxQSize): newCharAvailEvent(event_type::evUartRxAvail), txBuffEmptyEvent(event_type::evUartTxBuffEmpty)
{
	txQueue.setQSize(txQSize);
	rxQueue.setQSize(rxQSize);
}

bool iUart::readTxByte_isr(char* const pTxByte)
{
	return txQueue.pullQ_isr(pTxByte);
}

bool iUart::writeRxByte_isr(char rxByte)
{
	if (true == rxQueue.pushQ_isr(rxByte))
	{
		newCharAvailEvent.fire();
		return true;
	}
	else
	{		
		return false;
	}
}

bool iUart::isTxDataAvail()
{
	if (txQueue.isQEmpty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool iUart::isRxDataAvail()
{
	if (rxQueue.isQEmpty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

int iUart::getNumRxBytes()
{
	return rxQueue.getUnreadElements();
}

bool iUart::readRxByte(char* const pRxByte)
{
	return rxQueue.pullQ(pRxByte);
}

int iUart::readRxBuffer(char* const pRxBuffer, int numRxBytes)
{
	return rxQueue.pullQ(pRxBuffer, numRxBytes);
}

bool iUart::writeTxByte(char txByte)
{
	return txQueue.pushQ(txByte);
}

int iUart::writeTxBuffer(char* const pTxBuffer, int numTxBytes)
{
	return txQueue.pushQ(pTxBuffer, numTxBytes);
}