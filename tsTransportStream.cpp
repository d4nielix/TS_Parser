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

int32_t xTS_PacketHeader::Parse(uint8_t* Input){

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

    return 1;
}

int32_t xTS_PacketHeader::Parse2(uint8_t* Input){

    set_syncByte(Input[0]);
    set_transportErrorIndicator((Input[1] & 128) != 0);
    set_payloadUnitStartIndicator((Input[1] & 64) != 0);
    set_transportPriority((Input[1] & 32) != 0);

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
    uint8_t AFC = get_adaptationFieldControl();
    return AFC == 2 || AFC == 3;
}

bool xTS_PacketHeader::hasPayload() const {
    return get_adaptationFieldControl() == 3;
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
    syncByte = temp;
}
void xTS_PacketHeader::set_transportErrorIndicator(const bool &temp){
    transportErrorIndicator = temp;
}
void xTS_PacketHeader::set_payloadUnitStartIndicator(const bool &temp){
    payloadUnitStartIndicator = temp;
}
void xTS_PacketHeader::set_transportPriority(const bool &temp){
    transportPriority = temp;
}
void xTS_PacketHeader::set_packetIdentifier(const uint16_t &temp){
    packetIdentifier = temp;
}
void xTS_PacketHeader::set_transportScramblingControl(const uint8_t &temp){
    transportScramblingControl = temp;
}
void xTS_PacketHeader::set_adaptationFieldControl(const uint8_t &temp){
    adaptationFieldControl = temp;
}
void xTS_PacketHeader::set_continuityCounter(const uint8_t &temp){
    continuityCounter = temp;
}

//=============================================================================================================================================================================
// xTS_AdaptationField
//=============================================================================================================================================================================
void xTS_AdaptationField::Reset() {
    set_adaptationFieldLength(0);
    set_discontinuityIndicator(false);
    set_randomAccessIndicator(false);
    set_elementaryStreamPriorityIndicator(false);
    set_programClockReferenceFlag(false);
    set_originalProgramClockReferenceFlag(false);
    set_splicingPointFlag(false);
    set_transportPrivateDataFlag(false);
    set_adaptationFieldExtensionFlag(false);
}

int32_t xTS_AdaptationField::Parse(const uint8_t *Input, uint8_t AdaptationFieldControl) {
    set_adaptationFieldLength(Input[4]);
    set_discontinuityIndicator((Input[5] & 128) != 0);
    set_randomAccessIndicator((Input[5] & 64) != 0);
    set_elementaryStreamPriorityIndicator((Input[5] & 32) != 0);
    set_programClockReferenceFlag((Input[5] & 16) != 0);
    set_originalProgramClockReferenceFlag((Input[5] & 8) != 0);
    set_splicingPointFlag((Input[5] & 4) != 0);
    set_transportPrivateDataFlag((Input[5] & 2) != 0);
    set_adaptationFieldExtensionFlag((Input[5] & 1) != 0);

    return 1;
}

void xTS_AdaptationField::Print() const {
    printf(" AF: L=%d DC=%d RA=%d ESP=%d PR=%d OR=%d SP=%d TP=%d EX=%d",
            get_adaptationFieldLength(),
            get_discontinuityIndicator(),
            get_randomAccessIndicator(),
            get_elementaryStreamPriorityIndicator(),
            get_programClockReferenceFlag(),
            get_originalProgramClockReferenceFlag(),
            get_splicingPointFlag(),
            get_transportPrivateDataFlag(),
            get_adaptationFieldExtensionFlag());
}

uint32_t xTS_AdaptationField::getNumBytes() const {
    return 0;
}

//GETTERS
const uint8_t &xTS_AdaptationField::get_adaptationFieldLength() const {
    return adaptationFieldLength;
}
const bool &xTS_AdaptationField::get_discontinuityIndicator() const {
    return discontinuityIndicator;
}
const bool &xTS_AdaptationField::get_randomAccessIndicator() const {
    return randomAccessIndicator;
}
const bool &xTS_AdaptationField::get_elementaryStreamPriorityIndicator() const {
    return elementaryStreamPriorityIndicator;
}
const bool &xTS_AdaptationField::get_programClockReferenceFlag() const {
    return programClockReferenceFlag;
}
const bool &xTS_AdaptationField::get_originalProgramClockReferenceFlag() const {
    return originalProgramClockReferenceFlag;
}
const bool &xTS_AdaptationField::get_splicingPointFlag() const {
    return splicingPointFlag;
}
const bool &xTS_AdaptationField::get_transportPrivateDataFlag() const {
    return transportPrivateDataFlag;
}
const bool &xTS_AdaptationField::get_adaptationFieldExtensionFlag() const {
    return adaptationFieldExtensionFlag;
}

//SETTERS
void xTS_AdaptationField::set_adaptationFieldLength(const uint8_t &temp) {
    adaptationFieldLength = temp;
}
void xTS_AdaptationField::set_discontinuityIndicator(const bool &temp) {
    discontinuityIndicator = temp;
}
void xTS_AdaptationField::set_randomAccessIndicator(const bool &temp) {
    randomAccessIndicator = temp;
}
void xTS_AdaptationField::set_elementaryStreamPriorityIndicator(const bool &temp) {
    elementaryStreamPriorityIndicator = temp;
}
void xTS_AdaptationField::set_programClockReferenceFlag(const bool &temp) {
    programClockReferenceFlag = temp;
}
void xTS_AdaptationField::set_originalProgramClockReferenceFlag(const bool &temp) {
    originalProgramClockReferenceFlag = temp;
}
void xTS_AdaptationField::set_splicingPointFlag(const bool &temp) {
    splicingPointFlag = temp;
}
void xTS_AdaptationField::set_transportPrivateDataFlag(const bool &temp) {
    transportPrivateDataFlag = temp;
}
void xTS_AdaptationField::set_adaptationFieldExtensionFlag(const bool &temp) {
    adaptationFieldExtensionFlag = temp;
}

