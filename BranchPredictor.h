
#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;
#include <fstream>
#include <bitset>
#include <vector>
#include <map>

struct Table
{
	string address;
	int* pred_val;
};


string IntToBinStr(int shiftReg)
{

	if ( shiftReg == 0 ) return "0";
    if ( shiftReg == 1 ) return "1";

    if (shiftReg % 2 == 0 )
        return IntToBinStr(shiftReg / 2) + "0";
    else
        return IntToBinStr(shiftReg / 2) + "1";
	
}

int BinStrtoInt(string binStr)
{
   int res = 0;
   for( int i=0; i<binStr.length(); i++ ) res = 2*res + binStr[i] - '0';
   
   return res;

}

string HexToBinary (string hexStr)
{
	string binStr = "";

	for (int i = 0; i < hexStr.length (); ++i)
	{
		switch ( hexStr[i] )
		{
			case '0': binStr.append ("0000"); break;
			case '1': binStr.append ("0001"); break;
			case '2': binStr.append ("0010"); break;
			case '3': binStr.append ("0011"); break;
			case '4': binStr.append ("0100"); break;
			case '5': binStr.append ("0101"); break;
			case '6': binStr.append ("0110"); break;
			case '7': binStr.append ("0111"); break;
			case '8': binStr.append ("1000"); break;
			case '9': binStr.append ("1001"); break;
			case 'a': binStr.append ("1010"); break;
			case 'b': binStr.append ("1011"); break;
			case 'c': binStr.append ("1100"); break;
			case 'd': binStr.append ("1101"); break;
			case 'e': binStr.append ("1110"); break;
			case 'f': binStr.append ("1111"); break;
		}
	}

	return binStr;
}





