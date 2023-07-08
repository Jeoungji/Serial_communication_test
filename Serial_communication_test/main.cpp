#include <stdio.h>
#include <tchar.h>
#include "Serial.h"	// Library described above
#include <string>
#include <iostream>





// application reads from the specified serial port and reports the collected data
int main()
{
	printf("Welcome to the serial test app!\n\n");

	Serial SP(8, "ATSAMD21G18");    // adjust as needed

	if (SP.IsConnected())
		std::cout << "Connetcted" << std::endl;

	//char incomingData[255];
	int dataLength = 255;
	int readResult = 0;
	
	Recvcom command;

	int x = 0;

	Sleep(100);
	while (SP.IsConnected())
	{
		//SP.WriteData("ON", dataLength);

		//Sleep(500);

		//SP.WriteData("OFF", dataLength);

		
		char incomingData[18] = "";


		readResult = SP.ReadData(incomingData, 4);
		if (readResult > sizeof(incomingData)) continue;

		unsigned int x, y, a, b = 0;

		if (readResult) {
			incomingData[0]--;
			incomingData[1]--;
			incomingData[2]--;
			incomingData[3]--;
			x = 0;
			y = 0;
			a = 0;
			b = 0;
			memcpy(&x, &incomingData[0], sizeof(char));
			memcpy(&y, &incomingData[1], sizeof(char));
			memcpy(&a, &incomingData[2], sizeof(char));
			memcpy(&b, &incomingData[3], sizeof(char));

			std::cout << "x : " << x << "  y : " << y << "  a : " << a << "  b : " << b << std::endl;
		}
	}
	return 0;
}