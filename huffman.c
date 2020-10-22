/* Program		: HuffmanImplementation
 * Deskripsi	: Membuat Program Implementasi Huffman Code
 * Nama			: M. Naufal Fadhil (181524016)
 				  Rayhan Azka Anandhias Putra (181524028)
 * Tanggal/versi: 10-07-2019/ v2.0.0 (Versi Revisi)
 * Compiler		: TDM-GCC 4.9.2 64-bit
 */
//=================================================================================================
#include "huffman.h"
#include "stacklink.h"
#include "queuelink.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

bool whichMenu;
char fileNameGlobal[MAX_ELMT];
int ref_idx;

address newNode (char data, unsigned int frequency) {
	address nNew = (address) malloc(sizeof(node));
	nNew->data = data;
	nNew->freq = frequency;
	nNew->left = nNew->right = NULL;
	return nNew;
}

addressCollection createCollection(unsigned int capacity) {
	addressCollection collection = (addressCollection) malloc(sizeof(nCollection));
	collection->size = 0;
	collection->capacity = capacity;
	collection->arr = (arrayOfNode) malloc(sizeof(address) * collection->capacity);
	return collection;
}  

char* append(char* oldstring, char c) {
    int result;
    char *newstring = malloc(sizeof(char)*CHAR_SET);
    result = sprintf(newstring, "%s%c", oldstring, c);
    if (result == -1) newstring = NULL;
    return newstring;
}

char* characterSet(char* text) {
	char* temp = malloc(sizeof(char)*MAX_ELMT);
	char* result = malloc(sizeof(char)*CHAR_SET);
	int i,j;
	strcpy(temp, text);
	strcpy(result, "");
	
	for (i = 0; i <= strlen(text) - 1; i++) {
		for (j = i+1; j <= strlen(text) - 1; j++) {
			if (temp[i] == temp[j]) 
				temp[j] = '\0';
		}
	}
	
	j = 1;
	for (i = 0; i <= strlen(text) - 1; i++) {
		if (temp[i] != '\0') {
			strcpy(result, append(result, temp[i]));
			j++;
		}
	}
	return result;
}

unsigned int frequency(char* text, char key) {
	unsigned int freq;
	char* temp = malloc(sizeof(char)*MAX_ELMT);
	int i,j;
	
	temp = characterSet(text);
	for(i = 0; i <= strlen(temp) - 1; i++) {
		if (temp[i] == key) {
			freq = 0;
			for(j = 0;j <= strlen(text) - 1; j++) {
				if(temp[i] == text[j])
					freq++;
			}
			break;
		}
	}
	return freq;
}

void swap(address* a, address* b) { 
	address temp = *a; 
	*a = *b; 
	*b = temp; 
}

void sortCollection(addressCollection collection) {
	int i,j;
	
	if (collection->size != 0) {
		for(i = 0; i < collection->size - 1; i++) {
			for(j = 0; j < collection->size - i - 1; j++) {
				if (collection->arr[j+1]->data != '*') {
					if (collection->arr[j]->freq > collection->arr[j+1]->freq) {
						swap(&collection->arr[j], &collection->arr[j+1]);
					}	
				} else {
					if (collection->arr[j]->freq >= collection->arr[j+1]->freq) {
						swap(&collection->arr[j], &collection->arr[j+1]);
					}
				}
			}
		}
	}
}

addressCollection createNodeCollection(char* text, unsigned int size, char* data) {
	int i;
	addressCollection collection = createCollection(size);
	
	for(i = 0; i < size; i++)
		if (whichMenu == true){
			collection->arr[i] = newNode(data[i], frequency(text, data[i]));	
		} else {
			collection->arr[i] = newNode(saved.charsaved[i], saved.freqsaved[i]);
		}
	
	collection->size = size;
	sortCollection(collection);	
	return collection;
}

address getMinFreqNode(addressCollection collection) {
	address temp = collection->arr[0];
	collection->arr[0] = collection->arr[collection->size - 1];
	collection->size--;
	sortCollection(collection);
	return temp;
}

bool isCollectionSizeOne (addressCollection collection) {
	return (collection->size == 1);
}

bool isLeaf(address root) {
	return (!(root->left) && !(root->right));
}

void insertToCollection(address internalNode, addressCollection collection) {
	int i;
	
	collection->size++;
	i = collection->size - 1;
	collection->arr[i] = internalNode;
	sortCollection(collection);
}

