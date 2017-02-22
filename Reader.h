#ifndef _READER_H_
#define _READER_H_

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#ifndef MAXBUFSIZE
#define MAXBUFSIZE 10000
#endif

class Reader {
public:
    ~Reader();
    
    /* constructor */
    Reader(char* fileName);

    /* check if has next bit */
    bool isHasNext();

    /* read one bit */
    int readBit();

    /* read one byte */
    int readByte();
    
    /* read one character */
    char readChar();

    /* read one word */
    int readWord();
    
    /* read one dword */
    int readDWord();

    /* for test only */
    void test();
private:
    
    ifstream file;  // file pointer
    char buf[MAXBUFSIZE];   // buffer to read
    int bufSize;    // current buffer size
    int bufPos;     // current position of reading
    int bitPos;     // current bit reading (on bufPos)

    int getBit(char c, int pos);

    void increaseBitPos();
    
    void increaseBufPos();

    void readToBuf();
};

#endif
