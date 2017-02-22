#ifndef _READER_CPP_
#define _READER_CPP_

#include "Reader.h"

#ifndef MAXBUFSIZE
#define MAXBUFSIZE 10000
#endif

Reader::~Reader() {
    file.close();
}
Reader::Reader(char* fileName) {
    file.open (fileName, ios::in|ios::binary);

    if (!file.is_open()) {
        cout << "Unable to open file\n";
        bufSize = 0;
    } else {
        readToBuf();
    }
    bufPos = 0;
    bitPos = 0;
}

bool Reader::isHasNext() {
    return bufPos < bufSize; //&& file.eof();
}

int Reader::readBit() {
    if (!file.is_open())
        return -1;
    int ret = getBit(buf[bufPos], bitPos);
    increaseBitPos();
    return ret;
}

int Reader::readByte() {
    int ret = 0;
    for (int i = 0; i < 8; ++i)
        ret = (ret << 1) + readBit();
    return ret;
}

char Reader::readChar() {
    int tmp = readByte();
    unsigned char val = (unsigned char) (tmp % 256);
    return (char)val;
}

int Reader::readWord() {
    int ret = readByte();
    ret = (ret << 8) + readByte();
    return ret;
}

int Reader::readDWord() {
    int ret = readWord();
    ret = (ret << 16) + readWord();
    return ret;
}

void Reader::test() {
    cout << "test: " << (int) '5' << " " << getBit('5', 5) << endl;
    cout << ((1 << 3) | (1 << 2)) << endl;
}

int Reader::getBit(char c, int pos) {
    if ((c & (1<<(7 - pos))) == 0)
        return 0;
    else
        return 1;
}

void Reader::increaseBitPos() {
    ++bitPos;
    if (bitPos == 8) {
        bitPos = 0;
        increaseBufPos();
    }
}

void Reader::increaseBufPos() {
    ++bufPos;
    if (bufPos == bufSize) {
        readToBuf();
        bufPos = 0;
    }
}

void Reader::readToBuf() {
    ifstream::pos_type size = MAXBUFSIZE;
    file.read (buf, size);
    bufSize = file.gcount();
    //printBuf(buf, bufSize);
}

#endif
