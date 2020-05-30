#include "tsCommon.h"
#include "tsTransportStream.h"
#include <iostream>

using namespace std;

int main( int argc, char *argv[ ], char *envp[ ]) {
    FILE *file = fopen("D:\\Dokumenty\\Polibuda\\3 rok\\PDM\\TS_Parser\\example_new.ts", "rb");
    if (file == nullptr) {
        printf("Nullptr! \n");
        return EXIT_FAILURE;
    }

    xTS_PacketHeader TS_PacketHeader;
    int32_t TS_PacketId = 0;

    char *buffer;
    buffer = (char *) malloc(sizeof(char) * xTS::TS_PacketLength);
    uint8_t TS_PacketBuffer[188];

    xTS_AdaptationField TS_AdaptationField;

    //xPES_Assembler PES_Assembler;
    //PES_Assembler.Init(136);

    while (!feof(file)) {
        fread(buffer, xTS::TS_PacketLength, 1, file);
        for (int i = 0; i < xTS::TS_PacketLength; i++) {
            TS_PacketBuffer[i] = buffer[i];
        }

        TS_PacketHeader.Reset();
        TS_PacketHeader.Parse2(TS_PacketBuffer);
        TS_AdaptationField.Reset();

        if(TS_PacketHeader.get_syncByte() == 71 && TS_PacketHeader.get_packetIdentifier() == 136) {
            if (TS_PacketHeader.hasAdaptationField()) {
                TS_AdaptationField.Parse(TS_PacketBuffer, TS_PacketHeader.get_adaptationFieldControl());
            }

            printf("%010d ", TS_PacketId);
            TS_PacketHeader.Print();
            if (TS_PacketHeader.hasAdaptationField()) {
                TS_AdaptationField.Print();
            }

            /*xPES_Assembler::eResult Result = PES_Assembler.AbsorbPacket(TS_PacketBuffer, &TS_PacketHeader,
                                                                        &TS_AdaptationField);
            switch (Result) {
                case xPES_Assembler::eResult::StreamPacketLost:
                    printf("PcktLost ");
                    break;
                case xPES_Assembler::eResult::AssemblingStarted:
                    printf("Started ");
                    PES_Assembler.PrintPESH();
                    break;
                case xPES_Assembler::eResult::AssemblingContinue:
                    printf("Continue ");
                    break;
                case xPES_Assembler::eResult::AssemblingFinished:
                    printf("Finished ");
                    printf("PES: Len=%d", PES_Assembler.getNumPacketBytes());
                    break;
                default:
                    break;
            }*/

            printf("\n");
        }
        TS_PacketId++;
    }

    fclose(file);
    free(buffer);
}