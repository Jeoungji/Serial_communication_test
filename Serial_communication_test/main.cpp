#include <stdio.h>
#include <tchar.h>
#include "Serial.h"	// Library described above
#include <string>
#include <iostream>





// application reads from the specified serial port and reports the collected data
int main()
{
	printf("Welcome to the serial test app!\n\n");

	Serial SP(5, "ESP32");    // adjust as needed

	if (SP.IsConnected())
		std::cout << "Connetcted" << std::endl;
	
	if (!SP.Checking()) exit(0);
	Recvcom recvcom = { NULL, };
	Sendcom sendcom = { 0, 0};

	Sleep(100);
	while (SP.IsConnected())
	{
		// Recving ex)
		int a = SP.ReadData(recvcom);
		if (a) {
			std::cout << "size : " << a << "  x: " << recvcom.x << "  y: " << recvcom.y;
			std::cout << "  swL" << (int)recvcom.swL << "  swR" << (int)recvcom.swR << std::endl;
		}

		//Sending ex)
		sendcom.x++;
		if (sendcom.x > 100) sendcom.x = 0;

		sendcom.y = sendcom.y + 2;
		if (sendcom.y > 100) sendcom.y = 0;

		if (!SP.WriteData(sendcom)) std::cout << "Sending Failed" << std::endl;
		
	}
	Sleep(10000);
	SP.Reset();
	return 0;
}