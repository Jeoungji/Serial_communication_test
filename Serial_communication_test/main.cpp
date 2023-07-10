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
	
	Recvcom recvcom;
	Sendcom sendcom;
	int x = 0;

	Sleep(100);
	while (SP.IsConnected())
	{
		int a = SP.ReadData(recvcom);
		if (a) {
			std::cout << "size : " << a << "  x: " << recvcom.x << "  y: " << recvcom.y;
			std::cout << "  swL" << recvcom.swL << "  swR" << recvcom.swR << std::endl;
		}
	}
	return 0;
}