int getLevelOfNode (address root, char data, unsigned int keyfreq, int level, address currentAddress) {
	address current = root;
	if (current == NULL)
		return -1;
	if (current->data == data && current->freq == keyfreq && current == currentAddress)
		return level;
	int result = getLevelOfNode (current->left, data, keyfreq, level+1, currentAddress);
	if (result != -1) {
		return result;
	}
	result = getLevelOfNode(current->right, data, keyfreq, level+1, currentAddress);
	return result;
}

int getLevel (address Troot, char data, unsigned int keyfreq, address currentAddress) {
	return getLevelOfNode(Troot, data, keyfreq, 0, currentAddress);
}

void HuffmanTree(address Troot, address root) {
	int i;
	int level;
	address current = root;
	if (current != NULL) {
		level = getLevel(Troot, current->data, current->freq, current);
		for (i = 0;i <= level; i++) {  
			printf("   ");
		}
		printf("%c(%d)\n", current->data, current->freq);
		HuffmanTree(Troot, current->left);
		HuffmanTree(Troot, current->right);
	}
}

void printHuffmanTree(address TreeRoot, address node) {
	printf("\t\tHUFFMAN TREE\n");
	printf("============================================\n");
	HuffmanTree(TreeRoot, node);
}

address buildHuffmanTree(char* text) {
	char* charset;
	//char* temps = malloc (sizeof(char)*255);
	unsigned size;
	address left, right,top,temp;
	addressCollection collection;
	if (whichMenu != true) {
		collection = startWordFreq();
		//strcpy(temps, saved.charsaved);
	} else {
		charset = characterSet(text);
		size = strlen(charset);
		collection = createNodeCollection(text, size, charset);
	}
	while (!isCollectionSizeOne(collection)) {
		left = getMinFreqNode(collection);
		right = getMinFreqNode(collection);
		if(left->data == '*' && right->data != '*' && left->freq == right->freq) {
			temp = left;
			left = right;
			right = temp;
		}
		top = newNode('*', left->freq + right->freq);
		top->left = left;
		top->right = right;
		insertToCollection(top, collection);
	}
	//strcpy(saved.charsaved, temps);
	return getMinFreqNode(collection);
}

void startProgramText(){
	char text[MAX_ELMT];
	system("cls");
	printf("========Welcome To HuffMan Generator=======");
	printf("\nThis Is Your First Launch ! Please Insert first text to be generate ");
	printf("\ninput here (Max 10.000 character): "); 
	scanf("%[^\n]s",text); fflush(stdin);
	printf("\ninput saved File Name : "); 
	scanf("%[^\n]s",fileNameGlobal);
	huffman(text);
}
void huffman(char* text) {
	address root;
	root = buildHuffmanTree(text);
	menu(root,text);
}

void menu(address root,char* text){
	int choice;
	bool exit=false;
	while(!exit){
	printf("\n");
	system("pause");
	printf("Press Enter to Back to menu");
	system("cls");
	printf("========Main Menu=======");
	printf("\n1.Show Hufman Tree");
	printf("\n2.Show Hufman Table and Detail");
	printf("\n0.exit");
	printf("\n Input Choice : ");
	scanf("%i",&choice);
		switch(choice){
			case 1	:
					printHuffmanTree(root, root);
					if (whichMenu == true) {
						printf("Current Text :");
						printf("\n%s",text);
					}
					break;
			case 2	:
					encode(root, text);
					break;
			default :
					printf("Invalid input");
					break;
			case 0 :
					exit=true;
					break;
		}
	}
	
}

char* findNode(address root,char find,refrence save[],int saveSize) {
	char* binary = malloc(sizeof(char) * MAX_ELMT);
	STACK *s=InitStack();
	QUEUE code;
	MakeQueue(&code);
	int i;
	int length;
	address current=root;
	while(current!=NULL) {
		if(current->data==find) {
			for (i=0;i<=saveSize-1;i++){
				if(save[i].data==find) {
					length=size(code);
					//save[i].code=malloc(sizeof(char) * length);
					strcpy(save[i].code, Display(code));
				}
			}
			binary = Display(code);
			return binary;
		}
		if(current->left!=NULL){
			if(current->right!=NULL){
			PushStack(&s,current->right);
			}
			Insert(&code,0);
			current=current->left;
		}else {
			if(current->right!=NULL){
				current=current->right;
			}else {
				current=PopStack(&s);
				if(getRear(code)==0){
					setRear(&code,1);	
				}else{
					while(getRear(code)!=0){
						DeleteRear(&code);	
					}
					setRear(&code,1);
				}
			}
		} 
	}
}

