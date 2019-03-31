/*
 Name:		Avr_Comm.ino
 Created:	3/31/2019 4:14:58 PM
 Author:	Shinryu
*/

//Own header file
#include "func_def.h"

elapsedMillis	_elpMili_send_data;
SoftwareSerial MCU_Serial(AVR_SW_RX, AVR_SW_TX);

// the setup function runs once when you press reset or power the board
void setup() {
	MCU_Serial.begin(9600);
	DebugSerial.begin(115200);

	// Start communicaiton with ESP
	comm_init();
}

// the loop function runs over and over again until power down or reset
void loop() {
	// read serial data
	comm_recv_esp();

	if (_elpMili_send_data > 10000) // send data to MCU every 5s
	{
		_elpMili_send_data = 0;
		DebugSerial.println("Send data to ESP");
		// send serial data
		comm_send_esp();
	}
}
