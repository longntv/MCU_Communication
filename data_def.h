#pragma once
#include <Arduino.h>

// Respone error code
#define STR_MSG						0xA0
#define RESP_MSG					0xA1
#define REQ_SET						0xA2	// request send setting data
#define END_MSG						0xA9	// end character

#define HEADERID_SETTING_PACKET		(0xF1)

#define RESP_OK						(0x0)
#define RESP_SIZE_NG				(0x1)	// size error
#define RESP_CRC_NG					(0x2)	// crc error
#define RESP_ID_NG					(0x3)	// id error

#define MAX_RESEND					5

typedef struct {
	uint8_t _comm_data1;
	uint8_t _comm_data2;
	uint8_t _comm_data3;
	uint8_t _comm_data4;

	uint16_t _comm_data5;
	uint16_t _comm_data6;
}  __attribute__((__packed__)) setting_packet_t;

typedef struct {
	uint8_t _msg_id;
	uint8_t _bIsUpdated;	// True: data is updated
							// False: data is not updated
	uint16_t _crc;
} __attribute__((__packed__))header_package_t;
typedef struct {
	header_package_t		_header;
	setting_packet_t		_data;
} __attribute__((__packed__))comm_data_t;

#define PACKET_HEADER_SIZE			sizeof(header_package_t)
#define PACKET_SETTING_SIZE			sizeof(setting_packet_t)
#define COMM_DATA_SIZE				sizeof(comm_data_t)

