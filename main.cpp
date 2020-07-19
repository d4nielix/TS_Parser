#pragma once
#include <cstdint>
#include <cinttypes>
#include <cfloat>
#include <climits>
#include <cstddef>

#define NOT_VALID  -1

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || defined(_M_IX86))
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif

//=============================================================================================================================================================================
// Byte swap
//=============================================================================================================================================================================
#if defined(_MSC_VER)
static inline uint16_t xSwapBytes16(uint16_t Value) { return _byteswap_ushort(Value); }
static inline  int16_t xSwapBytes16(int16_t Value) { return _byteswap_ushort(Value); }
static inline uint32_t xSwapBytes32(uint32_t Value) { return _byteswap_ulong(Value); }
static inline  int32_t xSwapBytes32(int32_t Value) { return _byteswap_ulong(Value); }
static inline uint64_t xSwapBytes64(uint64_t Value) { return _byteswap_uint64(Value); }
static inline  int64_t xSwapBytes64(int64_t Value) { return _byteswap_uint64(Value); }
#elif defined (__GNUC__)
static inline uint16_t xSwapBytes16(uint16_t Value) { return __builtin_bswap16(Value); }
static inline  int16_t xSwapBytes16(int16_t Value) { return __builtin_bswap16(Value); }
static inline uint32_t xSwapBytes32(uint32_t Value) { return __builtin_bswap32(Value); }
static inline  int32_t xSwapBytes32(int32_t Value) { return __builtin_bswap32(Value); }
static inline uint64_t xSwapBytes64(uint64_t Value) { return __builtin_bswap64(Value); }
static inline  int64_t xSwapBytes64(int64_t Value) { return __builtin_bswap64(Value); }
#else
#error Unrecognized compiler
#endif

#pragma once
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

	static constexpr uint32_t BaseClockFrequency_Hz = 90000; //Hz
	static constexpr uint32_t ExtendedClockFrequency_Hz = 27000000; //Hz
	static constexpr uint32_t BaseClockFrequency_kHz = 90; //kHz
	static constexpr uint32_t ExtendedClockFrequency_kHz = 27000; //kHz
	static constexpr uint32_t BaseToExtendedClockMultiplier = 300;
};

//=============================================================================================================================================================================

