#pragma once
// RadioHead Library
#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>

// Radio Config
#include "RadioOptions.h"

class RadioSensor
{
public:
    RadioSensor();
    void ReplyToRecievedMessageExample(String data);

private:
    RH_RF95 _rf95 = RH_RF95(1, 6);          //(1,6) needed when using RP2040-Zero board.
    RHMesh _mesh = RHMesh(_rf95, NODE_ID);
    void _sx1278_setup();
    uint8_t _incoming_buffer[RH_MESH_MAX_MESSAGE_LEN];
};