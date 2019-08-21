#include <iostream>
//#include "string.h"
#include <cstring>
#include "ippcp.h"

typedef unsigned char  Ipp8u;
using namespace std;

void printCharArray(Ipp8u *p) {
	cout << "[-] printCharArray" << endl;
	//while(p && p != '\0') {
	while(p && *p != '\0') {
		cout << *p++;
		//cout << *p++ << " ";
		//printf("0x%02x ", *p++);
	}
	cout << endl;
}

int aes_sample(void) {

	// error
	//Ipp8u key[] = "0x000x010x02";

	// ok
	Ipp8u key[] = "\xFF\x01\x02\x03\x04\x05\x06\x07"
			"\x08\x09\x10\x11\x12\x13\x14\x15";

	// AES
	int ctxSize;
	ippsAESGetSize(&ctxSize);
	IppsAESSpec* pAES = (IppsAESSpec *)(new Ipp8u[ctxSize]);
	ippsAESInit(key, sizeof(key) - 1, pAES, ctxSize);

	// content
	Ipp8u content[] = "I am alien, I am legal alien, I am an Englishman in New York!";
	cout << "sizeof content:\t" << sizeof(content) << endl;
	printCharArray(content);

	// counter
	Ipp8u ctr0[] = "\xff\xee\xdd\xcc\xbb\xaa\x99\x88"
			"\x77\x66\x55\x44\x33\x22\x11\x00";
	Ipp8u ctr[16];

	// encryption
	Ipp8u encrypted[sizeof(content)];
	memcpy(ctr, ctr0, sizeof(ctr));
	ippsAESEncryptCTR(content, encrypted, sizeof(content), pAES, ctr, 64);
	cout << "sizeof encrypted:\t" << sizeof(encrypted) << endl;
	printCharArray(encrypted);
	
	// decryption
	Ipp8u decrypted[sizeof(encrypted)];
	memcpy(ctr, ctr0, sizeof(ctr)) ;
	ippsAESDecryptCTR(encrypted, decrypted, sizeof(encrypted), pAES, ctr, 64);
	cout << "sizeof decrypted:\t" << sizeof(decrypted) << endl;
	printCharArray(decrypted);

	return 0;
}

int main(int argc, char **argv) {

	cout << "[+] main" << endl;

	aes_sample();
	return 0;
}
