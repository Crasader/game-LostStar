//以下是实现文件


//#include <stdafx.h>
#include <string>
#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <stdlib.h>
#include "AES.h"

using namespace std;

#define PLAIN_FILE_OPEN_ERROR -1
#define KEY_FILE_OPEN_ERROR -2
#define CIPHER_FILE_OPEN_ERROR -3
#define OK 1 
#define PARAM_ERROR  -1

//////////////////////////////////////////////////////////////////////////



/*初始置换表IP*/
int IP_Table[64] = {  57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7,
	56,48,40,32,24,16,8,0,
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6}; 
/*逆初始置换表IP^-1*/
int IP_1_Table[64] = {39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25,
	32,0,40,8,48,16,56,24};

/*扩充置换表E*/
int E_Table[48] = {31, 0, 1, 2, 3, 4,
	3,  4, 5, 6, 7, 8,
	7,  8,9,10,11,12,
	11,12,13,14,15,16,
	15,16,17,18,19,20,
	19,20,21,22,23,24,
	23,24,25,26,27,28,
	27,28,29,30,31, 0};

/*置换函数P*/
int P_Table[32] = {15,6,19,20,28,11,27,16,
	0,14,22,25,4,17,30,9,
	1,7,23,13,31,26,2,8,
	18,12,29,5,21,10,3,24};

