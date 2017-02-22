#ifndef _HCZHeader_CPP_
#define  _HCZHeader_CPP_

#include "HCZHeader.h"

#define SUCCESS 0
#define WRONG_FORMAT -2

// convert byte to character
#define byte2Char(val) ((unsigned char) ((val) % 256))

// convert character to ASCII
#define char2Int(ch) ((int) (ch) < 0 ? 128 + (((int) (ch)) & 127): (((int) (ch)) & 127))

HCZHeader::~HCZHeader() {
    if (chars != NULL)
        delete[] chars;
    if (bitOfChar != NULL)
        delete[] bitOfChar;

    if (totalBitsUntilCh != NULL)
        delete[] totalBitsUntilCh;
    if (bitList != NULL)
        delete[] bitList;
}

HCZHeader::HCZHeader() {
    code[0] = 'H';
    code[1] = 'C';
    code[2] = 'Z';
    bodySize = 0;

    totalChar = 0;
    chars = NULL;
    for (int ch = 0; ch < CHAR_SET_SIZE; ++ch)
        charMap[ch] = -1;
    cPos = 0;

    bitOfChar = NULL;
    totalBitsUntilCh = NULL;

    totalBits = 0;
    bitList = NULL;
    ctotalBits = -1;
}

/* read header from reader and check if right format */
int HCZHeader::read(Reader* reader) {

    code[0] = byte2Char(reader->readByte());
    code[1] = byte2Char(reader->readByte());
    code[2] = byte2Char(reader->readByte());

    // check special charactor
    if (code[0] != 'H' || code[1] != 'C' || code[2] != 'Z')
        return WRONG_FORMAT;

    bodySize = reader->readDWord();

    totalChar = reader->readWord();
//cout << "totalChar: " << totalChar << " " << bodySize << endl;
    chars = new char[totalChar];
    for (int i = 0; i < totalChar; ++i) {
        chars[i] = byte2Char(reader->readByte());
        charMap[char2Int(chars[i])] = i;
    }

    bitOfChar = new int[totalChar];
    totalBits = 0;
    for (int i = 0; i < totalChar; ++i) {
        bitOfChar[i] = reader->readByte();
        totalBits += bitOfChar[i];
    }

    // read bit list
    bitList = new char[(totalBits + 7) / 8];
    for (int i = 0; i < (totalBits + 7) / 8; ++i) {
        bitList[i] = byte2Char(reader->readByte());
    }

    totalBitsUntilCh = new int[totalChar];
    totalBitsUntilCh[0] = 0;
    for (int i = 1; i < totalChar; ++i) {
        totalBitsUntilCh[i] = totalBitsUntilCh[i-1] + bitOfChar[i-1];
    }

    return SUCCESS;
}

/* write header to writer */
int HCZHeader::write(Writer* writer) {
//cout << "totalChar: " << (int)totalChar << " " << bodySize << endl;
    writer->writeByte(char2Int(code[0]));
    writer->writeByte(char2Int(code[1]));
    writer->writeByte(char2Int(code[2]));

    writer->writeDWord(bodySize);

    writer->writeWord(totalChar);
    for (int i = 0; i < totalChar; ++i)
        writer->writeByte(char2Int(chars[i]));

    for (int i = 0; i < totalChar; ++i)
        writer->writeByte(bitOfChar[i]);

    for (int i = 0; i < (totalBits + 7) / 8; ++i)
        writer->writeByte(char2Int(bitList[i]));

    return SUCCESS;
}

int HCZHeader::setBodySize(int bodySize) {
    this->bodySize = bodySize;
    return SUCCESS;
}

int HCZHeader::getBodySize() {
    return bodySize;
}

/* set total char used and total of bits used to encode */
void HCZHeader::setTotal(int totalChar, int totalBit) {
    this->totalChar = totalChar;
    chars = new char[totalChar];
    bitOfChar = new int[totalChar];

    this->totalBits = totalBit;
    bitList = new char[(totalBits + 7) / 8];
    totalBitsUntilCh = new int[totalChar];
}

void HCZHeader::getTotal(int &totalChar, int &totalBit) {
    totalChar = this->totalChar;
    totalBit = this->totalBits;
}
/*
 * set number of bits and code for one charactor
 * e.g. character 'a' use 3 bit '001' then call set('a', 3, '001')
 */
int HCZHeader::set(char ch, int nbits, char* code) {
//cout << "set " << ch << " " << nbits << " " << code << " " << ctotalBits << endl;
    chars[cPos] = ch;
    charMap[char2Int(ch)] = cPos;
    bitOfChar[cPos] = nbits;

    if (ctotalBits < 0)
        ctotalBits = 0;
    // set code
    for (int i = 0; i < nbits; ++i)
        setBit(bitList, i + ctotalBits, code[i] - '0');

    totalBitsUntilCh[cPos] = ctotalBits;
    ctotalBits += nbits;
    ++cPos;

    return SUCCESS;
}

int HCZHeader::get(char ch, int &nbits, char* code) {
    int pp = charMap[char2Int(ch)];
    nbits = 0;
    if (pp == -1) return -1;  // fix bug: 25 Oct
    nbits = bitOfChar[pp];

    for (int i = 0; i < nbits; ++i)
        code[i] = '0' + getBit(bitList, i + totalBitsUntilCh[pp]);
    code[nbits] = '\0';
    return SUCCESS;
}

/*
 * Set bit on bitList in position (pos) which value 1/0
 */
void HCZHeader::setBit(char* bitList, int pos, int code) {
    int pp = pos / 8;
    pos = pos % 8;

    if (code == 0) {
        bitList[pp] = bitList[pp] & ~(1 << (7 - pos));
    } else { // code == 1
        bitList[pp] = bitList[pp] | (1 << (7 - pos));
    }
}

/*
 * get a bit in position
 */
int HCZHeader::getBit(char* bitList, int pos) {
    int pp = pos / 8;
    pos = pos % 8;

    return (bitList[pp] & (1<<(7-pos))) == 0 ? 0 : 1;
}

#endif