void encode(address root, char* text) {
	printf("\n");
	detail(root, text);
}

void detail(address root, char* text) {
	char* charset;
	unsigned int size;
	unsigned int saveSize;
	unsigned int binarySave;
    addressCollection collection;
    FILE *saveFile;
	FILE *fptr;
    char* binaryString = malloc(sizeof(char) * MAX_ELMT);
    int i, codeLength, sum;
    float avgCLength = 0;
    if (whichMenu == true) {
    	charset = characterSet(text);
		size = strlen(charset);
		saveSize = strlen(charset);
		collection = createNodeCollection(text, size, charset);
	} else {
		size = strlen(saved.charsaved);
		saveSize = strlen(saved.charsaved);
		collection = createNodeCollection(NULL, size, saved.charsaved);
	} 
	strcpy(binaryString, "");
	codeLength = sum = 0;
	huffmanTable(root, collection, binaryString, &sum, size, &avgCLength);
	printf("\nText Encoded With Total Code Length %d", sum);
	printf("\nAverage Code Length\t: %.2f\n", avgCLength);
	if (whichMenu == true) {
		refrence save[saveSize];
		refrence test[MAX_ELMT];
		FILE *refrenceFile;
		int j;
		char fromFileNameGlobal[MAX_ELMT];
		strcpy(fromFileNameGlobal,fileNameGlobal);
		refrenceFile=fopen(getRefName(fromFileNameGlobal),"w");
		for(j=0;j<=saveSize-1;j++) {
			save[j].data=charset[j];
			printf("\n%c  banding %c\n",save[j].data,charset[j]);
		}
		printf("Original Text\t\t: %s\n", text);
		printf("Encoded\t\t\t: ");
		for (i = 0; i <= strlen(text) - 1; i++) {
			binaryString = strcat(binaryString, findNode(root, text[i],save,saveSize));
		}
		int count = fwrite(save,sizeof(refrence),saveSize,refrenceFile);
		fclose(refrenceFile);
		printf("%s\n", binaryString);
		printf("Compressed Text\t\t: ");

		char fromFileNameGlobal4[MAX_ELMT];
		strcpy(fromFileNameGlobal4,fileNameGlobal);
		fptr = fopen(getExtendedBit(fromFileNameGlobal4), "w");
		fclose(fptr);

		binaryString=ShowCompressedText(binaryString);
		binarySave=strlen(binaryString);
		char fromFileNameGlobal2[MAX_ELMT];
		strcpy(fromFileNameGlobal2,fileNameGlobal);
		saveFile=fopen(getCompressedName(fromFileNameGlobal2),"w");
		fwrite(binaryString,sizeof(char),binarySave,saveFile);
		fclose(saveFile);
		printf("\nOriginal Text Size\t: %2d byte", strlen(text));
		//printf("\nCompressed Text Size\t: %.f  byte", ceil((float)sum / 8));
		printf("\nCompressed Text Size\t: %.f  byte", (float)binarySave);
		printf("\nCompression Rate\t: %.2f %%", (float) ( ((float)strlen(text) - (float)binarySave) / (float)strlen(text) ) * (float)100);
		char fromFileNameGlobal3[MAX_ELMT];
		strcpy(fromFileNameGlobal3,fileNameGlobal);
		printf("\nYour Compressed File Save As : %s",getCompressedName(fromFileNameGlobal3));
	} 
}

void huffmanTable(address root, addressCollection collection, char* binaryString, int *sum, int size, float *avg) {
	int i = 0;
	int codeLength = 0;
	int sumFreq = 0;
	printf("\n==================================================================\n");
	printf("                          Huffman Table\n==================================================================\n");
	printf("\t||\t       ||     Binary     ||   Bit   ||   Code   ||");
	printf("\n Symbol\t||  Frequency  ||      Code      ||  Length ||  Length  ||\n");
    printf("%s\n","==================================================================");
    for(i = 0; i <= size - 1; i++) {
    	printf("   %c", collection->arr[i]->data);
	    printf("\t\t%d", collection->arr[i]->freq);
		binaryString = findNode1(root, collection->arr[i]->data);
		printf("\t       %5s", binaryString);
		printf("\t       %d", strlen(binaryString));
		codeLength = collection->arr[i]->freq * strlen(binaryString);
		printf("\t   %2d\n", codeLength);
		sumFreq += collection->arr[i]->freq;
		*sum += codeLength;	
    	printf("------------------------------------------------------------------\n");	
	}
	*avg = (float)*sum/(float)sumFreq;
}

