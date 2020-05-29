#include "tsTransportStream.h"
#include <iostream>

using namespace std;

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================
void xTS_PacketHeader::Reset(){
    set_syncByte(0);
    set_transportErrorIndicator(false);
    set_payloadUnitStartIndicator(false);
    set_transportPriority(false);
    set_packetIdentifier(0);
    set_transportScramblingControl(0);
    set_adaptationFieldControl(0);
    set_continuityCounter(0);
}

int32_t xTS_PacketHeader::Parse(const uint8_t* Input){

    uint32_t newInput = Input[0] | (Input[1] << 8) | (Input[2] << 16) | (Input[3] << 24);
    uint32_t mainmask = 0x00000080;
    uint16_t localmask = 0x0080;

    uint8_t SB = get_syncByte();
    bool E = false;
    bool S = false;
    bool T = false;
    uint16_t PID = get_packetIdentifier();
    uint8_t TSC = get_transportScramblingControl();
    uint8_t AFC = get_adaptationFieldControl();
    uint8_t CC = get_continuityCounter();

    for(int i=0; i<8; i++){
        if((newInput&mainmask) > 0){
            SB = SB | localmask;
        }
        mainmask = mainmask >> 1;
        localmask = localmask >> 1;
    }

    mainmask = 0x00008000;
    if((newInput & mainmask) > 0){
        E = true;
    }
    mainmask = mainmask >> 1;
    if((newInput & mainmask) > 0){
        S = true;
    }
    mainmask = mainmask >> 1;
    if((newInput & mainmask) > 0){
        T = true;
    }
    mainmask = mainmask >> 1;

    localmask = 0x1000;
    for(int i=0; i<5; i++){
        if((newInput & mainmask) > 0){
            PID = PID | localmask;
        }
        mainmask = mainmask >> 1;
        localmask = localmask >> 1;
    }

    mainmask = 0x00800000;
    for(int i=0; i<8; i++){
        if((newInput & mainmask) > 0){
            PID = PID | localmask;
        }
        mainmask = mainmask >> 1;
        localmask = localmask >> 1;
    }

    localmask = 0x0002;
    mainmask = 0x80000000;

    for(int i=0; i<2; i++){
        if((newInput & mainmask) > 0){
            TSC = TSC | localmask;
        }
        mainmask = mainmask >> 1;
        localmask = localmask >> 1;
    }

    localmask = 0x0002;

    for(int i=0; i<2; i++){
        if((newInput & mainmask) > 0){
            AFC = AFC | localmask;
        }
        mainmask = mainmask >> 1;
        localmask = localmask >> 1;
    }

    localmask = 0x0008;

    for(int i=0; i<4; i++){
        if((newInput & mainmask) > 0){
            CC = CC | localmask;
        }
        mainmask = mainmask >> 1;
        localmask = localmask >> 1;
    }

    set_syncByte(SB);
    set_transportErrorIndicator(E);
    set_payloadUnitStartIndicator(S);
    set_transportPriority(T);
    set_packetIdentifier(PID);
    set_transportScramblingControl(TSC);
    set_adaptationFieldControl(AFC);
    set_continuityCounter(CC);


    /*set_syncByte(Input[0]);
    set_transportErrorIndicator((Input[1] & 128)? 1:0);
    set_payloadUnitStartIndicator((Input[1] & 64)? 1:0);
    set_transportPriority((Input[1] & 32)? 1:0);

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
    set_continuityCounter(CC);*/

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