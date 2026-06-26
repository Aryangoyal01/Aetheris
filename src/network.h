#pragma once

#include <cstdint>
#include <cstring>

namespace EngineNetwork {

    enum PacketType : uint8_t {
        PACKET_TRACKING = 0x01,
        PACKET_AI_CONFIG = 0x02,
        PACKET_SPAWN = 0x03,
    };

    struct PacketHeader {
        uint8_t version;
        uint8_t type;
        uint16_t size;
        uint32_t sequence;
    };

    struct TrackingData {
        float x = 0.0f;
        float y = 0.0f;
        float normX = 0.0f;
        float normY = 0.0f;
        bool isPinching = false;
        bool handDetected = false;
        bool hasNewPacket = false;
    };

    struct AIPayload {
        bool hasUpdate = false;
        float gravity = 9.81f;
        float friction = 0.15f;
        float elasticity = 0.75f;
        float timeScale = 1.0f;
        int spawnType = -1;
    };

    bool Initialize(int port);
    TrackingData PollTrackingData();
    AIPayload PollAIPayload();
    void Shutdown();
}