char* ShowCompressedText(char* binaryString) {
	FILE *fptr;
	int i,j,k,q;
	int bit;
	int decimal = 0;
	char* result = malloc(sizeof(char)*8);
	char* compressedText = malloc(sizeof(char)*512);
	char* compressedTxt = malloc(sizeof(char)*512);
	int length = strlen(binaryString);
	length -= (length % 8);
	strcpy(compressedText, "");
	strcpy(compressedTxt, "");
	k=0;
	q = 0;
	for (i = 0; i <= strlen(binaryString) - 1; i++) {
		for (j = 0; j <= 7; j++) {
			result[j] = binaryString[k];
			k++;
		}
		decimal = binaryToDecimal(StringToInt(result));
		if (decimal == 10 || decimal == 13) {
			printf(" ");	
		} else {
			printf("%c", decimal);
			strcpy(compressedTxt, append(compressedTxt,decimal));
		}
		if (k == length && length!=strlen(binaryString)) {
			for (i = k; i<= strlen(binaryString) - 1; i++) {
				char fromFileNameGlobal4[MAX_ELMT];
				strcpy(fromFileNameGlobal4,fileNameGlobal);
				fptr = fopen(getExtendedBit(fromFileNameGlobal4), "a");
				fprintf(fptr, "%c", binaryString[i]);
				fclose(fptr);
			}
			i = k;
		}
		if (i==k) {
			return compressedTxt;
		}
		i = k - 1;
	}
	return compressedTxt;//asalnya return compressedTxt disini; 
}

int binaryToDecimal(int i) {
	int  num, binary_val, decimal_val = 0, base = 1, rem;
 	
 	num = i;
    binary_val = num;
    while (num > 0)
    {
        rem = num % 10;
        decimal_val = decimal_val + rem * base;
        num = num / 10 ;
        base = base * 2;
    }
    return decimal_val;
}

int StringToInt(char* binString) {
	int i;
	sscanf(binString, "%d", &i);
	return i;
}

void startProgram() {
	int choice;
		printf("========Huffman Generator=======");
		printf("\n1.Enter Text To Generate");
		printf("\n2.Enter Symbol and Frequency");
		printf("\n3.Compressed from .txt format file");
		printf("\n4.Decompressed text");
		printf("\n0.exit");
		printf("\n Input Choice : ");
		scanf("%d", &choice); fflush(stdin);
		switch (choice) {
			case 1 :
				whichMenu = true;
				startProgramText();
				break;
			case 2 :
				whichMenu = false;
				address root;
				root = buildHuffmanTree(NULL);
				menu(root, NULL);
				break;
			case 3 :
				whichMenu = true;
				readText();
				break;
			case 4 :
				decompressedMenu();
				break;
			case 0 :
				//exit=true;
				printf("Exiting Program...\n");
				break;
			default :
				printf("invalid input\n");
				break;
		}
	
}

addressCollection startWordFreq() {
	whichMenu = false;
	saved.charsaved = malloc(sizeof(char)*255);
	saved.freqsaved = malloc(sizeof(unsigned)*255);
	char charset[CHAR_SET];
	unsigned freqset[CHAR_SET];
	char word;
	unsigned cWord;
	unsigned freq;
	unsigned i;
	unsigned size = 0;
	printf("\nHow Many Words ? "); 
	scanf("%d", &cWord); 
	fflush(stdin);
	for(i = 0; i < cWord; i++) {
		printf("Enter Word\t: "); 
		scanf("%c", &charset[i]); fflush(stdin);
		printf("Enter Frequency\t: "); 
		scanf("%d", &freqset[i]); fflush(stdin);
		printf("\n");
	}
	size = strlen(charset);
	addressCollection collection = createCollection(size);
	for(i = 0; i < size; i++)
		collection->arr[i] = newNode(charset[i], freqset[i]);
	collection->size = size;
	sortCollection(collection);
	strcpy(saved.charsaved, charset);
	for(i=0; i < strlen(saved.charsaved); i++) {
		saved.freqsaved[i] = freqset[i];
	}	
	return collection;
}

