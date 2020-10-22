/* Program		: HuffmanImplementation
 * Deskripsi	: Membuat Program Implementasi Huffman Code
 * Nama			: M. Naufal Fadhil (181524016)
 				  Rayhan Azka Anandhias Putra (181524028)
 * Tanggal/versi: 10-07-2019/ v2.0.0 (Versi Revisi)
 * Compiler		: TDM-GCC 4.9.2 64-bit
 */
//=================================================================================================
#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdbool.h>
#define MAX_ELMT 10000
#define CHAR_SET 255

typedef struct huffmanNode* address;
typedef struct huffmanNode node;
struct huffmanNode {
	char data;
	unsigned int freq;
	address left, right;
};
typedef struct huffmanNode** arrayOfNode;

typedef struct nodeCollection nCollection;
typedef struct nodeCollection* addressCollection;
struct nodeCollection {
	unsigned int size;
	unsigned int capacity;
	arrayOfNode arr;
};

typedef struct {
	char* charsaved;
	unsigned* freqsaved;
}SavedString;

typedef struct {
	char data;
	char code[20];
}refrence;

void huffman(char* text);
char* characterSet(char* text);
char* append(char* oldstring, char c);
void swap(address* a, address* b);
void sortCollection(addressCollection collection);
addressCollection createNodeCollection(char* text, unsigned int size, char* data);
address getMinFreqNode(addressCollection collection);
bool isCollectionSizeOne (addressCollection collection);
bool isLeaf(address root);
void insertToCollection(address internalNode, addressCollection collection);
int getLevel (address Troot, char data, unsigned int keyfreq, address currentAddress);
address newNode (char data, unsigned int frequency);
addressCollection createCollection(unsigned int capacity);
unsigned int frequency(char* text, char key);
char* findNode (address root,char find,refrence save[],int saveSize);
void GenerateCode(address root,char alph);
void encode(address root, char* text);
int StringToInt(char* binString);
int binaryToDecimal(int i);
char* ShowCompressedText(char* binaryString);
void detail(address root, char* text);
void huffmanTable(address root, addressCollection collection, char* binaryString, int *sum, int size, float *avg);
void HuffmanTree(address Troot, address root);
void printHuffmanTree(address TreeRoot, address node);
address buildHuffmanTree(char* text);
int StringToInt(char* binString);
void menu(address root,char* text);
void startProgramText();
void startProgram();
addressCollection startWordFreq();
SavedString saved;
void readText();
char* findNode1(address root,char find);
char* decimalToBinary(int n);
void test();
char* getBinaryFromCompressed(char fileName[],char extendedBit[]);
refrence* getRequirementForDecode(char refFile[]);
char* decode();
char* reverse_string(char *str);
void saveDecompressedFile(char* decoded);
char* getRefName(char fileName[]);
char* getExtendedBit(char fileName[]);
char* getRealName(char fileName[]);
char* getCompressedName(char fileName[]);
void decompressedMenu();
#endif
