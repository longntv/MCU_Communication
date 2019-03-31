/*--------------------------------------------------*/
/*--------------------Communication-----------------*/
/*--------------------------------------------------*/
//System included file

// Own included files
#include "func_def.h"

// communication data
comm_data_t _comm_data;
byte* _pdata = (byte*)&_comm_data;

// temperotary buffer
byte _uart_buffer[COMM_DATA_SIZE + 2];

// Local Function /////////////////////////////////////////////////////////////////////////////////
// send response
static void comm_send_resp(byte resp)
{
	Debug("Send response code", resp);
	// send start character
	MCU_Serial.write(RESP_MSG);
	// send content
	MCU_Serial.write(resp);
	// send end character
	MCU_Serial.write(RESP_MSG);
}

// Only receive setting data from MCU
static byte comm_process_data()
{
	comm_data_t dp;
	memcpy(&dp, _uart_buffer, COMM_DATA_SIZE);

	byte* pdp_data = (byte*)&dp._data;

	// Check valid packet
	if (dp._header._msg_id != HEADERID_SETTING_PACKET)
	{
		Debug("Msg Id error:", dp._header._msg_id);
		return RESP_ID_NG;
	}
	uint8_t crc = cal_crc(pdp_data, PACKET_SETTING_SIZE);
	if (crc == dp._header._crc)
	{
		// Valid packet
		_comm_data = dp;

		// print out debug data
		comm_print_setting(_comm_data._data);

		return RESP_OK;
	}
	else
	{
		Debug("Send crc", dp._header._crc);
		Debug("Calc crc", crc);
		DebugSerial.println("CRC Error");
		return RESP_CRC_NG;
	}

}

// Public Function /////////////////////////////////////////////////////////////////////////////////
// Initialization variable
void comm_init()
{
	// initialize data
	memset(&_comm_data, 0, sizeof(comm_data_t));
}

// Process of sending data to MCU
static void comm_send_setting_data()
{
	// update setting data
	_comm_data._data._comm_data1 += 1;
	_comm_data._data._comm_data2 += 2;
	_comm_data._data._comm_data3 += 3;
	_comm_data._data._comm_data4 += 4;
	_comm_data._data._comm_data5 += 5;
	_comm_data._data._comm_data6 += 6;

	// set flag to false to stop sending
	_comm_data._header._bIsUpdated = false;
	_comm_data._header._crc = cal_crc((byte*)&_comm_data._data, PACKET_SETTING_SIZE);
	Debug("_comm_data._header._crc", _comm_data._header._crc);
	comm_print_setting(_comm_data._data);

	DebugSerial.println("Start send setting data");
	// setup message ID
	_comm_data._header._msg_id = HEADERID_SETTING_PACKET;
	// send start message
	MCU_Serial.write(STR_MSG);
	for (int i = 0; i < COMM_DATA_SIZE; i++) {
		// for debug
		DebugSerial.print(_pdata[i]);
		// write data to buffer
		MCU_Serial.write(_pdata[i]);
	}
	// send end message
	MCU_Serial.write(END_MSG);

	DebugSerial.println(" ");
}
// Process of receiving data from MCU
void comm_recv_esp()
{
	byte inCommingByte;
	bool isUpdated = false;

	while (MCU_Serial.available())
	{
		inCommingByte = MCU_Serial.read();
		if (inCommingByte == STR_MSG)
		{
			// reset all buffer
			memset(_uart_buffer, 0, sizeof(_uart_buffer));
			// receive all buffer data
			int ret = MCU_Serial.readBytesUntil(END_MSG, _uart_buffer, COMM_DATA_SIZE);
			if (ret == COMM_DATA_SIZE)
			{
				DebugSerial.println("comm_process_data");
				byte resp = comm_process_data();
				comm_send_resp(resp);
			}
			else
			{
				DebugSerial.println("packet error");
				Debug("ret", ret);
				Debug("COMM_DATA_SIZE", COMM_DATA_SIZE);
				comm_send_resp(RESP_SIZE_NG);
			}
		}
		else if (inCommingByte == RESP_MSG)
		{
			byte resp[3];
			int ret = MCU_Serial.readBytesUntil(END_MSG, resp, 3);
			if (ret > 0)
			{
				if (resp[0] == RESP_OK)
				{
					// do nothing
					Debug("Received OK", RESP_OK);
				}
				else
				{
					Debug("Received NG", resp[0]);
					// set flag to resend data
					_comm_data._header._bIsUpdated = true;
				}
			}
		}
		else if (inCommingByte == REQ_SET)
		{
			_comm_data._header._bIsUpdated = true;
		}
		else if (inCommingByte == END_MSG)
		{
			//Debug("End message:", inCommingByte);
			// Do nothing
		}
	}
}
// Determine to sendd data to MCU
void comm_send_esp()
{
	// somewhere set flag _bIsUpdated -> true to start send data
	_comm_data._header._bIsUpdated = true;

	if (_comm_data._header._bIsUpdated)
	{
		comm_send_setting_data();
	}
}