void readText(){
		FILE* file;
		system("cls");
		char filename[100];
		printf("========Welcome To HuffMan Generator=======");
		printf("\nThis Is Your First Launch ! Please input the .txt file directory\n(make sure it is on the same folder with this program) ");
		printf("\ninput here (Max 100 character & only .txt format): "); 
		scanf("%s",filename);
		file = fopen(filename,"r");
		
		if( access( filename,R_OK ) != -1 ) {
    	// file exists
    		strcpy(fileNameGlobal,filename);
    		char* test = malloc(sizeof(char) * MAX_ELMT);
					
			while(!feof(file)){
				fgets(test,MAX_ELMT,file);
			}
				fclose(file);
			huffman(test);
		} else {
    	// file doesn't exist
    		printf("File Not Found");	
		}

}

char* findNode1(address root,char find) {
	char* binary = malloc(sizeof(char) * MAX_ELMT);
	STACK *s=InitStack();
	QUEUE code;
	MakeQueue(&code);
	int i;
	address current=root;
	while(current!=NULL) {
		if(current->data==find) {
			binary = Display(code);
			return binary;
		}
		if(current->left!=NULL){
			if(current->right!=NULL){
			PushStack(&s,current->right);
			}
			Insert(&code,0);
			current=current->left;
		}else {
			if(current->right!=NULL){
				current=current->right;
			}else {
				current=PopStack(&s);
				if(getRear(code)==0){
					setRear(&code,1);	
				}else{
					while(getRear(code)!=0){
						DeleteRear(&code);	
					}
					setRear(&code,1);
				}
			}
		} 
	}
}

char* decimalToBinary(int n) {
	int i, bn;
	char* bin = malloc(sizeof(char)*9);
	bin[8] = '\0';
	for (i = 7; i >= 0; i--) {
		bn = n >> i;
		if (bn & 1)
			bin[i] = '1';
		else
			bin[i] = '0';
	}
	reverse_string(bin);
	return bin;
}

void test() { // BUAT NGE TEST DOANG
	FILE *fptr;
	refrence save[3];
	int i;
	fptr = fopen("testref.txt", "r");
	int count = fread(save, sizeof(refrence), 3, fptr);

	for ( i = 0;i<3;i++) {
		printf("%c\n", save[i].data);
		printf("%s\n", save[i].code);
	}
	fclose(fptr);
}

char* getBinaryFromCompressed(char fileName[],char extendedBit[]) {
	FILE *fp;
	int lengthBinaryString = 0;
	char binaryString[MAX_ELMT];
	char sisabit[CHAR_SET];
	char* decodedBinaryText = malloc(sizeof(char)*MAX_ELMT);
	int i;
	strcpy(decodedBinaryText, "");
	fp = fopen(fileName, "r");
	int count = fread(binaryString, sizeof(char), MAX_ELMT, fp);
	lengthBinaryString = strlen(binaryString);
	//printf("%s",binaryString);
	for (i = 0; i <= lengthBinaryString - 1; i++) {
		strcat(decodedBinaryText, decimalToBinary(binaryString[i]));
	}
	fclose(fp);
	if( access(extendedBit, F_OK ) != -1 ) {
    	fp = fopen(extendedBit, "r");
		int count = fread(sisabit, sizeof(char), CHAR_SET, fp);
		fclose(fp);
		strcat(decodedBinaryText, sisabit);
	} else {
    	// file doesn't exist
	}
	return decodedBinaryText;
}

refrence* getRequirementForDecode(char refFile[]) {
	FILE *fptr;
	refrence* save = malloc(sizeof(refrence)*CHAR_SET);
	//refrence save[CHAR_SET];
	fptr = fopen(refFile, "r");
	int count = fread(save, sizeof(refrence), CHAR_SET, fptr);
	ref_idx = count;
	fclose(fptr);
	return save;
}


void decompressedMenu(){
	char fileName[MAX_ELMT];
	system("cls");
	printf("Input Compressed text File Name (ex textcom.txt) :");
	scanf("%s",fileName);
	decode(fileName);
	
}

