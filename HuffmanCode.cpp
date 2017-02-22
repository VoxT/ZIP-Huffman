#ifndef _HUFFMAN_CODE_CPP_
#define _HUFFMAN_CODE_CPP_

#include "HuffmanCode.h"

// convert byte to character
#define byte2Char(val) ((unsigned char) ((val) % 256))

// convert character to ASCII
#define char2Int(ch) ((int) (ch) < 0 ? 128 + (((int) (ch)) & 127): (((int) (ch)) & 127))


int HuffmanCode::zip(char* inputFile, char* outputFile) {
	// TODO implement for zip
	cout << "zipping...";

	HCZHeader header;
	Reader* reader = new Reader(inputFile);

	readInput(reader);

	if( List_node[0] == NULL) return ERR;
	
	Writer *writer = new Writer(outputFile);

	huffTree();  // xay dung cay huffman

	unsigned char Bangma[256][256];
	unsigned char Tam[256];

	/*khoi tao bang ma voi gia tri dau = 0 la so luong bit ma hoa cho ki tu theo ma ASCII cua no va tao bang ma*/
	for(int i = 0; i < 256; i++) Bangma[i][0] = 0;

	if(List_node[0]->pLeft == NULL && List_node[0]->pRight == NULL)
	{
		Bangma[List_node[0]->c][0] = 1;
		Bangma[List_node[0]->c][1] = 1;
		bodySize = List_node[0]->nFeq;
	}
	else
		TaoBangMa(List_node[0], Bangma, 0, Tam);

	header.setBodySize(bodySize);
	header.setTotal(totalChar, totalBit);

	// set(char ch, int nbits, char* code)
	for(int i = 0; i < 256; i++)
		if(Bangma[i][0] != 0) {
			char* codeBit = new char[Bangma[i][0] + 1];
			int j = 1;
			for(j; j <= Bangma[i][0]; j++){
				if(Bangma[i][j] == 1)
					codeBit[j -1] = '1';
				else	codeBit[j -1] = '0'; 
			}
			codeBit[j -1] ='\0';

			header.set(i, Bangma[i][0], codeBit);
		}

		header.write(writer);
		reader = new Reader(inputFile);

		// write zip
		while(reader->isHasNext())
		{
			int nBit = (unsigned char) reader->readChar();
			for(int i = 1; i <= Bangma[nBit][0]; i++)
				writer->writeBit(Bangma[nBit][i]);
		}

		delete reader;
		delete writer;

		return SUCCESS;
}

int HuffmanCode::unzip(char* inputFile, char* outputFile) {
	// TODO implement for unzip
	cout << "unzipping...";

	HCZHeader header;
	Reader* reader = new Reader(inputFile);
	
	if(header.read(reader) == SUCCESS) {
		Writer* writer = new Writer(outputFile);
		unzipTree(&header);
		writeUnzip(reader, & header, writer);
	}
	else return WRONG_FORMAT;

	return UN_IMPLEMENT;
}

void HuffmanCode::readInput(Reader* reader) 
{
	if(reader->isHasNext()) {
		totalChar++;
		unsigned char ch = reader->readChar();
		insertQueue(ch, 1, NULL, NULL);
	}

	while(reader->isHasNext())
	{
		int i = 0;
		unsigned char ch = reader->readChar();

		while( List_node[i] != NULL)
		{
			if(ch == List_node[i]->c) {
				List_node[i]->nFeq++;
				break;
			}
			i++;
		} // stop while 

		if(List_node[i] == NULL) {
			totalChar++;
			insertQueue(ch, 1, NULL, NULL);
		}
	} // stop while 
}

void HuffmanCode::insertQueue(unsigned char ch, unsigned long nfeq, HuffmanTree* left, HuffmanTree* right)
{
	int i = 0;
	while(List_node[i] != NULL)	i++;

	List_node[i] = new HuffmanTree(ch, nfeq, left, right);
}

void HuffmanCode::huffTree()
{
	while(List_node[1] != NULL)
	{
		sort();

		unsigned long nFeq = List_node[0]->nFeq + List_node[1]->nFeq;

		insertQueue( 0, nFeq, List_node[0], List_node[1]);

		remove();
	}
}

void HuffmanCode::sort()
{
	for(int i = 0; List_node[i] != NULL; i++)
	{
		for(int j = i + 1; List_node[j] != NULL; j++)
		{
			if(List_node[j]->nFeq < List_node[i]->nFeq)
			{
				HuffmanTree* pTemp = List_node[i];
				List_node[i] = List_node[j];
				List_node[j] = pTemp;
			}
		}
	}
}

void HuffmanCode::remove()
{
	for(int i = 0; i <= 254; i++)
	{
		List_node[i] = List_node[i+2];
	}
	List_node[255] = NULL;
	List_node[256] = NULL;
}

void HuffmanCode::TaoBangMa(HuffmanTree *pRoot,unsigned char Bang[256][256],int VT,unsigned char Tam[256])
{   
	if (pRoot != NULL)
	{   
		if (pRoot->pLeft == NULL && pRoot->pRight == NULL)
		{
			for(int i = 1; i <= VT; i++)
				Bang[pRoot->c][i] = Tam[i];
			Bang[pRoot->c][0] = VT;
			bodySize += VT * pRoot->nFeq;
			totalBit += VT;
		}

		VT++;//Duyet nhanh trai
		Tam[VT] = 0;
		TaoBangMa(pRoot->pLeft, Bang, VT, Tam);

		Tam[VT] = 1;//Duyet nhanh phai
		TaoBangMa(pRoot->pRight, Bang, VT, Tam);
		VT--;
	}
}

void HuffmanCode::unzipTree(HCZHeader* header)
{
	List_node[0] = new HuffmanTree(0, 0, NULL, NULL);

	for(int i = 0; i < 256; i++)
	{
		int nBit = 0;
		char code[256];
		if(header->get(i, nBit, code) == 0) {
			//cout<< i << " " << nBit << " " ;
			HuffmanTree* pTemp = List_node[0];
			for( int j = 0; j < nBit; j++)
			{
				//cout << code[j];
				if((code[j] - '0') == 1)
				{
					if(j == nBit - 1) 
						pTemp->pRight = new HuffmanTree(i, 0, NULL, NULL);
					else if(pTemp->pRight == NULL)
						pTemp->pRight = new HuffmanTree(0, 0, NULL, NULL);
					pTemp = pTemp->pRight;
				}
				else
				{
					if( j == nBit - 1)
						pTemp->pLeft = new HuffmanTree(i, 0, NULL, NULL);
					else if(pTemp->pLeft == NULL)
						pTemp->pLeft = new HuffmanTree(0, 0, NULL, NULL);
					pTemp = pTemp->pLeft;
				}
			} // for
		//	cout << endl;
		} // if
	} // stop for
}

void HuffmanCode::writeUnzip(Reader* reader, HCZHeader* header, Writer* writer)
{

	HuffmanTree* pTemp = List_node[0];

	for(int i = 0; i <= header->getBodySize(); i++)
	{	
		if(pTemp->pLeft == NULL && pTemp->pRight == NULL)
		{
			writer->writeByte(pTemp->c);
			pTemp = List_node[0];
		}
		if(reader->readBit() == 1)
			pTemp = pTemp->pRight;
		else pTemp = pTemp->pLeft;
	}

	delete writer;
}

#endif