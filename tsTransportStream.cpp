#include "tsTransportStream.h"
#include <iostream>

using namespace std;

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================
void xTS_PacketHeader::Reset(){
    syncByte = 0;
    transportErrorIndicator = false;
    payloadUnitStartIndicator = false;
    transportPriority = false;
    packetIdentifier = 0;
    transportScramblingControl = 0;
    adaptationFieldControl = 0;
    continuityCounter = 0;
}

int32_t xTS_PacketHeader::Parse(const uint8_t* Input){
    set_syncByte(Input[0]);
    set_transportErrorIndicator((Input[1] & 128)? 1:0);
    set_payloadUnitStartIndicator((Input[1] & 64)? 1:0);
    set_transportPriority((Input[1] & 32)? 1:0);
    /*set_packetIdentifier(Input[1]&16 ? 4096:0 + Input[1]&8 ? 2048:0 + Input[1]&4 ? 1024:0 + Input[1]&2 ? 512:0 + Input[1]&1 ? 256:0 + Input[2]&128 ? 128:0 + Input[2]&64 ? 64:0 + Input[2]&32 ? 32:0 + Input[2]&16 ? 16:0 + Input[2]&8 ? 8:0 + Input[2]&4 ? 4:0 + Input[2]&2 ? 2:0 + Input[2]&1 ? 1:0);
    set_transportScramblingControl(Input[3]&128 ? 2:0 + Input[3]&64 ? 1:0);
    set_adaptationFieldControl(Input[3]&32 ? 2:0 + Input[3]&16 ? 1:0);
    set_continuityCounter(Input[3]&8 ? 8:0 + Input[3]&4 ? 4:0 + Input[3]&2 ? 2:0 + Input[3]&1 ? 1:0);*/

    uint16_t PID = Input[1]%32;
    PID <<= 8;
    PID |= Input[2];
    set_packetIdentifier(PID);

    uint8_t TSC = Input[3];
    TSC >>= 6;
    set_transportScramblingControl(TSC);

    uint8_t AFC = Input[3]%64;
    AFC >>= 4;
    set_adaptationFieldControl(AFC);

    uint8_t CC = Input[3]%16;
    set_continuityCounter(CC);

    return 1;
}

void xTS_PacketHeader::Print() const{
    printf("TS: SB=%d E=%d S=%d P=%d PID=%d TSC=%d AFC=%d CC=%d",
           get_syncByte(),
           get_transportErrorIndicator(),
           get_payloadUnitStartIndicator(),
           get_transportPriority(),
           get_packetIdentifier(),
           get_transportScramblingControl(),
           get_adaptationFieldControl(),
           get_continuityCounter());
}

bool xTS_PacketHeader::hasAdaptationField() const {
    return false;
}

bool xTS_PacketHeader::hasPayload() const {
    return false;
}

//GETTERS
const uint8_t &xTS_PacketHeader::get_syncByte() const{
    return syncByte;
}
const bool &xTS_PacketHeader::get_transportErrorIndicator() const{
    return transportErrorIndicator;
}
const bool &xTS_PacketHeader::get_payloadUnitStartIndicator() const{
    return payloadUnitStartIndicator;
}
const bool &xTS_PacketHeader::get_transportPriority() const{
    return transportPriority;
}
const uint16_t &xTS_PacketHeader::get_packetIdentifier() const{
    return packetIdentifier;
}
const uint8_t &xTS_PacketHeader::get_transportScramblingControl() const{
    return transportScramblingControl;
}
const uint8_t &xTS_PacketHeader::get_adaptationFieldControl() const{
    return adaptationFieldControl;
}
const uint8_t &xTS_PacketHeader::get_continuityCounter() const{
    return continuityCounter;
}

//SETTERS
void xTS_PacketHeader::set_syncByte(const uint8_t &temp){
    xTS_PacketHeader::syncByte = temp;
}
void xTS_PacketHeader::set_transportErrorIndicator(const bool &temp){
    xTS_PacketHeader::transportErrorIndicator = temp;
}
void xTS_PacketHeader::set_payloadUnitStartIndicator(const bool &temp){
    xTS_PacketHeader::payloadUnitStartIndicator = temp;
}
void xTS_PacketHeader::set_transportPriority(const bool &temp){
    xTS_PacketHeader::transportPriority = temp;
}
void xTS_PacketHeader::set_packetIdentifier(const uint16_t &temp){
    xTS_PacketHeader::packetIdentifier = temp;
}
void xTS_PacketHeader::set_transportScramblingControl(const uint8_t &temp){
    xTS_PacketHeader::transportScramblingControl = temp;
}
void xTS_PacketHeader::set_adaptationFieldControl(const uint8_t &temp){
    xTS_PacketHeader::adaptationFieldControl = temp;
}
void xTS_PacketHeader::set_continuityCounter(const uint8_t &temp){
    xTS_PacketHeader::continuityCounter = temp;
}
//=============================================================================================================================================================================