#include "Uart2.h"


uartISRHandler::uartISRHandler(int intNum, iUart* pOwner): pUartOwner(pOwner)
{	
	iInterruptInterface::registerIntHandler(intNum, this);
}

void uartISRHandler::isr()
{
	//check if rx interrupt
	uartRxHandler();

	//check if tx interrupt
	uartTxhandler();

}

unsigned char rxArr[] = {0x01, 0x03, 0x10, 0x00, 0x00, 0x01, 0xbb, 0xaa};
unsigned char rxCount = 0;
bool enableReception = false;

unsigned char txArr[20] = {0};
unsigned char txCount = 0;
bool enableTransmission = false;

void uartISRHandler::uartRxHandler()
{
	char rxByte = 0;
	//read received byte from uart register
	if ((enableReception) && rxCount < sizeof (rxArr))
	{		
		rxByte = rxArr[rxCount++];

		pUartOwner->writeRxByte_isr(rxByte);	
	}
}

void uartISRHandler::uartTxhandler()
{
	char txByte = 0;

	if (enableTransmission)
	{
		if (pUartOwner->readTxByte_isr(&txByte))
		{
			//actual uart transmission to be implemented here
			txArr[txCount++] = txByte;
		}
		else
		{
			pUartOwner->txBuffEmptyEvent.fire();
		}	
	}
	
}
/*----------------------------------------------------------------*/

Uart2::Uart2(int txQSize, int rxQSize):iUart(txQSize, rxQSize), uartHandler(0, this)
{
	
}

void Uart2::config(BAUDRATE baudRate, PARITY parityBits)
{

}
void Uart2::rx_enable()
{
	enableReception = true;
	rxCount = 0;
}
void Uart2::rx_disable()
{
	enableReception = false;
	rxCount = 0;
}
void Uart2::tx_enable()
{
	enableTransmission = true;
	txCount = 0;
}
void Uart2::tx_disable()
{
	enableTransmission = false;
	txCount = 0;
}
void Uart2::enable()
{

}
void Uart2::disable()
{

}