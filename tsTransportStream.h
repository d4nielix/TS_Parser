#pragma once
#include "tsCommon.h"
#include <string>

using namespace std;

/*
MPEG-TS packet:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |                             Header                            | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   4 |                  Adaptation field + Payload                   | `
`     |                                                               | `
` 184 |                                                               | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `


MPEG-TS packet header:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |       SB      |E|S|T|           PID           |TSC|AFC|   CC  | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `

Sync byte                    (SB ) :  8 bits
Transport error indicator    (E  ) :  1 bit
Payload unit start indicator (S  ) :  1 bit
Transport priority           (T  ) :  1 bit
Packet Identifier            (PID) : 13 bits
Transport scrambling control (TSC) :  2 bits
Adaptation field control     (AFC) :  2 bits
Continuity counter           (CC ) :  4 bits
*/


//=============================================================================================================================================================================

class xTS
{
public:
    static constexpr uint32_t TS_PacketLength = 188;
    static constexpr uint32_t TS_HeaderLength = 4;

    static constexpr uint32_t PES_HeaderLength = 6;

    static constexpr uint32_t BaseClockFrequency_Hz         =    90000; //Hz
    static constexpr uint32_t ExtendedClockFrequency_Hz     = 27000000; //Hz
    static constexpr uint32_t BaseClockFrequency_kHz        =       90; //kHz
    static constexpr uint32_t ExtendedClockFrequency_kHz    =    27000; //kHz
    static constexpr uint32_t BaseToExtendedClockMultiplier =      300;
};

//=============================================================================================================================================================================

class xTS_PacketHeader
{
public:
    enum class ePID : uint16_t
    {
        PAT  = 0x0000,
        CAT  = 0x0001,
        TSDT = 0x0002,
        IPMT = 0x0003,
        NIT  = 0x0010, //DVB specific PID
        SDT  = 0x0011, //DVB specific PID
        NuLL = 0x1FFF,
    };

protected:
    uint8_t syncByte;
    bool transportErrorIndicator;
    bool payloadUnitStartIndicator;
    bool transportPriority;
    uint16_t packetIdentifier;
    uint8_t transportScramblingControl;
    uint8_t adaptationFieldControl;
    uint8_t continuityCounter;

public:
    void     Reset();
    int32_t  Parse(uint8_t* Input);
    int32_t  Parse2(uint8_t* Input);
    void     Print() const;
    bool hasAdaptationField() const;
    bool hasPayload() const;

    const uint8_t &get_syncByte() const;
    const bool &get_transportErrorIndicator() const;
    const bool &get_payloadUnitStartIndicator() const;
    const bool &get_transportPriority() const;
    const uint16_t &get_packetIdentifier() const;
    const uint8_t &get_transportScramblingControl() const;
    const uint8_t &get_adaptationFieldControl() const;
    const uint8_t &get_continuityCounter() const;

    void set_syncByte(const uint8_t &temp);
    void set_transportErrorIndicator(const bool &temp);
    void set_payloadUnitStartIndicator(const bool &temp);
    void set_transportPriority(const bool &temp);
    void set_packetIdentifier(const uint16_t &temp);
    void set_transportScramblingControl(const uint8_t &temp);
    void set_adaptationFieldControl(const uint8_t &temp);
    void set_continuityCounter(const uint8_t &temp);
};

//=============================================================================================================================================================================

class xTS_AdaptationField
{
protected:
    uint8_t adaptationFieldLength;
    bool discontinuityIndicator;
    bool randomAccessIndicator;
    bool elementaryStreamPriorityIndicator;
    bool programClockReferenceFlag;
    bool originalProgramClockReferenceFlag;
    bool splicingPointFlag;
    bool transportPrivateDataFlag;
    bool adaptationFieldExtensionFlag;

public:
    void Reset();
    int32_t Parse(const uint8_t* Input, uint8_t AdaptationFieldControl);
    void Print() const;
    uint32_t getNumBytes() const;

    const uint8_t &get_adaptationFieldLength() const;
    const bool &get_discontinuityIndicator() const;
    const bool &get_randomAccessIndicator() const;
    const bool &get_elementaryStreamPriorityIndicator() const;
    const bool &get_programClockReferenceFlag() const;
    const bool &get_originalProgramClockReferenceFlag() const;
    const bool &get_splicingPointFlag() const;
    const bool &get_transportPrivateDataFlag() const;
    const bool &get_adaptationFieldExtensionFlag() const;

