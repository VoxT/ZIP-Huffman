#ifndef _HUFFMAN_CODE_H_
#define _HUFFMAN_CODE_H_

#include <iostream>
#include <fstream>
#include <cstring>

#include "Reader.h"
#include "Writer.h"
#include "HCZHeader.h"

using namespace std;

#define ERR -1

#define SUCCESS 0
#define UN_IMPLEMENT -2
#define MAXBUFSIZE 10000

// convert byte to character
#define byte2Char(val) ((unsigned char) ((val) % 256))

// convert character to ASCII
#define char2Int(ch) ((int) (ch) < 0 ? 128 + (((int) (ch)) & 127): (((int) (ch)) & 127))


class HuffmanTree {
public:
	HuffmanTree(){}
	HuffmanTree(unsigned char ch, unsigned long nfeq, HuffmanTree* left, HuffmanTree* right){
		this->c = ch;
		this->nFeq = nfeq;
		this->pLeft = left;
		this->pRight = right;
	}

	unsigned long nFeq;
	unsigned char c;
	HuffmanTree* pLeft;
	HuffmanTree* pRight;
};

class HuffmanCode {
public:
	HuffmanCode() {
		for(int i = 0; i < 257; i++)
			this->List_node[i] = NULL;
		this-> totalChar = 0;
		this-> totalBit = 0;
		this-> bodySize = 0;
	}

	int zip(char* inputFile, char* outputFile);
	int unzip(char* inputFile, char* outputFile);

	void readInput(Reader* reader);	// read input, dem so lan xuat hien moi loai ki tu va them vao Queue
	void insertQueue(unsigned char ch, unsigned long nfeq,  HuffmanTree* left, HuffmanTree* right);
	void huffTree();
	void sort();
	void remove();
	void TaoBangMa(HuffmanTree *pRoot,unsigned char Bang[256][256],int VT,unsigned char Tam[256]);

	void unzipTree(HCZHeader* header);
	void writeUnzip(Reader* reader, HCZHeader* header, Writer* writer);

	// mang con tro tro den cac node cua huffman tree.
	HuffmanTree* List_node[257];
	int totalChar;
	int totalBit;
	unsigned long bodySize;

private:

};


#endif