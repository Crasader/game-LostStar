#ifndef AES_H_
#define AES_H_

#include <string>

using std::string;

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

class AES
{
public:
	AES();
	//AES(const char *cchKey);
	~AES();

public:
	//加密文件数据
	int AES_EncryptFile(
		const char *cszpSourceFileName, 
		const char *cszpPwdFileName, 
		const char *cszpKey);
	//解密文件数据
	int AES_DecryptFile(
		const char *cszpPwdFileName, 
		const char *cszpResultFileName, 
		const char *cszpKey);  
	//加密字符串
	string EncryptText(
		const char *pszInText, 
		const char *cszpKey);
	//解密字符串
	string DecryptText(
		const char *pszInText, 
		const char *cszpKey);

	// 读取加密的文件，并且解密，然后返回
	bool ReadFileAndDecryptText(const char *cszpPwdFileName,const char *cszpKey,string& out_text);
	bool saveFileAndEncryptText(const char *cszpPwdFileName,const char *cszpKey,string in_text);

	//备份数据
	int BackUp(const char *cszpData, const char *cszpKey, const char *cszpFileName);
	//恢复备份的数据
	string Recover(const char *cszpKey, const char *cszpFileName);

private:

	int ByteToBit(char ch,char bit[8]);
	int BitToByte(char bit[8],char *ch);
	int Char8ToBit64(char ch[8],char bit[64]);
	int Bit64ToChar8(char bit[64],char ch[8]);
	int DES_MakeSubKeys(char key[64],char subKeys[16][48]);
	int DES_PC1_Transform(char key[64], char tempbts[56]);
	int DES_PC2_Transform(char key[56], char tempbts[48]);
	int DES_ROL(char data[56], int time);
	int DES_IP_Transform(char data[64]);
	int DES_IP_1_Transform(char data[64]);
	int DES_E_Transform(char data[48]);
	int DES_P_Transform(char data[32]);
	int DES_SBOX(char data[48]);
	int DES_XOR(char R[48], char L[48],int count);
	int DES_Swap(char left[32],char right[32]);
	int DES_EncryptBlock(char plainBlock[8], char subKeys[16][48], char cipherBlock[8]);
	int DES_DecryptBlock(char cipherBlock[8], char subKeys[16][48], char plainBlock[8]);

};

#endif /* AES_H_ */