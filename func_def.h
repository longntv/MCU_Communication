#pragma once
#include "data_def.h"
#include <SoftwareSerial.h>
#include <elapsedMillis.h>

// extern variables
extern setting_packet_t	_setting_data;


#define AVR_SW_RX					10		// --> 11
#define AVR_SW_TX					11		// --> 10

#define DebugSerial Serial
extern SoftwareSerial MCU_Serial;

// common module
extern void comm_print_setting(setting_packet_t setting_data);
extern void Debug(const char* title, int value);
extern void Debug(const char* title, const char* value);
extern void Debug(const char* title);
extern byte cal_crc(byte* pBuff, int len);

// communication module
extern void comm_init();
extern void comm_send_esp();
extern void comm_recv_esp();

// orther modules
// ............
