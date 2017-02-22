#ifndef _HCZHeader_H_
#define  _HCZHeader_H_

#include "Reader.h"
#include "Writer.h"

#define SUCCESS 0
#define WRONG_FORMAT -2

#define CHAR_SET_SIZE 256

class HCZHeader {
public:
    ~HCZHeader();
    HCZHeader();

    /* read header from reader and check if right format */
    int read(Reader* reader);

    /* write header to writer */
    int write(Writer* writer);

    int setBodySize(int bodySize);
    int getBodySize();

    /* set total char used and total of bits used to encode */
    void setTotal(int totalChar, int totalBit);
    
    /* get total */
    void getTotal(int &totalChar, int &totalBit);

    /*
     * set number of bits and code for one charactor
     * e.g. character 'a' use 3 bit '001' then call set('a', 3, '001')
     */
    int set(char ch, int nbits, char* code);

    int get(char ch, int &nbits, char* code);

private:
    char code[3];   // code HCZ
    int bodySize;   // number of bits of body (zip content)

    int totalChar;  // number of char used on document
    char* chars;    // number of bits used for each char
    int charMap[CHAR_SET_SIZE];  // to map from char to index on chars
    int cPos;      // position of char to set
    int* bitOfChar;    // bit list
    int* totalBitsUntilCh;

    int totalBits;  // total of bits use to encode for chars
    char* bitList;  // bit list
    int ctotalBits; // store count bits until current character (used in set)

    /*
     * Set bit on bitList in position (pos) which value 1/0
     */
    void setBit(char* bitList, int pos, int code);

    /*
     * get a bit in position
     */
    int getBit(char* bitList, int pos);
};

#endif
