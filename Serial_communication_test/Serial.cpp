#include "Serial.h"
#include <string>
#include <iostream>
#define RESETCODE 8972

Serial::Serial(unsigned short port, std::string Name) /*COM port number*/
    : Port(port), PortName(Name)
{
    //const char* portName;
    std::string portname = "COM";
    portname = portname + std::to_string(Port);

    std::cout << "Connecting " << PortName << "Port Number : " << portname << std::endl;
    //We're not yet connected
    this->connected = false;

    //Try to connect to the given port throuh CreateFile
    this->hSerial = CreateFileA(portname.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    //Check if the connection was successfull
    if (this->hSerial == INVALID_HANDLE_VALUE)
    {
        //If not success full display an Error
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            //Print Error if neccessary
            std::cout << "ERROR: Handle was not attached. Reason: %s not available. COM" << Port << std::endl;
        }
        else
            std::cout << "ERROR!!!" << std::endl;
    }
    else
    {
        //If connected we try to set the comm parameters
        DCB dcbSerialParams = { 0 };

        //Try to get the current
        if (!GetCommState(this->hSerial, &dcbSerialParams))
        {
            //If impossible, show an error
            std::cout << "failed to get current serial parameters!" << std::endl;
        }
        else
        {
            //Define serial connection parameters for the arduino board
            dcbSerialParams.BaudRate = CBR_115200;
            dcbSerialParams.ByteSize = 8;
            dcbSerialParams.StopBits = ONESTOPBIT;
            dcbSerialParams.Parity = NOPARITY;
            //Setting the DTR to Control_Enable ensures that the Arduino is properly
            //reset upon establishing a connection
            dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

            //Set the parameters and check for their proper application
            if (!SetCommState(hSerial, &dcbSerialParams))
            {
                std::cout << "ALERT: Could not set Serial Port parameters" << std::endl;
            }
            else
            {
                //If everything went fine we're connected
                this->connected = true;
                //Flush any remaining characters in the buffers 
                PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
                //We wait 2s as the arduino board will be reseting
                Sleep(ARDUINO_WAIT_TIME);
            }
        }
    }
    std::cout << "buffer size : " << Rbuffer.size() << std::endl;
}

Serial::~Serial()
{
    //Check if we are connected before trying to disconnect
    if (this->connected)
    {
        //We're no longer connected
        this->connected = false;
        //Close the serial handler
        CloseHandle(this->hSerial);
    }
}

bool Serial::Reset() {

    if (this->connected)
    {
        //We're no longer connected
        this->connected = false;
        //Close the serial handler
        CloseHandle(this->hSerial);
    }

    //const char* portName;
    std::string portname = "COM";
    portname = portname + std::to_string(Port);

    std::cout << "Connecting " << PortName << "Port Number : " << portname << std::endl;
    //We're not yet connected
    this->connected = false;

    //Try to connect to the given port throuh CreateFile
    this->hSerial = CreateFileA(portname.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    //Check if the connection was successfull
    if (this->hSerial == INVALID_HANDLE_VALUE)
    {
        //If not success full display an Error
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            //Print Error if neccessary
            std::cout << "ERROR: Handle was not attached. Reason: %s not available. COM" << Port << std::endl;
        }
        else
            std::cout << "ERROR" << std::endl;
        exit(0);
    }
    else
    {
        //If connected we try to set the comm parameters
        DCB dcbSerialParams = { 0 };

        //Try to get the current
        if (!GetCommState(this->hSerial, &dcbSerialParams))
        {
            //If impossible, show an error
            std::cout << "failed to get current serial parameters!" << std::endl;
        }
        else
        {
            //Define serial connection parameters for the arduino board
            dcbSerialParams.BaudRate = CBR_115200;
            dcbSerialParams.ByteSize = 8;
            dcbSerialParams.StopBits = ONESTOPBIT;
            dcbSerialParams.Parity = NOPARITY;
            //Setting the DTR to Control_Enable ensures that the Arduino is properly
            //reset upon establishing a connection
            dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

            //Set the parameters and check for their proper application
            if (!SetCommState(hSerial, &dcbSerialParams))
            {
                std::cout << "ALERT: Could not set Serial Port parameters" << std::endl;
            }
            else
            {
                //If everything went fine we're connected
                this->connected = true;
                //Flush any remaining characters in the buffers 
                PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
                //We wait 2s as the arduino board will be reseting
                Sleep(ARDUINO_WAIT_TIME);
            }

        }
    }
    return true;
}
int Serial::ReadData(char* buffers, const unsigned int nbChar)
{
    //Number of bytes we'll have read
    DWORD bytesRead;
    //Number of bytes we'll really ask to read
    unsigned int toRead = 0;

    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(this->hSerial, &this->errors, &this->status);

    //Check if there is something to read
    if (this->status.cbInQue > (nbChar - 1))
    {
        //If there is we check if there is enough data to read the required number
        //of characters, if not we'll read only the available characters to prevent
        //locking of the application.

        if (this->status.cbInQue > nbChar)
        {
            toRead = nbChar;
            std::cout << "active if" << std::endl;
        }
        else
        {
            toRead = this->status.cbInQue;
            std::cout << "active else" << std::endl;
        }

        //Try to read the require number of chars, and return the number of read bytes on success

        if (ReadFile(this->hSerial, buffers, nbChar, &bytesRead, NULL))
        {
            return bytesRead;
        }
    }
    //If nothing has been read, or that an error was detected return 0
    return 0;
}