char* decode(char fileName[]) {
	int codeLength, i, j, size, temp, countloop;
	char binaryString[MAX_ELMT];
	char compName[MAX_ELMT];
	strcpy(compName,fileName);
	fileName=getRealName(fileName);
	char refName[MAX_ELMT];
	strcpy(refName,fileName);
	getRefName(refName);
	char bitName[MAX_ELMT];
	strcpy(bitName,fileName);
	getExtendedBit(bitName);
	//printf("%s,%s,%s,%s\n\n",fileName,compName,refName,bitName);
	char* bantu = malloc(sizeof(char)*20);
	char* decodedString = malloc(sizeof(char)*MAX_ELMT);
	refrence* ref = malloc(sizeof(refrence)*CHAR_SET);
	//int temp;
	
	//STACKINT *s=InitStack();
	int idx_now = 0;
	j = 0;
	i = 0;
	ref = getRequirementForDecode(refName);
	size = ref_idx;
	strcpy(binaryString, getBinaryFromCompressed(compName,bitName));
	codeLength = strlen(binaryString);
	strcpy(decodedString, "");
	//printf(" adalah %s",binaryString);
	bool forloop;
	for (i=0; i<=codeLength-1;i++) {
		temp = i;
		idx_now = i;
		forloop = false;
		int k;
		strcpy(bantu, "");
		strcpy(bantu, append(bantu, binaryString[i]));
		j = 0;
		while (j<=size-1) {
			for (k = 0; k<=size-1;k++) {
				if (!forloop) {
					if (strcmp(bantu, ref[k].code) == 0) {
					forloop = true;
					strcpy(decodedString, append(decodedString, ref[k].data));
					break;	
				}
			}
		}
			j++;
			if (!forloop) {
				i++;
				idx_now = i;
				strcpy(bantu, append(bantu, binaryString[idx_now]));
			}
		}
		if(idx_now!=0)
			i = idx_now;
		
	}
	printf("\nteks setelah dekompresi adalah:\n");
	printf("%s",decodedString);
	saveDecompressedFile(decodedString);
	//int damage = remove(bitName);
	//damage = remove(compName);
	//damage = remove(refName);
	return decodedString;
}

char* reverse_string(char *str)
{
    char temp;
    size_t len = strlen(str) - 1;
    size_t i;
    size_t k = len;

    for(i = 0; i < len; i++)
    {
        temp = str[k];
        str[k] = str[i];
        str[i] = temp;
        k--;

        /* As 2 characters are changing place for each cycle of the loop
           only traverse half the array of characters */
        if(k == (len / 2))
        {
            break;
        }
    }
}

void saveDecompressedFile(char* decoded){
	FILE *save;
	int length;
	char* savedFile=malloc(sizeof(char)*MAX_ELMT);
	printf("\n WARNING : File Has to be save as .txt !");
	printf("Save As : ");
	scanf("%s",savedFile);
	save=fopen(savedFile,"w");
	length=strlen(decoded);
	fwrite(decoded,sizeof(char),length,save);
	fclose(save);
}

char* getRefName(char fileName[]){
	int length;
	length=strlen(fileName);
	length=length-4;
	fileName[length]='r';
	length++;
	fileName[length]='e';
	length++;
	fileName[length]='f';
	length++;
	fileName[length]='.';
	length++;
	fileName[length]='t';
	length++;
	fileName[length]='x';
	length++;
	fileName[length]='t';
	return fileName;
}

char* getRealName(char fileName[]){
	int length;
	int i;
	length=strlen(fileName);
	char realName[MAX_ELMT];
	length=length-7;
	fileName[length]='.';
	length++;
	fileName[length]='t';
	length++;
	fileName[length]='x';
	length++;
	fileName[length]='t';
	for(i=0;i<=length;i++){
		realName[i]=fileName[i];
	}
	strcpy(fileName,"");
	strcpy(fileName,realName);
	return fileName;
}

char* getExtendedBit(char fileName[]){
	int length;
	length=strlen(fileName);
	length=length-4;
	fileName[length]='e';
	length++;
	fileName[length]='x';
	length++;
	fileName[length]='t';
	length++;
	fileName[length]='.';
	length++;
	fileName[length]='t';
	length++;
	fileName[length]='x';
	length++;
	fileName[length]='t';
	return fileName;
}


char* getCompressedName(char fileName[]){
	int length;
	length=strlen(fileName);
	length=length-4;
	fileName[length]='c';
	length++;
	fileName[length]='o';
	length++;
	fileName[length]='m';
	length++;
	fileName[length]='.';
	length++;
	fileName[length]='t';
	length++;
	fileName[length]='x';
	length++;
	fileName[length]='t';
	return fileName;
}
