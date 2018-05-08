/*
Lizard in C++

Lizard was proposed by Mattias Hamann, Matthias Krause and Willi Meier.
The C++ implementation code is created by John Fjeld
*/
#include "stdafx.h"	//standard library in visual studio
#include <string>	//strings
using namespace std;

int main() {

	class Lizard {
	private:
		//*******************************VARIABELS********************************
		string ivBinary;				// 64 bit
		string keyBinary;				// 120 bit
		string B; const int bLength = 90;		// NFSR2[90]
		string S; const int sLength = 31;		// NFSR1[31]
		char z, NFSR1, NFSR2;				// Used for update bits

		//*******************************FUNCTIONS********************************
		void initialization(const char* IV, const char* K) {
			//Phase 1 - Key and IV Loading
			keyBinary = hex2binary(K);
			ivBinary = hex2binary(IV);

			//create NFSR2 aka B
			B.resize(bLength);	//make const
			for (int i = 0; i < 64; i++) { B[i] = (keyBinary[i] ^ ivBinary[i]) + '0'; }
			for (int i = 64; i < bLength; i++) { B[i] = keyBinary[i]; }

			//create NFSR1 aka S
			S.resize(sLength);
			for (int i = 0; i < 29; i++) { S[i] = keyBinary[i + bLength]; }	// rest of Key after B length = i + 90 
			S[29] = (keyBinary[119] ^ '1') + '0';				// keyBinary[keyBinary.length()-1)]
			S[30] = '1';

			//Phase 2 - Grain-like Mixing (same as diffusion function + XOR last bit with z)
			for (int i = 0; i < 128; i++) {
				a();															//update z
				diffusion();													//shift and update last bit
				B[B.length() - 1] = (B[B.length() - 1] ^ z) + '0';		//xor last bit with z
				S[S.length() - 1] = (S[S.length() - 1] ^ z) + '0';		//xor last bit with z
			}

			//Phase 3 - Second Key Addition
			for (int i = 0; i < B.length(); i++) {					// i {0 .. 89} -> for all B
				B[i] = (B[i] ^ keyBinary[i]) + '0';				// XOR B with Key
			}

			for (int i = 0; i < S.length() - 1; i++) {				// i {0 .. 29} -> for all S except last bit
				S[i] = (S[i] ^ keyBinary[i + B.length()]) + '0';		//XOR S with K(i+90) / rest of Key
			}
			S[S.length() - 1] = '1';						// set S[30] last bit = 1

			//Phase 4 - Final Diffusion
			for (int i = 129; i < 257; i++) {
				diffusion();
			}
		}

		void a() {
			char L, Q, T1, T2;

			L = B[7] ^ B[11] ^ B[30] ^ B[40] ^ B[45] ^ B[54] ^ B[71];

			Q = ((B[4] & B[21]) ^ (B[9] & B[52]) ^ (B[18] & B[37]) ^ (B[44] & B[76])) + '0';

			T1 = (B[5] ^ (B[8] & B[82]) ^ (B[34] & B[67] & B[73]) ^ (B[2] & B[28] & B[41] & B[65]) ^
				(B[13] & B[29] & B[50] & B[64] & B[75]) ^ (B[6] & B[14] & B[26] & B[32] & B[47] & B[61]) ^
				(B[1] & B[19] & B[27] & B[43] & B[57] & B[66] & B[78]));

			T2 = (S[23] ^ (S[3] & S[16]) ^ (S[9] & S[13] & B[48]) ^ (S[1] & S[24] & B[38] & B[63])) + '0';

			z = (L ^ Q ^ T1 ^ T2) + '0';
		}

		void diffusion() {
			f2();		// Get updated last bit B
			f1();		// Get updated last bit S

			for (int i = 0; i < B.length() - 1; i++) {		// i {0 .. 88} == strlen(B)-1 OR 89
				B[i] = B[i + 1];					//shift all bits in B
			}
			B[B.length() - 1] = NFSR2;					// update last bit in B = B[89]

			for (int i = 0; i < S.length() - 1; i++) {		// i {0 .. 29} == strlen(S)-1 OR 30
				S[i] = S[i + 1];					//shift all bits in S
			}
			S[S.length() - 1] = NFSR1;				// update last bit in S = S[30]
		}

		void f2() {		//update function for last B bit aka B[89]	
			NFSR2 = (S[0] ^ B[0] ^ B[24] ^ B[49] ^ B[79] ^ B[84] ^ (B[3] & B[59]) ^ (B[10] & B[12]) ^
				(B[15] & B[16]) ^ (B[25] & B[53]) ^ (B[35] & B[42]) ^ (B[55] & B[58]) ^ (B[60] & B[74]) ^
				(B[20] & B[22] & B[23]) ^ (B[62] & B[68] & B[72]) ^ (B[77] & B[80] & B[81] & B[83])) + '0';
		}

		void f1() {		//update function for last S bit aka S[30]
			NFSR1 = (S[0] ^ S[2] ^ S[5] ^ S[6] ^ S[15] ^ S[17] ^ S[18] ^ S[20] ^ S[25] ^
				(S[8] & S[18]) ^ (S[8] & S[20]) ^ (S[12] & S[21]) ^ (S[14] & S[19]) ^ (S[17] & S[21]) ^ (S[20] & S[22]) ^
				(S[4] & S[12] & S[22]) ^ (S[4] & S[19] & S[22]) ^ (S[7] & S[20] & S[21]) ^ (S[8] & S[18] & S[22]) ^
				(S[8] & S[20] & S[22]) ^ (S[12] & S[19] & S[22]) ^ (S[20] & S[21] & S[22]) ^ (S[4] & S[7] & S[12] & S[21]) ^
				(S[4] & S[7] & S[19] & S[21]) ^ (S[4] & S[12] & S[21] & S[22]) ^ (S[4] & S[19] & S[21] & S[22]) ^
				(S[7] & S[8] & S[18] & S[21]) ^ (S[7] & S[8] & S[20] & S[21]) ^ (S[7] & S[12] & S[19] & S[21]) ^
				(S[8] & S[18] & S[21] & S[22]) ^ (S[8] & S[20] & S[21] & S[22]) ^ (S[12] & S[19] & S[21] & S[22])) + '0';
		}

	public:
		//*******************************FUNCTIONS********************************
		string hex2binary(const char* convert) {
			string toBin;	// return variable
			int j = 0;	// temp conter keep track of possition of new char

			//foreach hex in input
			for (int i = 0; i < strlen(convert); i++) {
				switch (convert[i]) {
				case '0': toBin += "0000"; break; // 0000
				case '1': toBin += "0001"; break; // 0001
				case '2': toBin += "0010"; break; // 0010
				case '3': toBin += "0011"; break; // 0011
				case '4': toBin += "0100"; break; // 0100
				case '5': toBin += "0101"; break; // 0101
				case '6': toBin += "0110"; break; // 0110
				case '7': toBin += "0111"; break; // 0111
				case '8': toBin += "1000"; break; // 1000
				case '9': toBin += "1001"; break; // 1001
				case 'A': toBin += "1010"; break; // 1010
				case 'B': toBin += "1011"; break; // 1011
				case 'C': toBin += "1100"; break; // 1100
				case 'D': toBin += "1101"; break; // 1101
				case 'E': toBin += "1110"; break; // 1110
				case 'F': toBin += "1111"; break; // 1111
				default: printf("IV not 16 Byte or Key not 30 Byte \n");
				}
				j += 4; // move newPointer 4 up for next hex value
			}
			return toBin;
		}

		string binary2hex(const string &convert) {
			string toHex;			// return variable
			char nibble[5] = "0";		// temp variable to store 4 bits

			//for loop to group four and four bits to a temp veraible
			for (int i = 0; i < convert.length(); i += 4) {
				int j = 0, s = 0;
				for (j += i; j < i + 4; j++) {
					nibble[s] = convert[j];
					s++;
				}
				//need string to compare against
				string check = nibble;

				//check four bits to convert to hexa
				if (check == "0000") { toHex += '0'; }		// 0
				else if (check == "0001") { toHex += '1'; }	// 1
				else if (check == "0010") { toHex += '2'; }	// 2
				else if (check == "0011") { toHex += '3'; }	// 3
				else if (check == "0100") { toHex += '4'; }	// 4
				else if (check == "0101") { toHex += '5'; }	// 5
				else if (check == "0110") { toHex += '6'; }	// 6
				else if (check == "0111") { toHex += '7'; }	// 7
				else if (check == "1000") { toHex += '8'; }	// 8
				else if (check == "1001") { toHex += '9'; }	// 9
				else if (check == "1010") { toHex += 'A'; }	// A
				else if (check == "1011") { toHex += 'B'; }	// B
				else if (check == "1100") { toHex += 'C'; }	// C
				else if (check == "1101") { toHex += 'D'; }	// D
				else if (check == "1110") { toHex += 'E'; }	// E
				else if (check == "1111") { toHex += 'F'; }	// F
				else { printf("something went wrong with converting Bin2Hex \n"); }
			}
			return toHex;
		}

		void getKeystream(const char* IV, const char* K, int keystreamLength) {
			string keystreamBinary;
			string keystreamHex;

			if (strlen(IV) == 16 && strlen(K) == 30) {	//check input length of IV & Key
				initialization(IV, K);

				for (int i = 0; i < keystreamLength; i++) {  // loop of keystreamLength
					a();					// get z[0] 
					keystreamBinary += z;			// get one and one keystream bit
					diffusion();				// shift and update last bit
				}

				printf("Key: \t\t0x %s \nIV: \t\t0x %s \n", K, IV);		// prints K and IV
				keystreamHex = binary2hex(keystreamBinary);			// convert keystream to Hex
				printf("Keystream: \t0x %s \n\n", keystreamHex.c_str());	// prints keystream. (c_str -> turns it to char array)
			}
			else printf("IV is not 16 Byte or Key is not 30 Byte \n");
		}
	};

	Lizard object;
	object.getKeystream("0000000000000000", "000000000000000000000000000000", 128);
	object.getKeystream("FFFFFFFFFFFFFFFF", "0000000000000000FFFFFFFFFFFFFF", 128);
	object.getKeystream("ABCDEF0123456789", "0123456789ABCDEF0123456789ABCD", 128);

	return 0;
}
