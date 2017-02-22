#include "HuffmanCode.h"

#define ZIP 4
#define UNZIP 5
#define ERR -1

#define SUCCESS 0
#define UN_IMPLEMENT -2

int getCode(char* code);

void test();

int main(int argc, char *argv[]) {

    /*argc = 4;
    argv = new char* [4];
    argv[1] = "zip";
    argv[2] = "input.txt";
    argv[3] = "out.txt";/**/

    if (argc < 4) {
        cout << "Usage:\n";
        cout << "huffman zip/unzip input output\n";
        return ERR;
    }

    int code = getCode(argv[1]);
    if (code == ERR) {

        cout << "wrong code: " << argv[1] << endl;
        return ERR;
    } else if (code == ZIP) {
        HuffmanCode hc;
        return hc.zip(argv[2], argv[3]);
    } else if (code == UNZIP) {
        HuffmanCode hc;
        return hc.unzip(argv[2], argv[3]);
    }

    return SUCCESS;
}

int getCode(char* code) {
    if (strcmp("zip", code) == 0)
        return ZIP;
    if (strcmp("unzip", code) == 0)
        return UNZIP;
    return ERR;
}