class xTS_PacketHeader
{
public:
	enum class ePID : uint16_t
	{
		PAT = 0x0000,
		CAT = 0x0001,
		TSDT = 0x0002,
		IPMT = 0x0003,
		NIT = 0x0010, //DVB specific PID
		SDT = 0x0011, //DVB specific PID
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
		eStreamId_program_stream_map = 0xBC,
		eStreamId_padding_stream = 0xBE,
		eStreamId_private_stream_2 = 0xBF,
		eStreamId_ECM = 0xF0,
		eStreamId_EMM = 0xF1,
		eStreamId_program_stream_directory = 0xFF,
		eStreamId_DSMCC_stream = 0xF2,
		eStreamId_ITUT_H222_1_type_E = 0xF8,
	};

protected:
	uint32_t m_PacketStartCodePrefix;
	uint8_t m_StreamId;
	uint16_t m_PacketLength;
	uint32_t m_DataLength;
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
	uint32_t getDataLength() const { return m_DataLength; }

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

#include <iostream>

using namespace std;

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================
void xTS_PacketHeader::Reset() {
	set_syncByte(0);
	set_transportErrorIndicator(false);
	set_payloadUnitStartIndicator(false);
	set_transportPriority(false);
	set_packetIdentifier(0);
	set_transportScramblingControl(0);
	set_adaptationFieldControl(0);
	set_continuityCounter(0);
}

int32_t xTS_PacketHeader::Parse(uint8_t* Input) {

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

	for (int i = 0; i < 8; i++) {
		if ((newInput&mainmask) > 0) {
			SB = SB | localmask;
		}
		mainmask = mainmask >> 1;
		localmask = localmask >> 1;
	}

	mainmask = 0x00008000;
	if ((newInput & mainmask) > 0) {
		E = true;
	}
	mainmask = mainmask >> 1;
	if ((newInput & mainmask) > 0) {
		S = true;
	}
	mainmask = mainmask >> 1;
	if ((newInput & mainmask) > 0) {
		T = true;
	}
	mainmask = mainmask >> 1;

	localmask = 0x1000;
	for (int i = 0; i < 5; i++) {
		if ((newInput & mainmask) > 0) {
			PID = PID | localmask;
		}
		mainmask = mainmask >> 1;
		localmask = localmask >> 1;
	}

	mainmask = 0x00800000;
	for (int i = 0; i < 8; i++) {
		if ((newInput & mainmask) > 0) {
			PID = PID | localmask;
		}
		mainmask = mainmask >> 1;
		localmask = localmask >> 1;
	}

	localmask = 0x0002;
	mainmask = 0x80000000;

	for (int i = 0; i < 2; i++) {
		if ((newInput & mainmask) > 0) {
			TSC = TSC | localmask;
		}
		mainmask = mainmask >> 1;
		localmask = localmask >> 1;
	}

	localmask = 0x0002;

	for (int i = 0; i < 2; i++) {
		if ((newInput & mainmask) > 0) {
			AFC = AFC | localmask;
		}
		mainmask = mainmask >> 1;
		localmask = localmask >> 1;
	}

	localmask = 0x0008;

	for (int i = 0; i < 4; i++) {
		if ((newInput & mainmask) > 0) {
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

int32_t xTS_PacketHeader::Parse2(uint8_t* Input) {

	set_syncByte(Input[0]);
	set_transportErrorIndicator((Input[1] & 128) != 0);
	set_payloadUnitStartIndicator((Input[1] & 64) != 0);
	set_transportPriority((Input[1] & 32) != 0);

	uint16_t PID = Input[1] % 32;
	PID <<= 8;
	PID |= Input[2];
	set_packetIdentifier(PID);

	uint8_t TSC = Input[3];
	TSC >>= 6;
	set_transportScramblingControl(TSC);

	uint8_t AFC = Input[3] % 64;
	AFC >>= 4;
	set_adaptationFieldControl(AFC);

	uint8_t CC = Input[3] % 16;
	set_continuityCounter(CC);

	return 1;
}

void xTS_PacketHeader::Print() const {
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
const uint8_t &xTS_PacketHeader::get_syncByte() const {
	return syncByte;
}
const bool &xTS_PacketHeader::get_transportErrorIndicator() const {
	return transportErrorIndicator;
}
const bool &xTS_PacketHeader::get_payloadUnitStartIndicator() const {
	return payloadUnitStartIndicator;
}
const bool &xTS_PacketHeader::get_transportPriority() const {
	return transportPriority;
}
const uint16_t &xTS_PacketHeader::get_packetIdentifier() const {
	return packetIdentifier;
}
const uint8_t &xTS_PacketHeader::get_transportScramblingControl() const {
	return transportScramblingControl;
}
const uint8_t &xTS_PacketHeader::get_adaptationFieldControl() const {
	return adaptationFieldControl;
}
const uint8_t &xTS_PacketHeader::get_continuityCounter() const {
	return continuityCounter;
}

//SETTERS
void xTS_PacketHeader::set_syncByte(const uint8_t &temp) {
	syncByte = temp;
}
void xTS_PacketHeader::set_transportErrorIndicator(const bool &temp) {
	transportErrorIndicator = temp;
}
void xTS_PacketHeader::set_payloadUnitStartIndicator(const bool &temp) {
	payloadUnitStartIndicator = temp;
}
void xTS_PacketHeader::set_transportPriority(const bool &temp) {
	transportPriority = temp;
}
void xTS_PacketHeader::set_packetIdentifier(const uint16_t &temp) {
	packetIdentifier = temp;
}
void xTS_PacketHeader::set_transportScramblingControl(const uint8_t &temp) {
	transportScramblingControl = temp;
}
void xTS_PacketHeader::set_adaptationFieldControl(const uint8_t &temp) {
	adaptationFieldControl = temp;
}
void xTS_PacketHeader::set_continuityCounter(const uint8_t &temp) {
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
	return get_adaptationFieldLength();
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
	m_DataLength = 0;
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
	uint32_t PSCP = Input[0];
	PSCP <<= 8;
	PSCP |= Input[1];
	PSCP <<= 8;
	PSCP |= Input[2];
	set_PacketStartCodePrefix(PSCP);

	set_StreamId(Input[3]);
	uint8_t stream = getStreamId();

	uint16_t PL = Input[4];
	PL <<= 8;
	PL |= Input[5];
	set_PacketLength(PL);


	if (stream != eStreamId::eStreamId_program_stream_map
		&& stream != eStreamId::eStreamId_padding_stream
		&& stream != eStreamId::eStreamId_private_stream_2
		&& stream != eStreamId::eStreamId_ECM
		&& stream != eStreamId::eStreamId_EMM
		&& stream != eStreamId::eStreamId_program_stream_directory
		&& stream != eStreamId::eStreamId_DSMCC_stream
		&& stream != eStreamId::eStreamId_ITUT_H222_1_type_E) {

		set_PESScramblingControl(Input[6] % 32 ? 2 : 0 + Input[6] % 16 ? 1 : 0);
		set_PESPriority(Input[6] % 8 != 0);
		set_dataAlignmentIndicator(Input[6] % 4 != 0);
		set_copyright(Input[6] % 2 != 0);
		set_originalOrCopy(Input[6] % 1 != 0);

		set_PTSDTSFlags(Input[7] % 128 ? 2 : 0 + Input[7] % 64 ? 1 : 0);
		set_ESCRFlag(Input[7] % 32 != 0);
		set_ESRateFlag(Input[7] % 16 != 0);
		set_DSMTrickModeFlag(Input[7] % 8 != 0);
		set_additionalCopyInfoFlag(Input[7] % 4 != 0);
		set_PESCRCFlag(Input[7] % 2 != 0);
		set_PESExtensionFlag(Input[7] % 1 != 0);

		set_PESHeaderDataLength(Input[8]);
	}

	set_PESHeaderDataLength(get_PESHeaderDataLength() + 9);
	m_DataLength = getPacketLength() - get_PESHeaderDataLength();

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
xPES_Assembler::xPES_Assembler() {

}
xPES_Assembler::~xPES_Assembler() {

}

void xPES_Assembler::Init(int32_t PID) {
	set_m_PID(PID);
}

void xPES_Assembler::set_m_PID(int32_t mPID) {
	m_PID = mPID;
}

void xPES_Assembler::xBufferReset() {
	m_Started = false;
	m_LastContinuityCounter = 0;
	m_EndContinuityCounter = 15;
	m_BufferSize = 0;
	m_DataOffset = 0;
	m_Buffer = 0;

	m_PESH.Reset();
}

void xPES_Assembler::xBufferAppend(const uint8_t * Data, int32_t Size)
{
	for (uint8_t i = 188 - Size; i < 188; i++)
	{
		m_Buffer[m_DataOffset++] = Data[i];
	}
}

/*xPES_Assembler::eResult xPES_Assembler::AbsorbPacket(const uint8_t *TransportStreamPacket, const xTS_PacketHeader *PacketHeader, const xTS_AdaptationField *AdaptationField) {
	if (PacketHeader->get_packetIdentifier() == m_PID) {
		uint8_t cc = PacketHeader->get_continuityCounter();

		uint32_t PayloadSize = xTS::TS_PacketLength - xTS::TS_HeaderLength;
		if (PacketHeader->hasAdaptationField()) {
			PayloadSize -= (AdaptationField->getNumBytes() + 1);
		}

		if (PacketHeader->get_payloadUnitStartIndicator() == 1) {
			xBufferReset();
			m_PESH.Reset();
			m_Started = true;
			m_LastContinuityCounter = cc;
			m_EndContinuityCounter = (cc + 15) % 16;
			if (PacketHeader->hasPayload()) {
				xBufferAppend(TransportStreamPacket,
							  xTS::TS_HeaderLength + AdaptationField->get_adaptationFieldLength() + 1);
			}

			return xPES_Assembler::eResult::AssemblingStarted;
		} else if (cc == m_EndContinuityCounter) {
			if (PacketHeader->hasPayload()) {
				xBufferAppend(TransportStreamPacket,
							  xTS::TS_HeaderLength + AdaptationField->get_adaptationFieldLength() + 1);
			}
			m_PESH.Reset();
			m_PESH.Parse(m_Buffer);
			m_DataOffset = m_PESH.getPacketLength() + xTS::PES_HeaderLength;

			return xPES_Assembler::eResult::AssemblingFinished;
		} else if (cc >= 0 && cc <= 15 && cc == (m_LastContinuityCounter + 1) % 16 && m_Started) {
			m_LastContinuityCounter++;
			if (PacketHeader->hasPayload()) {
				xBufferAppend(TransportStreamPacket,
							  xTS::TS_HeaderLength + AdaptationField->get_adaptationFieldLength() + 1);
			}

			return xPES_Assembler::eResult::AssemblingContinue;
		} else {
			xBufferReset();
			return xPES_Assembler::eResult::StreamPacketLost;
		}
	} else {
		return xPES_Assembler::eResult::UnexpectedPID;
	}
}
*/
xPES_Assembler::eResult xPES_Assembler::AbsorbPacket(const uint8_t *TransportStreamPacket, const xTS_PacketHeader *PacketHeader, const xTS_AdaptationField *AdaptationField) {
	if (PacketHeader->get_packetIdentifier() != m_PID) {
		return xPES_Assembler::eResult::UnexpectedPID;
	}

	uint32_t PayloadSize = xTS::TS_PacketLength - xTS::TS_HeaderLength;

	if (PacketHeader->hasAdaptationField()) {
		PayloadSize -= (AdaptationField->getNumBytes() + 1);
	}

	if (PacketHeader->get_payloadUnitStartIndicator() == 1 && AdaptationField->get_randomAccessIndicator()) {
		xBufferReset();

		m_PESH.Reset();
		m_PESH.Parse(&TransportStreamPacket[5 + AdaptationField->getNumBytes()]);

		if (m_PESH.getPacketStartCodePrefix() != 1) {
			return xPES_Assembler::eResult::StreamPacketLost;
		}

		if (m_PESH.getPacketStartCodePrefix() == 1) {
			m_Started = true;
			m_BufferSize = m_PESH.getPacketLength() + xTS::PES_HeaderLength - m_PESH.get_PESHeaderDataLength();
			m_Buffer = new uint8_t[m_BufferSize];

			PayloadSize -= m_PESH.get_PESHeaderDataLength();
			xBufferAppend(TransportStreamPacket, PayloadSize);
			return xPES_Assembler::eResult::AssemblingStarted;
		}
		else return xPES_Assembler::eResult::StreamPacketLost;
	}
	//if (m_Started)cout << "\nm_Started=1"; else cout << "\nm_Started=0";

	//cout << "\nm_DataOffset + PayloadSize=" << m_DataOffset + PayloadSize;
	//cout << "\n m_PESH.getPacketLength() + xTS::TS_HeaderLength - m_PESH.get_PESHeaderDataLength()=" << m_PESH.getPacketLength() + xTS::TS_HeaderLength - m_PESH.get_PESHeaderDataLength();
	
	if (m_Started && m_DataOffset + PayloadSize == m_PESH.getPacketLength() + xTS::PES_HeaderLength - m_PESH.get_PESHeaderDataLength()) {
		xBufferAppend(TransportStreamPacket, PayloadSize);
		m_Started = false;
		return xPES_Assembler::eResult::AssemblingFinished;
	}

	else if (m_Started) {
		xBufferAppend(TransportStreamPacket, PayloadSize);
		m_LastContinuityCounter++;
		return xPES_Assembler::eResult::AssemblingContinue;
	}
	return xPES_Assembler::eResult::StreamPacketLost;
}

#include <iostream>
#include <cstdio>

using namespace std;

int main(int argc, char *argv[], char *envp[]) {
	FILE *file = fopen("example_new.ts", "rb");
	if (file == nullptr) {
		printf("Nullptr! \n");
		return EXIT_FAILURE;
	}

	FILE *outputfile = fopen("PID136.mp2", "wb");

	xTS_PacketHeader TS_PacketHeader;
	int32_t TS_PacketId = 0;

	char *buffer;
	buffer = (char *)malloc(sizeof(char) * xTS::TS_PacketLength);
	uint8_t TS_PacketBuffer[188];

	xTS_AdaptationField TS_AdaptationField;

	xPES_Assembler PES_Assembler;
	PES_Assembler.Init(136);

	while (!feof(file)) {
		fread(buffer, xTS::TS_PacketLength, 1, file);
		for (int i = 0; i < xTS::TS_PacketLength; i++) {
			TS_PacketBuffer[i] = buffer[i];
		}

		TS_PacketHeader.Reset();
		TS_PacketHeader.Parse2(TS_PacketBuffer);
		TS_AdaptationField.Reset();

		if (TS_PacketHeader.get_syncByte() == 71 && TS_PacketHeader.get_packetIdentifier() == 136) {
			if (TS_PacketHeader.hasAdaptationField()) {
				TS_AdaptationField.Parse(TS_PacketBuffer, TS_PacketHeader.get_adaptationFieldControl());
			}

			printf("%010d ", TS_PacketId);
			TS_PacketHeader.Print();
			if (TS_PacketHeader.hasAdaptationField()) {
				TS_AdaptationField.Print();
			}

			xPES_Assembler::eResult Result = PES_Assembler.AbsorbPacket(TS_PacketBuffer, &TS_PacketHeader,
				&TS_AdaptationField);
			switch (Result) {
			case xPES_Assembler::eResult::StreamPacketLost:
				printf(" PcktLost ");
				break;
			case xPES_Assembler::eResult::AssemblingStarted:
				printf(" Started ");
				PES_Assembler.PrintPESH();
				break;
			case xPES_Assembler::eResult::AssemblingContinue:
				printf(" Continue ");
				break;
			case xPES_Assembler::eResult::AssemblingFinished:
				printf(" Finished ");
				printf(" PES: Len=%d", PES_Assembler.getNumPacketBytes());
				fwrite(PES_Assembler.getPacket(), PES_Assembler.getNumPacketBytes(), 1, outputfile);
				break;
			default:
				break;
			}

			printf("\n");
		}
		TS_PacketId++;
	}

	fclose(file);
	free(buffer);
	fclose(outputfile);
}