    void set_adaptationFieldLength(const uint8_t &temp);
    void set_discontinuityIndicator(const bool &temp);
    void set_randomAccessIndicator(const bool &temp);
    void set_elementaryStreamPriorityIndicator(const bool &temp);
    void set_programClockReferenceFlag(const bool &temp);
    void set_originalProgramClockReferenceFlag(const bool &temp);
    void set_splicingPointFlag(const bool &temp);
    void set_transportPrivateDataFlag(const bool &temp);
    void set_adaptationFieldExtensionFlag(const bool &temp);
};

//=============================================================================================================================================================================

class xPES_PacketHeader
{
public:
    enum eStreamId : uint8_t
    {
        eStreamId_program_stream_map       = 0xBC,
        eStreamId_padding_stream           = 0xBE,
        eStreamId_private_stream_2         = 0xBF,
        eStreamId_ECM                      = 0xF0,
        eStreamId_EMM                      = 0xF1,
        eStreamId_program_stream_directory = 0xFF,
        eStreamId_DSMCC_stream             = 0xF2,
        eStreamId_ITUT_H222_1_type_E       = 0xF8,
    };

protected:
    uint32_t m_PacketStartCodePrefix;
    uint8_t m_StreamId;
    uint16_t m_PacketLength;
    uint8_t PESScramblingControl;
    bool PESPriority;
    bool dataAlignmentIndicator;
    bool copyright;
    bool originalOrCopy;
    uint8_t PTSDTSFlags;
    bool ESCRFlag;
    bool ESRateFlag;
    bool DSMTrickModeFlag;
    bool additionalCopyInfoFlag;
    bool PESCRCFlag;
    bool PESExtensionFlag;
    uint8_t PESHeaderDataLength;

public:
    void Reset();
    int32_t Parse(const uint8_t* Input);
    void Print() const;

    uint32_t getPacketStartCodePrefix() const { return m_PacketStartCodePrefix; }
    uint8_t getStreamId() const { return m_StreamId; }
    uint16_t getPacketLength() const { return m_PacketLength; }

    const uint8_t &get_PESScramblingControl() const;
    const bool &get_PESPriority() const;
    const bool &get_dataAlignmentIndicator() const;
    const bool &get_copyright() const;
    const bool &get_originalOrCopy() const;
    const uint8_t &get_PTSDTSFlags() const;
    const bool &get_ESCRFlag() const;
    const bool &get_ESRateFlag() const;
    const bool &get_DSMTrickModeFlag() const;
    const bool &get_additionalCopyInfoFlag() const;
    const bool &get_PESCRCFlag() const;
    const bool &get_PESExtensionFlag() const;
    const uint8_t &get_PESHeaderDataLength() const;

    void set_PacketStartCodePrefix(const uint32_t &temp);
    void set_StreamId(const uint8_t &temp);
    void set_PacketLength(const uint16_t &temp);
    void set_PESScramblingControl(const uint8_t &temp);
    void set_PESPriority(const bool &temp);
    void set_dataAlignmentIndicator(const bool &temp);
    void set_copyright(const bool &temp);
    void set_originalOrCopy(const bool &temp);
    void set_PTSDTSFlags(const uint8_t &temp);
    void set_ESCRFlag(const bool &temp);
    void set_ESRateFlag(const bool &temp);
    void set_DSMTrickModeFlag(const bool &temp);
    void set_additionalCopyInfoFlag(const bool &temp);
    void set_PESCRCFlag(const bool &temp);
    void set_PESExtensionFlag(const bool &temp);
    void set_PESHeaderDataLength(const uint8_t &temp);
};

//=============================================================================================================================================================================

class xPES_Assembler
{
public:
    enum class eResult : int32_t
    {
        UnexpectedPID = 1,
        StreamPacketLost = 2,
        AssemblingStarted = 3,
        AssemblingContinue = 4,
        AssemblingFinished = 5,
    };

protected:
    int32_t m_PID;
    uint8_t* m_Buffer;
    uint32_t m_BufferSize;
    uint32_t m_DataOffset;
    int8_t m_LastContinuityCounter;
    bool m_Started;
    xPES_PacketHeader m_PESH;
    uint8_t m_EndContinuityCounter;

public:
    xPES_Assembler();
    ~xPES_Assembler();

    void Init(int32_t PID);
    eResult AbsorbPacket(const uint8_t* TransportStreamPacket, const xTS_PacketHeader* PacketHeader, const xTS_AdaptationField* AdaptationField);

    void PrintPESH() const { m_PESH.Print(); }
    uint8_t* getPacket() { return m_Buffer; }
    int32_t getNumPacketBytes() const { return m_DataOffset; }

    void set_m_PID(int32_t mPID);

protected:
    void xBufferReset();
    void xBufferAppend(const uint8_t* Data, int32_t Size);
};