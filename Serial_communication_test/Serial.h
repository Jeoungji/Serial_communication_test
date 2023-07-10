#pragma once
#ifndef SERIALCLASS_H_INCLUDED
#define SERIALCLASS_H_INCLUDED

#define ARDUINO_WAIT_TIME 2000
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <array>

typedef struct Sendcom {    // size 48
    unsigned char right;
};

typedef struct Recvcom {
    unsigned int x; // two char , IMU x
    unsigned int y; // two char , IMU y
    unsigned int swL; // Right Request
    unsigned int swR; // fire Request
};

class Serial
{
private:
    HANDLE hSerial;         /*Serial comm handler*/
    bool connected;         /*Connection status*/
    COMSTAT status;         /*Get various information about the connection*/
    DWORD errors;           /*Keep track of last error*/
    unsigned short Port;    /*COM port number*/
    std::string PortName;   /*Naming port name*/
    std::array <unsigned char, 1> Sbuffer;
    std::array <unsigned char, 6> Rbuffer;
public:
    //Initialize Serial communication with the given COM port
    Serial(unsigned short port, std::string Name);
    //Close the connection
    ~Serial();

    bool Reset(); /*Reset Serial connect*/
    //Read data in a buffer, if nbChar is greater than the
    //maximum number of bytes available, it will return only the
    //bytes available. The function return -1 when nothing could
    //be read, the number of bytes actually read.
    int ReadData(char* buffer, const unsigned int nbChar); /*Original Serial Recving function*/

    //Writes data from a buffer through the Serial connection
    //return true on success.
    bool WriteData(const char* buffer, const unsigned int nbChar); /*Original Serial Sending function*/

    bool IsConnected(); /*Check the Serial port connected*/

    int ReadData(Recvcom& data); /*Read Serial data to recv struct type, return read buffer size*/

    bool WriteData(Sendcom& data); /*Send Serial data to Send struct type, return true on success*/
};

#endif // SERIALCLASS_H_INCLUDED