/*--------------------------------------------------*/
/*---------------------- Helpers -------------------*/
/*--------------------------------------------------*/
#include <Arduino.h>
#include "func_def.h"

#define _DEBUG
#define DebugSerial Serial
// local function
void comm_print_setting(setting_packet_t dp)
{
	Debug("_comm_data1", dp._comm_data1);
	Debug("_comm_data2", dp._comm_data2);
	Debug("_comm_data3", dp._comm_data3);
	Debug("_comm_data4", dp._comm_data4);
	Debug("_comm_data5", dp._comm_data5);
	Debug("_comm_data6", dp._comm_data6);
}

void Debug(const char* title, int value)
{
#ifdef _DEBUG
	DebugSerial.print(title);
	DebugSerial.print(": ");
	DebugSerial.println(value);
#endif // DEBUG

}

void Debug(const char* title, const char* value)
{
#ifdef _DEBUG
	DebugSerial.print(title);
	DebugSerial.print(": ");
	DebugSerial.println(value);
#endif // DEBUG

}
void Debug(const char* title)
{
#ifdef _DEBUG
	DebugSerial.println(title);
#endif // DEBUG
}

// local function
uint8_t cal_crc(byte* pBuff, int len)
{
	uint8_t crc = 0;
	for (int i = 0; i < len; i++)
	{
		crc += pBuff[i];
	}
	crc = (~crc) + 1;
	return crc;
}