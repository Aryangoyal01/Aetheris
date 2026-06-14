#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include "network.h"
#include <iostream>

// File-local socket variables (hidden from the rest of the engine)
static SOCKET serverSocket = INVALID_SOCKET;
static bool networkReady = false;

namespace EngineNetwork
{

    bool Initialize(int port)
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "[Network] WSAStartup failed.\n";
            return false;
        }

        serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Assuming UDP for real-time tracking
        if (serverSocket == INVALID_SOCKET)
        {
            std::cerr << "[Network] Socket creation failed.\n";
            WSACleanup();
            return false;
        }

        // Set socket to non-blocking mode so it won't freeze your Raylib frame rate
        u_long mode = 1;
        ioctlsocket(serverSocket, FIONBIO, &mode);

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        {
            std::cerr << "[Network] Bind failed.\n";
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        networkReady = true;
        std::cout << "[Network] Pipeline listening on port " << port << "\n";
        return true;
    }

  
    TrackingData PollTrackingData()
    {
        TrackingData data;
        data.hasNewPacket = false; 
        if (!networkReady) return data;

        char buffer[256];
        sockaddr_in clientAddr;
        int clientLength = sizeof(clientAddr);
        int bytesReceived;

        // DRAIN THE BUFFER: Keep reading until there are no packets left.
        // This ensures 'data' always contains the absolute latest frame.
        while ((bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr *)&clientAddr, &clientLength)) > 0)
        {
            buffer[bytesReceived] = '\0';
            int handPresent = 0, pinchVal = 0;
            float px, py, nx, ny; 

            if (sscanf(buffer, "%d,%f,%f,%f,%f,%d", &handPresent, &px, &py, &nx, &ny, &pinchVal) == 6)
            {
                data.hasNewPacket = true; 
                data.handDetected = (handPresent == 1);
                data.x = px;
                data.y = py;
                data.normX = nx;
                data.normY = ny;
                data.isPinching = (pinchVal == 1);
            }
        }
        return data;
    }

    void Shutdown()
    {
        if (serverSocket != INVALID_SOCKET)
        {
            closesocket(serverSocket);
        }
        WSACleanup();
        std::cout << "[Network] Pipeline offline.\n";
    }
}