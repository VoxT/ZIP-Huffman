#ifndef _WRITER_CPP_
#define _WRITER_CPP_

#include "Writer.h"

#ifndef MAXBUFSIZE
#define MAXBUFSIZE 10000
#endif

Writer::~Writer() {
    // TODO
    if (bitPos == 0)
        file.write (buf, bufPos);
    else
        file.write (buf, bufPos + 1);
    if (file.is_open())
        file.close();
}

Writer::Writer(char* fileName) {
    file.open(fileName, ios::out | ios::binary);
    bufSize = MAXBUFSIZE;
    bufPos = 0;
    bitPos = 0;
}

void Writer::writeBit(int bit) {
    if (bit == 1)
        setBit(buf[bufPos], bitPos);
    else
        unSetBit(buf[bufPos], bitPos);

    increaseBitPos();
}

void Writer::writeByte(int ch) {
    for (int i = 7; i >= 0; --i) {
        writeBit((ch >> i) % 2);
    }
}

void Writer::writeWord(int word) {
    writeByte((word >> 8) % 256);
    writeByte(word % 256);
}

void Writer::writeDWord(int dword) {
    writeWord((dword >> 16) % 65536);   // 65536
    writeWord(dword % 65536);
}

void Writer::test() {
    //printBuf(buf, 1);
    cout << (int) buf[0] << endl;

    // charactor '5'
    writeBit(0);
    writeBit(0);
    writeBit(1);
    writeBit(1);
    writeBit(0);
    writeBit(1);
    writeBit(0);
    writeBit(1);

    // print 53 5
    cout << (int) buf[0] << " " << buf[0] << endl;;

}

void Writer::increaseBitPos() {
    ++bitPos;
    if (bitPos == 8) {
        bitPos = 0;
        increaseBufPos();
    }
}
void Writer::increaseBufPos() {
    ++bufPos;
    if (bufPos == bufSize) {
        // write to file
        file.write (buf, bufSize);
        bufPos = 0;
    }
}

void Writer::setBit(char &ch, int pos) {
    ch = ch | (1 << (7 - pos));
}
void Writer::unSetBit(char &ch, int pos) {
    ch = ch & ~(1 << (7 - pos));
}


#endif