/*S盒*/
int S[8][4][16] =
	/*S1*/
{{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
/*S2*/
{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
/*S3*/
{{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
/*S4*/
{{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
/*S5*/
{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
/*S6*/
{{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
/*S7*/
{{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
/*S8*/
{{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}};
/*置换选择1*/
int PC_1[56] = {56,48,40,32,24,16,8,
	0,57,49,41,33,25,17,
	9,1,58,50,42,34,26,
	18,10,2,59,51,43,35,
	62,54,46,38,30,22,14,
	6,61,53,45,37,29,21,
	13,5,60,52,44,36,28,
	20,12,4,27,19,11,3};

/*置换选择2*/
int PC_2[48] = {13,16,10,23,0,4,2,27,
	14,5,20,9,22,18,11,3,
	25,7,15,6,26,19,12,1,
	40,51,30,36,46,54,29,39,
	50,44,32,46,43,48,38,55,
	33,52,45,41,49,35,28,31};

/*对左移次数的规定*/
int MOVE_TIMES[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1}; 

//////////////////////////////////////////////////////////////////////////

AES::AES()
{

}
// AES::AES(const char *cchKey)
// {
// 
// }
AES::~AES()
{

}



/*字节转换成二进制*/
int AES::ByteToBit(char ch, char bit[8]){
	if (bit == NULL)
	{
		return PARAM_ERROR;
	}
	int cnt = 0;
	for(cnt = 0;cnt < 8; cnt++){
		*(bit+cnt) = (ch>>cnt)&1;
	}
	return 0;
}

/*二进制转换成字节*/
int AES::BitToByte(char bit[8],char *ch){
	if ((ch == NULL) || (bit == NULL))
	{
		return PARAM_ERROR;
	}
	int cnt = 0;
	for(cnt = 0;cnt < 8; cnt++){
		*ch |= *(bit + cnt)<<cnt;
	}
	return 0;
}

/*将长度为8的字符串转为二进制位串*/
int AES::Char8ToBit64(char ch[8],char bit[64]){
	if ((ch == NULL) || (bit == NULL))
	{
		return PARAM_ERROR;
	}

	int cnt = 0;
	for(cnt = 0; cnt < 8; cnt++){  
		ByteToBit(*(ch+cnt),bit+(cnt<<3));
	}
	return 0;
}

/*将二进制位串转为长度为8的字符串*/
int AES::Bit64ToChar8(char bit[64],char ch[8]){
	if ((ch == NULL) || (bit == NULL))
	{
		return PARAM_ERROR;
	}

	int cnt = 0;
	memset(ch,0,8);
	for(cnt = 0; cnt < 8; cnt++){
		BitToByte(bit+(cnt<<3),ch+cnt);
	}
	return 0;
}

/*生成子密钥*/
int AES::DES_MakeSubKeys(char key[64],char subKeys[16][48]){
	if ((key == NULL) || (subKeys == NULL))
	{
		return PARAM_ERROR;
	}

	char temp[56] = {0};
	int cnt = 0;
	if(DES_PC1_Transform(key,temp) < 0)/*PC1置换*/
	{
		return -2;
	}
	for(cnt = 0; cnt < 16; cnt++){/*16轮跌代，产生16个子密钥*/
		DES_ROL(temp,MOVE_TIMES[cnt]);/*循环左移*/
		DES_PC2_Transform(temp,subKeys[cnt]);/*PC2置换，产生子密钥*/
	}
	return 0;
}

/*密钥置换1*/
int AES::DES_PC1_Transform(char key[64], char tempbts[56]){
	if ((key == NULL) || (tempbts == NULL))
	{
		return PARAM_ERROR;
	}

	int cnt = 0; 
	for(cnt = 0; cnt < 56; cnt++){
		tempbts[cnt] = key[PC_1[cnt]];
	}
	return 0;
}

/*密钥置换2*/
int AES::DES_PC2_Transform(char key[56], char tempbts[48]){
	if ((key == NULL) || (tempbts == NULL))
	{
		return PARAM_ERROR;
	}

	int cnt = 0;
	for(cnt = 0; cnt < 48; cnt++){
		tempbts[cnt] = key[PC_2[cnt]];
	}
	return 0;
}

/*循环左移*/
int AES::DES_ROL(char data[56], int time){
	if ((data == NULL) || (time < 0))
	{
		return PARAM_ERROR;
	}

	char temp[56] = {0};

	/*保存将要循环移动到右边的位*/
	memcpy(temp,data,time);
	memcpy(temp+time,data+28,time);

	/*前28位移动*/
	memcpy(data,data+time,28-time);
	memcpy(data+28-time,temp,time);

	/*后28位移动*/
	memcpy(data+28,data+28+time,28-time);
	memcpy(data+56-time,temp+time,time); 

	return 0;
}

/*IP置换*/
int AES::DES_IP_Transform(char data[64]){
	if (data == NULL)
	{
		return PARAM_ERROR;
	}
	int cnt = 0;
	char temp[64] = {0};
	for(cnt = 0; cnt < 64; cnt++){
		temp[cnt] = data[IP_Table[cnt]];
	}
	memcpy(data,temp,64);
	return 0;
}

/*IP逆置换*/
int AES::DES_IP_1_Transform(char data[64]){
	if (data == NULL)
	{
		return PARAM_ERROR;
	}
	int cnt = 0;
	char temp[64] = {0};
	for(cnt = 0; cnt < 64; cnt++){
		temp[cnt] = data[IP_1_Table[cnt]];
	}
	memcpy(data,temp,64);
	return 0;
}

/*扩展置换*/
int AES::DES_E_Transform(char data[48]){
	if (data == NULL)
	{
		return PARAM_ERROR;
	}
	int cnt = 0;
	char temp[48] = {0};
	for(cnt = 0; cnt < 48; cnt++){
		temp[cnt] = data[E_Table[cnt]];
	}
	memcpy(data,temp,48);
	return 0;
}

/*P置换*/
int AES::DES_P_Transform(char data[32]){
	if (data == NULL)
	{
		return PARAM_ERROR;
	}
	int cnt = 0;
	char temp[32] = {0};
	for(cnt = 0; cnt < 32; cnt++){
		temp[cnt] = data[P_Table[cnt]];
	}
	memcpy(data,temp,32);
	return 0;
}

/*异或*/
int AES::DES_XOR(char R[48], char L[48] ,int count){
	if ((R == NULL) ||  (L == NULL))
	{
		return PARAM_ERROR;
	}
	int cnt = 0;
	for(cnt = 0; cnt < count; cnt++){
		R[cnt] ^= L[cnt];
	}
	return 0;
}

/*S盒置换*/
int AES::DES_SBOX(char data[48]){
	if(data == NULL)
	{
		return PARAM_ERROR;
	}

	int cnt = 0;
	int line = 0;
	int row = 0;
	int output = 0;
	int cur1 = 0,cur2 = 0;
	for(cnt = 0; cnt < 8; cnt++){
		cur1 = cnt*6;
		cur2 = cnt<<2;

		/*计算在S盒中的行与列*/
		line = (data[cur1]<<1) + data[cur1+5];
		row = (data[cur1+1]<<3) + (data[cur1+2]<<2)
			+ (data[cur1+3]<<1) + data[cur1+4];
		output = S[cnt][line][row];

		/*化为2进制*/
		data[cur2] = (output&0X08)>>3;
		data[cur2+1] = (output&0X04)>>2;
		data[cur2+2] = (output&0X02)>>1;
		data[cur2+3] = output&0x01;
	}
	return 0;
}

/*交换*/
int AES::DES_Swap(char left[32], char right[32]){
	if((left == NULL) || (right == NULL))
	{
		return PARAM_ERROR;
	}

	char temp[32] = {0};
	memcpy(temp,left,32);
	memcpy(left,right,32);  
	memcpy(right,temp,32);
	return 0;
}

/*加密单个分组*/
int AES::DES_EncryptBlock(char plainBlock[8], char subKeys[16][48], char cipherBlock[8]){

	if((plainBlock == NULL) || (subKeys == NULL) || (cipherBlock == NULL))
	{
		return PARAM_ERROR;
	}

	char plainBits[64] = {0};
	char copyRight[48] = {0};
	int cnt = 0;

	Char8ToBit64(plainBlock,plainBits);  
	/*初始置换（IP置换）*/
	DES_IP_Transform(plainBits);

	/*16轮迭代*/
	for(cnt = 0; cnt < 16; cnt++){ 
		memcpy(copyRight,plainBits+32,32);
		/*将右半部分进行扩展置换，从32位扩展到48位*/
		DES_E_Transform(copyRight);
		/*将右半部分与子密钥进行异或操作*/
		DES_XOR(copyRight,subKeys[cnt],48); 
		/*异或结果进入S盒，输出32位结果*/
		DES_SBOX(copyRight);
		/*P置换*/
		DES_P_Transform(copyRight);
		/*将明文左半部分与右半部分进行异或*/
		DES_XOR(plainBits,copyRight,32);
		if(cnt != 15){
			/*最终完成左右部的交换*/
			DES_Swap(plainBits,plainBits+32);
		}
	}
	/*逆初始置换（IP^1置换）*/
	DES_IP_1_Transform(plainBits);
	Bit64ToChar8(plainBits,cipherBlock);
	return 0;
}

/*解密单个分组*/
int AES::DES_DecryptBlock(char cipherBlock[8], char subKeys[16][48],char plainBlock[8]){
	if((plainBlock == NULL) || (subKeys == NULL) || (cipherBlock == NULL))
	{
		return PARAM_ERROR;
	}

	char cipherBits[64] = {0};
	char copyRight[48] = {0};
	int cnt = 0;

	Char8ToBit64(cipherBlock,cipherBits); 
	/*初始置换（IP置换）*/
	DES_IP_Transform(cipherBits);

	/*16轮迭代*/
	for(cnt = 15; cnt >= 0; cnt--){
		memcpy(copyRight,cipherBits+32,32);
		/*将右半部分进行扩展置换，从32位扩展到48位*/
		DES_E_Transform(copyRight);
		/*将右半部分与子密钥进行异或操作*/
		DES_XOR(copyRight,subKeys[cnt],48);  
		/*异或结果进入S盒，输出32位结果*/
		DES_SBOX(copyRight);
		/*P置换*/
		DES_P_Transform(copyRight);  
		/*将明文左半部分与右半部分进行异或*/
		DES_XOR(cipherBits,copyRight,32);
		if(cnt != 0){
			/*最终完成左右部的交换*/
			DES_Swap(cipherBits,cipherBits+32);
		}
	}
	/*逆初始置换（IP^1置换）*/
	DES_IP_1_Transform(cipherBits);
	Bit64ToChar8(cipherBits,plainBlock);
	return 0;
}

/*
功能：加密文件
参数：
[cszpSourceFileName]:原数据文件名
[cszpPwdFileName]:密码文件名
[cszpKey]:密码
*/

int AES::AES_EncryptFile(
	const char *cszpSourceFileName, 
	const char *cszpPwdFileName, 
	const char *cszpKey)
{
	if ((cszpSourceFileName == NULL) || 
		(cszpPwdFileName == NULL) || 
		(cszpKey == NULL))
	{
		return PARAM_ERROR;
	}
	FILE *plain = NULL,*cipher = NULL;
	int count = 0;
	char plainBlock[8] = {0};
	char cipherBlock[8] = {0};
	char keyBlock[8] = {0};
	char bKey[64] = {0};
	char subKeys[16][48] = {0};
	if((plain = fopen(cszpSourceFileName,("rb"))) == NULL){
		return PLAIN_FILE_OPEN_ERROR;
	}
	if((cipher = fopen(cszpPwdFileName,"wb")) == NULL){
		return CIPHER_FILE_OPEN_ERROR;
	}
	/*设置密钥*/
	memcpy(keyBlock,cszpKey,8);
	/*将密钥转换为二进制流*/
	Char8ToBit64(keyBlock,bKey);
	/*生成子密钥*/
	DES_MakeSubKeys(bKey,subKeys);

	while(!feof(plain)){
		/*每次读8个字节，并返回成功读取的字节数*/
		if((count = fread(plainBlock,sizeof(char),8,plain)) == 8){
			DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
			fwrite(cipherBlock,sizeof(char),8,cipher);  
		}
	}
	if(count){
		/*填充*/
		memset(plainBlock + count,'\0',7 - count);
		/*最后一个字符保存包括最后一个字符在内的所填充的字符数量*/
		plainBlock[7] = 8 - count;
		DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
		fwrite(cipherBlock,sizeof(char),8,cipher);
	}
	fclose(plain);
	plain = NULL;
	fclose(cipher);
	cipher = NULL;
	return OK;
}

/*
功能：解密文件
参数：
[cszpPwdFileName]:密码文件名
[cszpResultFileName]:解密后数据文件名
[cszpKey]:密码
*/
int AES::AES_DecryptFile(
	const char *cszpPwdFileName, 
	const char *cszpResultFileName, 
	const char *cszpKey)
{
	if ((cszpResultFileName == NULL) || 
		(cszpPwdFileName == NULL) || 
		(cszpKey == NULL))
	{
		return PARAM_ERROR;
	}

	FILE *plain = NULL, *cipher = NULL;
	int count,times = 0;
	long fileLen = 0;
	char plainBlock[8] = {0};
	char cipherBlock[8] = {0};
	char keyBlock[8] = {0};
	char bKey[64] = {0};
	char subKeys[16][48] = {0};
	if((cipher = fopen(cszpPwdFileName,"rb")) == NULL){
		return CIPHER_FILE_OPEN_ERROR;
	}
	if((plain = fopen(cszpResultFileName,"wb")) == NULL){
		return PLAIN_FILE_OPEN_ERROR;
	}

	/*设置密钥*/
	memcpy(keyBlock,cszpKey,8);
	/*将密钥转换为二进制流*/
	Char8ToBit64(keyBlock,bKey);
	/*生成子密钥*/
	DES_MakeSubKeys(bKey,subKeys);

	/*取文件长度 */
	fseek(cipher,0,SEEK_END);/*将文件指针置尾*/
	fileLen = ftell(cipher); /*取文件指针当前位置*/
	rewind(cipher); /*将文件指针重指向文件头*/
	while(1){
		/*密文的字节数一定是8的整数倍*/
		fread(cipherBlock,sizeof(char),8,cipher);
		DES_DecryptBlock(cipherBlock,subKeys,plainBlock);  
		times += 8;
		if(times < fileLen){
			fwrite(plainBlock,sizeof(char),8,plain);
		}
		else{
			break;
		}
	}
	/*判断末尾是否被填充*/
	if(plainBlock[7] < 8){
		for(count = 8 - plainBlock[7]; count < 7; count++){
			if(plainBlock[count] != '\0'){
				break;
			}
		}
	}
	if(count == 7){/*有填充*/
		fwrite(plainBlock,sizeof(char),8 - plainBlock[7],plain);
	}
	else{/*无填充*/
		fwrite(plainBlock,sizeof(char),8,plain);
	}

	fclose(plain);
	plain = NULL;
	fclose(cipher);
	cipher = NULL;
	return OK;
}  


bool AES::ReadFileAndDecryptText(const char *cszpPwdFileName,const char *cszpKey,string& out_text)
{
	FILE *cipher = NULL;
	int count,times = 0;
	long fileLen = 0;
	char plainBlock[8] = {0};
	char cipherBlock[8] = {0};
	char keyBlock[8] = {0};
	char bKey[64] = {0};
	char subKeys[16][48] = {0};
	if((cipher = fopen(cszpPwdFileName,"rb")) == NULL){
		return false;
	}


	/*设置密钥*/
	memcpy(keyBlock,cszpKey,8);
	/*将密钥转换为二进制流*/
	Char8ToBit64(keyBlock,bKey);
	/*生成子密钥*/
	DES_MakeSubKeys(bKey,subKeys);

	/*取文件长度 */
	fseek(cipher,0,SEEK_END);/*将文件指针置尾*/
	fileLen = ftell(cipher); /*取文件指针当前位置*/
	rewind(cipher); /*将文件指针重指向文件头*/

	int newLength = fileLen+16;
	char* temp = new char[newLength];
	memset(temp,0,newLength);
	while(1){
		/*密文的字节数一定是8的整数倍*/
		fread(cipherBlock,sizeof(char),8,cipher);
		DES_DecryptBlock(cipherBlock,subKeys,plainBlock);  
		times += 8;
		if(times < fileLen){
			memcpy(temp+times,plainBlock,sizeof(char)*8);
			//fwrite(plainBlock,sizeof(char),8,plain);
		}
		else{
			break;
		}
	}
	/*判断末尾是否被填充*/
	if(plainBlock[7] < 8){
		for(count = 8 - plainBlock[7]; count < 7; count++){
			if(plainBlock[count] != '\0'){
				break;
			}
		}
	}
	if(count == 7){/*有填充*/
		//fwrite(plainBlock,sizeof(char),8 - plainBlock[7],plain);
		memcpy(temp+times,plainBlock,sizeof(char) *(8 - plainBlock[7]));
	}
	else{/*无填充*/
		//fwrite(plainBlock,sizeof(char),8,plain);
		memcpy(temp+times,plainBlock,sizeof(char) *8);
	}

	fclose(cipher);
	cipher = NULL;

	int move = 0;
	while (temp[move] == 0 && move < newLength)
		move++;
	out_text = string(temp+move);
	delete [] temp;
	return true;
}
bool AES::saveFileAndEncryptText(const char *cszpPwdFileName,const char *cszpKey,string in_text)
{
	if ((cszpPwdFileName == NULL) || 
		(cszpKey == NULL))
	{
		return false;
	}
	FILE *cipher = NULL;
	int count = 0;
	char plainBlock[8] = {0};
	char cipherBlock[8] = {0};
	char keyBlock[8] = {0};
	char bKey[64] = {0};
	char subKeys[16][48] = {0};
	if((cipher = fopen(cszpPwdFileName,"wb")) == NULL){
		return false;
	}

	int newLength = in_text.length();
	int more = newLength % 8; // 计算多余的字节
	newLength += 8-more;	// 补齐
	char * temp = new char[newLength];
	memset(temp,0,newLength);
	memcpy(temp,in_text.c_str(),in_text.length());

	/*设置密钥*/
	memcpy(keyBlock,cszpKey,8);
	/*将密钥转换为二进制流*/
	Char8ToBit64(keyBlock,bKey);
	/*生成子密钥*/
	DES_MakeSubKeys(bKey,subKeys);


	int index = 0;
	while(index < newLength)
	{
		DES_EncryptBlock(temp+index,subKeys,cipherBlock);
		fwrite(cipherBlock,sizeof(char),8,cipher);  
		index += 8;
	}
	//while(!feof(plain)){
	//	/*每次读8个字节，并返回成功读取的字节数*/
	//	if((count = fread(plainBlock,sizeof(char),8,plain)) == 8){
	//		DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
	//		fwrite(cipherBlock,sizeof(char),8,cipher);  
	//	}
	//}
	//if(count){
	//	/*填充*/
	//	memset(plainBlock + count,'\0',7 - count);
	//	/*最后一个字符保存包括最后一个字符在内的所填充的字符数量*/
	//	plainBlock[7] = 8 - count;
	//	DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
	//	fwrite(cipherBlock,sizeof(char),8,cipher);
	//}
	fclose(cipher);
	cipher = NULL;
	return true;
}


/*
功能：加密字符串
参数：
[pszInText]:要加密的字符串
[cszpKey]:密码
*/
string AES::EncryptText(
	const char *pszInText, 
	const char *cszpKey)
{
	if ((pszInText == NULL) || 
		(cszpKey == NULL))
	{
		return "";
	}

	int count = 0;
	char plainBlock[9] = {0};
	char cipherBlock[9] = {0};
	char keyBlock[9] = {0};
	char bKey[64] = {0};
	char subKeys[16][48] = {0};

	int nSize = strlen(pszInText);
	char *pchData = (char *)malloc(nSize + 1);
	if (pchData == NULL)
	{
		return "";
	}

	memset(pchData, 0, nSize + 1);
	memcpy(pchData, pszInText, nSize);

	/*设置密钥*/
	memcpy(keyBlock,cszpKey,8);
	/*将密钥转换为二进制流*/
	Char8ToBit64(keyBlock,bKey);
	/*生成子密钥*/
	DES_MakeSubKeys(bKey,subKeys);

	string strData;
	char *pch = pchData;

	do 
	{
		memset(plainBlock, 0, 9);
		memcpy(plainBlock, pch, 8);
		int nLen = strlen(plainBlock);
		if (nLen < 8)
		{
			count = nLen;
			break;
		}
		else
		{
			DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
			strData += cipherBlock;
			pch += 8;
		}
	} while (true);

	if(count){
		/*填充*/
		memset(plainBlock + count,'\0',7 - count);
		/*最后一个字符保存包括最后一个字符在内的所填充的字符数量*/
		plainBlock[7] = 8 - count;
		DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
		strData += cipherBlock;
	}

	return strData;
}

/*
功能：解密字符串
参数：
[pszInText]:要解密的字符串，即加密后的字符串
[cszpKey]:密码
*/
string AES::DecryptText(
	const char *pszInText, 
	const char *cszpKey)
{
	if ((pszInText == NULL) || 
		(cszpKey == NULL))
	{
		return "";
	}

	int count = 0;
	int times = 0;
	char plainBlock[9] = {0};
	char cipherBlock[9] = {0};
	char keyBlock[9] = {0};
	char bKey[64] = {0};
	char subKeys[16][48] = {0};


	int nSize = strlen(pszInText);
	char *pchData = (char *)malloc(nSize + 1);
	if (pchData == NULL)
	{
		return "";
	}

	memset(pchData, 0, nSize + 1);
	memcpy(pchData, pszInText, nSize);

	/*设置密钥*/
	memcpy(keyBlock,cszpKey,8);
	/*将密钥转换为二进制流*/
	Char8ToBit64(keyBlock,bKey);
	/*生成子密钥*/
	DES_MakeSubKeys(bKey,subKeys);

	string strData;
	char *pch = pchData;

	while(true){
		/*密文的字节数一定是8的整数倍*/
		memset(cipherBlock, 0, 9);
		memset(plainBlock, 0, 9);
		memcpy(cipherBlock,pch,8);
		DES_DecryptBlock(cipherBlock,subKeys,plainBlock);
		times += 8;
		pch += 8;
		if(times < nSize){
			strData += plainBlock;
		}
		else{
			break;
		}
	}
	/*判断末尾是否被填充*/
	if(plainBlock[7] < 8){
		for(count = 8 - plainBlock[7]; count < 7; count++){
			if(plainBlock[count] != '\0'){
				break;
			}
		}
	}
	if(count == 7){/*有填充*/
		memset(cipherBlock, 0, 9);
		memcpy(cipherBlock,plainBlock,8 - plainBlock[7]);
		strData += cipherBlock;
	}
	else{/*无填充*/
		memset(cipherBlock, 0, 9);
		memcpy(cipherBlock,plainBlock,8);
		strData += cipherBlock;
	}

	return strData;
}




/*
功能：
备份数据至指定的文件中
参数：
【cszpData】：要备份的数据
【cszpKey】：用于对数据加密的密码
【cszpFileName】：保存数据的文件名
返回值：
成功：大于等于零（>=0）
失败：小于零（<0）
*/
int AES::BackUp(const char *cszpData, const char *cszpKey, const char *cszpFileName)
{
	if ((cszpData == NULL) ||
		(cszpKey == NULL) ||
		(cszpFileName == NULL))
	{
		return PARAM_ERROR;
	}

	string strData = EncryptText(cszpData, cszpKey);
	if (strData.empty())
	{
		return -2;
	}

	FILE *pfile = NULL;
	if((pfile = fopen(cszpFileName,"wb")) == NULL){
		return CIPHER_FILE_OPEN_ERROR;
	}

	fwrite(strData.c_str(), strlen(strData.c_str()), 1, pfile);
	fclose(pfile);
	pfile = NULL;

	return 0;
}

//恢复备份的数据
/*
功能：
从指定的文件中恢复备份的数据
参数：
【cszpKey】：用于对数据解密的密码
【cszpFileName】：保存数据的文件名
返回值：
成功：解密后的数据
失败：空值
*/
string AES::Recover(const char *cszpKey, const char *cszpFileName)
{
	if ((cszpKey == NULL) || (cszpFileName == NULL))
	{
		return "";
	}

	string strData;

	FILE *pfile = NULL;
	if ((pfile = fopen(cszpFileName, "rb")) == NULL)
	{
		return "";
	}
	/*取文件长度 */
	fseek(pfile,0,SEEK_END);/*将文件指针置尾*/
	long fileLen = ftell(pfile); /*取文件指针当前位置*/
	rewind(pfile); /*将文件指针重指向文件头*/

	char *szpFileData = (char *)malloc(fileLen + 1);
	if (szpFileData == NULL)
	{
		fclose(pfile);
		pfile = NULL;
		return "";
	}

	memset(szpFileData, 0, fileLen + 1);
	fread(szpFileData, fileLen, 1, pfile);
	fclose(pfile);

	strData = DecryptText(szpFileData, cszpKey);

	return strData;
}
