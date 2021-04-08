#include "controllers/sim-alfred/radio.h"

CRadio::CRadio() {
    isConnected = false;
}

bool CRadio::connectToServer(int idRobot) {
    if (isConnected) {
        return true;
    }
    sock = 0;
    isConnected = true;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DEFAULT_PORT + idRobot);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return false;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        isConnected = false;
    }

    return isConnected;
}


void CRadio::sendTelemetry(CVector3 pos, StateMode stateMode, float vBat) {
    // Unblock socket
    int flags = fcntl(sock, F_GETFL);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);


    struct PacketPosition packetPosition;
    packetPosition.x = pos.GetX();
    packetPosition.y = pos.GetY();
    packetPosition.z = pos.GetZ();
    packetPosition.packetType = position;
    send(sock, &packetPosition, sizeof(packetPosition), 0 );

    struct PacketVelocity packetVelocity;
    packetVelocity.packetType = velocity;
    /*packetVelocity.px = (posFinal.GetX() - posInitial.GetX());
    packetVelocity.py = (posFinal.GetY() - posInitial.GetY());
    packetVelocity.pz = (posFinal.GetZ() - posInitial.GetZ());*/
    send(sock, &packetVelocity, sizeof(packetVelocity), 0 );

    struct PacketDistance packetDistance;
    packetDistance.packetType = distance;
    packetDistance.front = 0;
    packetDistance.left = 0;
    packetDistance.right = 0;
    packetDistance.up = 0;
    packetDistance.back = 0;
    packetDistance.zrange = 0;
    send(sock, &packetDistance, sizeof(packetDistance), 0 );

    struct PacketTX packetTx;
    packetTx.packetType = tx;
    packetTx.isLedActivated = true;
    packetTx.vbat = vBat;
    packetTx.stateMode = stateMode;
    packetTx.rssiToBase = 0;
    send(sock, &packetTx, sizeof(packetTx), 0 );
}


StateMode* CRadio::ReceiveData() {
    char buffer[1024];
    StateMode* stateModeReceived = nullptr;
    if (recv(sock , buffer, sizeof(buffer), 0) != -1) {
        stateModeReceived = reinterpret_cast<StateMode*>(buffer);
    }
    return stateModeReceived;
}