//=============================================================================================================================================================================
// xPES_PacketHeader
//=============================================================================================================================================================================
void xPES_PacketHeader::Reset() {
    set_PacketStartCodePrefix(0);
    set_StreamId(0);
    set_PacketLength(0);
    set_PESScramblingControl(0);
    set_PESPriority(false);
    set_dataAlignmentIndicator(false);
    set_copyright(false);
    set_originalOrCopy(false);
    set_PTSDTSFlags(0);
    set_ESCRFlag(false);
    set_ESRateFlag(false);
    set_DSMTrickModeFlag(false);
    set_additionalCopyInfoFlag(false);
    set_PESCRCFlag(false);
    set_PESExtensionFlag(false);
    set_PESHeaderDataLength(0);
}

int32_t xPES_PacketHeader::Parse(const uint8_t *Input) {
    return 1;
}

void xPES_PacketHeader::Print() const {
    printf(" PES: PSCP=%d SID=%d PL=%d HL=%lu DL=%lu",
            getPacketStartCodePrefix(),
            getStreamId(),
            getPacketLength(),
            get_PESHeaderDataLength() + xTS::TS_HeaderLength + 3,
            getPacketLength() - get_PESHeaderDataLength() - 3);
}

//GETTERS
const uint8_t &xPES_PacketHeader::get_PESScramblingControl() const {
    return PESScramblingControl;
}
const bool &xPES_PacketHeader::get_PESPriority() const {
    return PESPriority;
}
const bool &xPES_PacketHeader::get_dataAlignmentIndicator() const {
    return dataAlignmentIndicator;
}
const bool &xPES_PacketHeader::get_copyright() const {
    return copyright;
}
const bool &xPES_PacketHeader::get_originalOrCopy() const {
    return originalOrCopy;
}
const uint8_t &xPES_PacketHeader::get_PTSDTSFlags() const {
    return PTSDTSFlags;
}
const bool &xPES_PacketHeader::get_ESCRFlag() const {
    return ESCRFlag;
}
const bool &xPES_PacketHeader::get_ESRateFlag() const {
    return ESRateFlag;
}
const bool &xPES_PacketHeader::get_DSMTrickModeFlag() const {
    return DSMTrickModeFlag;
}
const bool &xPES_PacketHeader::get_additionalCopyInfoFlag() const {
    return additionalCopyInfoFlag;
}
const bool &xPES_PacketHeader::get_PESCRCFlag() const {
    return PESCRCFlag;
}
const bool &xPES_PacketHeader::get_PESExtensionFlag() const {
    return PESExtensionFlag;
}
const uint8_t &xPES_PacketHeader::get_PESHeaderDataLength() const {
    return PESHeaderDataLength;
}

//SETTERS
void xPES_PacketHeader::set_PacketStartCodePrefix(const uint32_t &temp) {
    m_PacketStartCodePrefix = temp;
}
void xPES_PacketHeader::set_StreamId(const uint8_t &temp) {
    m_StreamId = temp;
}
void xPES_PacketHeader::set_PacketLength(const uint16_t &temp) {
    m_PacketLength = temp;
}
void xPES_PacketHeader::set_PESScramblingControl(const uint8_t &temp) {
    PESScramblingControl = temp;
}
void xPES_PacketHeader::set_PESPriority(const bool &temp) {
    PESPriority = temp;
}
void xPES_PacketHeader::set_dataAlignmentIndicator(const bool &temp) {
    dataAlignmentIndicator = temp;
}
void xPES_PacketHeader::set_copyright(const bool &temp) {
    copyright = temp;
}
void xPES_PacketHeader::set_originalOrCopy(const bool &temp) {
    originalOrCopy = temp;
}
void xPES_PacketHeader::set_PTSDTSFlags(const uint8_t &temp) {
    PTSDTSFlags = temp;
}
void xPES_PacketHeader::set_ESCRFlag(const bool &temp) {
    ESCRFlag = temp;
}
void xPES_PacketHeader::set_ESRateFlag(const bool &temp) {
    ESRateFlag = temp;
}
void xPES_PacketHeader::set_DSMTrickModeFlag(const bool &temp) {
    DSMTrickModeFlag = temp;
}
void xPES_PacketHeader::set_additionalCopyInfoFlag(const bool &temp) {
    additionalCopyInfoFlag = temp;
}
void xPES_PacketHeader::set_PESCRCFlag(const bool &temp) {
    PESCRCFlag = temp;
}
void xPES_PacketHeader::set_PESExtensionFlag(const bool &temp) {
    PESExtensionFlag = temp;
}
void xPES_PacketHeader::set_PESHeaderDataLength(const uint8_t &temp) {
    PESHeaderDataLength = temp;
}

//=============================================================================================================================================================================
// xPES_Assembler
//=============================================================================================================================================================================
