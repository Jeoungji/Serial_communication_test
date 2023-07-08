#pragma once
#ifndef SERIALCLASS_H_INCLUDED
#define SERIALCLASS_H_INCLUDED

#define ARDUINO_WAIT_TIME 2000
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

typedef struct Recvcom {    // size 48
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int d;
};

class Serial
{
private:
    //Serial comm handler
    HANDLE hSerial;
    //Connection status
    bool connected;
    //Get various information about the connection
    COMSTAT status;
    //Keep track of last error
    DWORD errors;
    unsigned short Port;
    std::string PortName;

    std::vector <char> buffer;
public:
    //Initialize Serial communication with the given COM port
    Serial(unsigned short port, std::string Name);
    //Close the connection
    ~Serial();

    bool Reset();
    //Read data in a buffer, if nbChar is greater than the
    //maximum number of bytes available, it will return only the
    //bytes available. The function return -1 when nothing could
    //be read, the number of bytes actually read.
    int ReadData(char* buffer, const unsigned int nbChar);
    //Writes data from a buffer through the Serial connection
    //return true on success.
    bool WriteData(const char* buffer, const unsigned int nbChar);
    //Check if we are actually connected
    bool IsConnected();

    int ReadData(Recvcom& data, const int datanum, const int nbChar);
};

#endif // SERIALCLASS_H_INCLUDED