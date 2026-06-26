#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include "network.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

static SOCKET serverSocket = INVALID_SOCKET;
static bool networkReady = false;
static uint32_t sequenceCounter = 0;

namespace EngineNetwork {

    bool Initialize(int port) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "[Network] WSAStartup failed.\n";
            return false;
        }

        serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "[Network] Socket creation failed.\n";
            WSACleanup();
            return false;
        }

        u_long mode = 1;
        ioctlsocket(serverSocket, FIONBIO, &mode);

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "[Network] Bind failed.\n";
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        networkReady = true;
        std::cout << "[Network] Pipeline listening on port " << port << "\n";
        return true;
    }

    static bool ParseTrackingCSV(const char* data, TrackingData& out) {
        int handPresent = 0, pinchVal = 0;
        float px, py, nx, ny;
        if (sscanf(data, "%d,%f,%f,%f,%f,%d", &handPresent, &px, &py, &nx, &ny, &pinchVal) == 6) {
            out.hasNewPacket = true;
            out.handDetected = (handPresent == 1);
            out.x = px;
            out.y = py;
            out.normX = nx;
            out.normY = ny;
            out.isPinching = (pinchVal == 1);
            return true;
        }
        return false;
    }

    static float ParseJSONFloat(const char* json, const char* key, float defaultVal) {
        char search[64];
        snprintf(search, sizeof(search), "\"%s\"", key);
        const char* pos = strstr(json, search);
        if (!pos) return defaultVal;
        pos += strlen(search);
        while (*pos == ' ' || *pos == ':' || *pos == '\t') pos++;
        return (float)atof(pos);
    }

    static int ParseJSONInt(const char* json, const char* key, int defaultVal) {
        char search[64];
        snprintf(search, sizeof(search), "\"%s\"", key);
        const char* pos = strstr(json, search);
        if (!pos) return defaultVal;
        pos += strlen(search);
        while (*pos == ' ' || *pos == ':' || *pos == '\t') pos++;
        if (*pos == '"') {
            pos++;
            if (strncmp(pos, "SINGULARITY", 11) == 0) return 0;
            if (strncmp(pos, "REPELLER", 8) == 0) return 1;
            if (strncmp(pos, "PULSAR", 6) == 0) return 2;
            return defaultVal;
        }
        return atoi(pos);
    }

    static bool ParseAIPacket(const char* data, AIPayload& out) {
        if (data[0] != '{') return false;

        out.hasUpdate = true;
        out.gravity = ParseJSONFloat(data, "gravity", out.gravity);
        out.friction = ParseJSONFloat(data, "friction", out.friction);
        out.elasticity = ParseJSONFloat(data, "elasticity", out.elasticity);
        out.timeScale = ParseJSONFloat(data, "time_scale", out.timeScale);
        out.spawnType = ParseJSONInt(data, "spawn_particles", out.spawnType);
        return true;
    }

    TrackingData PollTrackingData() {
        TrackingData data;
        data.hasNewPacket = false;
        if (!networkReady) return data;

        char buffer[1024];
        sockaddr_in clientAddr;
        int clientLength = sizeof(clientAddr);

        while (true) {
            int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0,
                                        (sockaddr*)&clientAddr, &clientLength);
            if (bytesReceived <= 0) break;
            buffer[bytesReceived] = '\0';

            if (bytesReceived >= (int)sizeof(PacketHeader)) {
                PacketHeader header;
                memcpy(&header, buffer, sizeof(PacketHeader));

                if (header.version == 1) {
                    const char* payload = buffer + sizeof(PacketHeader);

                    if (header.type == PACKET_TRACKING) {
                        ParseTrackingCSV(payload, data);
                    }
                }
            } else {
                ParseTrackingCSV(buffer, data);
            }
        }
        return data;
    }

    AIPayload PollAIPayload() {
        AIPayload payload;
        payload.hasUpdate = false;
        if (!networkReady) return payload;

        char buffer[1024];
        sockaddr_in clientAddr;
        int clientLength = sizeof(clientAddr);

        while (true) {
            int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0,
                                        (sockaddr*)&clientAddr, &clientLength);
            if (bytesReceived <= 0) break;
            buffer[bytesReceived] = '\0';

            if (bytesReceived >= (int)sizeof(PacketHeader)) {
                PacketHeader header;
                memcpy(&header, buffer, sizeof(PacketHeader));

                if (header.version == 1 && header.type == PACKET_AI_CONFIG) {
                    const char* jsonData = buffer + sizeof(PacketHeader);
                    ParseAIPacket(jsonData, payload);
                }
            }
        }
        return payload;
    }

    void Shutdown() {
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
        }
        WSACleanup();
        std::cout << "[Network] Pipeline offline.\n";
    }
}
