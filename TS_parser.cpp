#include "tsCommon.h"
#include "tsTransportStream.h"
#include <iostream>

using namespace std;

int main( int argc, char *argv[ ], char *envp[ ]) {
    FILE *file = fopen("example_new.ts", "rb");
    if (file == nullptr) {
        printf("Nullptr! \n");
        //return EXIT_FAILURE;
    }

    xTS_PacketHeader TS_PacketHeader;
    int32_t TS_PacketId = 0;

    char *buffer;
    buffer = (char *) malloc(sizeof(char) * xTS::TS_PacketLength);
    uint8_t TS_PacketBuffer[188];

    while (!feof(file)) {
        fread(buffer, xTS::TS_PacketLength, 1, file);
        for (int i = 0; i < xTS::TS_PacketLength; i++) {
            TS_PacketBuffer[i] = buffer[i];
        }

        TS_PacketHeader.Reset();
        TS_PacketHeader.Parse(TS_PacketBuffer);

        printf("%010d ", TS_PacketId);
        TS_PacketHeader.Print();
        printf("\n");

        TS_PacketId++;
    }

    fclose(file);
    free(buffer);
}