#ifndef _WRITER_H_
#define _WRITER_H_

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#ifndef MAXBUFSIZE
#define MAXBUFSIZE 10000
#endif

class Writer {
public:
    ~Writer();
    
    /* contructor */
    Writer(char* fileName);
    
    /* write one bit */
    void writeBit(int bit);
    
    /* write one byte */
    void writeByte(int ch);
    
    /* write one word */
    void writeWord(int word);
    
    /* write one dword */
    void writeDWord(int dword);
    
    void test();
    
private:
    ofstream file;
    char buf[MAXBUFSIZE];
    int bufSize;
    int bufPos;
    int bitPos;

    /* increase bitpos by 1 */
    void increaseBitPos();
    
    /* increase bufpos by 1 */
    void increaseBufPos();

    /* set a bit on byte at position */
    void setBit(char &ch, int pos);
    
    /* unset a bit on byte at position */
    void unSetBit(char &ch, int pos);
};

#endif
