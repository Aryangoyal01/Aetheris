#ifndef NETWORK_H
#define NETWORK_H

namespace EngineNetwork {

    /**
     * @brief Contains the telemetry state received from the Python tracking pipeline.
     */
    struct TrackingData {
        // Raw pixel coordinates from the camera matrix
        float x = 0.0f;
        float y = 0.0f;

        // Normalized coordinates (0.0f to 1.0f) for engine-agnostic scaling
        float normX = 0.0f;
        float normY = 0.0f;

        // Status flags
        bool isPinching = false;   // Gesture state
        bool handDetected = false; // Visibility state
        bool hasNewPacket = false; // Frame-specific signal to update logic
    };

    // --- API FUNCTIONS ---

    /**
     * @brief Initializes the network socket.
     * @param port The port to listen on (e.g., 8080).
     * @return True if successful.
     */
    bool Initialize(int port);

    /**
     * @brief Non-blocking poll for new tracking data.
     * @return The latest TrackingData snapshot.
     */
    TrackingData PollTrackingData();

    /**
     * @brief Closes sockets and cleans up networking resources.
     */
    void Shutdown();
}

#endif // NETWORK_H