bool Serial::WriteData(const char* buffer, const unsigned int nbChar)
{
    DWORD bytesSend;

    //Try to write the buffer on the Serial port
    if (!WriteFile(this->hSerial, (void*)buffer, nbChar, &bytesSend, 0))
    {
        //In case it don't work get comm error and return false
        ClearCommError(this->hSerial, &this->errors, &this->status);

        return false;
    }
    else
        return true;
}

bool Serial::IsConnected()
{
    //Simply return the connection status
    return this->connected;
}

int Serial::ReadData(Recvcom& data) {
    //Number of bytes we'll have read
    DWORD bytesRead;
    //Number of bytes we'll really ask to read
    unsigned int toRead = 0;

    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(this->hSerial, &this->errors, &this->status);

    //Check if there is something to read
    if (this->status.cbInQue > (Rbuffer.size() - 1))
    {
        //If there is we check if there is enough data to read the required number
        //of characters, if not we'll read only the available characters to prevent
        //locking of the application.

        if (this->status.cbInQue > Rbuffer.size())
        {
            toRead = Rbuffer.size();
            std::cout << "active if" << std::endl;
        }
        else
        {
            toRead = this->status.cbInQue;
            std::cout << "active else" << std::endl;
        }

        //Try to read the require number of chars, and return the number of read bytes on success

        if (ReadFile(this->hSerial, &Rbuffer.data()[0], Rbuffer.size(), &bytesRead, NULL))
        {
            //Eliminate Bias
            for (int i = 0; i < Rbuffer.size(); i++)
                Rbuffer[i]--;

            //Decoding
            data.x = (unsigned int)Rbuffer[5] * 254 + (unsigned int)Rbuffer[4];
            data.y = (unsigned int)Rbuffer[3] * 254 + (unsigned int)Rbuffer[2];
            std::cout << "RBuffer : " << (int)Rbuffer[0] << (int)Rbuffer[1] << std::endl;
            data.swL = Rbuffer[1];
            data.swR = Rbuffer[0];
            return bytesRead;
        }
    }
    //If nothing has been read, or that an error was detected return 0
    return 0;
}

bool Serial::WriteData(Sendcom& data) {
    DWORD bytesSend;

    // Encoding
    Sbuffer[0] = data.right;

    // Bias
    for (int i = 0; i < Sbuffer.size(); i++)
        Sbuffer[i]++;

    //Try to write the buffer on the Serial port
    if (!WriteFile(this->hSerial, Sbuffer.data(), Sbuffer.size(), &bytesSend, 0))
    {
        //In case it don't work get comm error and return false
        ClearCommError(this->hSerial, &this->errors, &this->status);

        return false;
    }
    else
        return true;
}