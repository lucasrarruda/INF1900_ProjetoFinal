#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include <strsafe.h>
#include "../Core/CommunicationService.h"
#include "../Core/StructMessage.h"
#include <iostream>

#pragma comment(lib,"Core.lib")

using namespace Communication;
int main()
{

    // The main loop creates an instance of the named pipe and 
    // then waits for a client to connect to it. When the client 
    // connects, a thread is created to handle communications 
    // with that client, and this loop is free to wait for the
    // next client connect request. It is an infinite loop.

        CommunicationService communication;
        auto valuepassword = communication.GetCurrentPasswordConnection();
        std::wcout << "Current password:" << valuepassword << std::endl;
        auto getopenedchannel = communication.OpenCommunicationChannel(valuepassword);
        std::cout << "Opened channel: " << getopenedchannel << std::endl;
        StructMessage receivedvalue = communication.ReceiveDataFromPipe();
}