// BranchPredictor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "math.h"
#include <stdio.h>
#include "BranchPredictor.h"
using namespace std;
#include <string.h>
#include <iomanip> 

string UpadateShiftReg(string shift_reg, bool branch_taken);
int ReadTraceFile(int m, int n, Table *BHistoryTable, int rows, int columns, int &file_elements, int access_bits);
double Log2( double n );
bool Prediction(Table *BHistoryTable, string curr_address, int n, int index, int columns, int shift_reg);
void UpdateHistoryTable(Table *BHistoryTable, int index, int n, int shift_reg, bool pred_res);
char* StrToChar(string str);

int main()
{
	int  n;
	double BHTable_size;
	int m;
	char newline;
	int rows, columns, access_bits; //number of rows, columns and bits to access BHTable
	long double result = 0.0;
	int count = 0;
	int file_elements = 0;

	cout<<"please give m, n and Branch History Table size" << endl;
	cin >> m >> n >> BHTable_size ; //read from user
	getchar();
	cout << "m = " << m << ", " << "n = " << n << ", " << "Branch History Table size = " << BHTable_size << "Kbits" << endl;

	rows = ((BHTable_size)*1024)/(pow(2, m)*n); //compute rows, columns and access_bits
	columns = (int)pow(2, m);
	access_bits = Log2 ((double)rows) - m;
	cout << "Please wait..." << endl;

	Table *BHistoryTable = new Table[rows]; //The Branch History Table

	for(int i=0; i<rows; i++)
	{
		BHistoryTable[i].address = new char[7];
		BHistoryTable[i].pred_val = new int[columns];

	   //first all prediction values = 0
		for(int j=0; j<columns; j++)
		{
			BHistoryTable[i].address = "";
			BHistoryTable[i].pred_val[j] = 0;
		}
	}

	count = ReadTraceFile(m, n, BHistoryTable, rows, columns, file_elements, access_bits); //read file
	
	result = (long double) ((double)(count*100)/file_elements);

	cout << " rate is " <<  std::setprecision(7) <<  result << "%" << endl;
	cout<<"Please press enter button to exit" << endl;
	getchar();
	return 0;
}


string UpadateShiftReg(string shift_reg, bool branch_taken)
{
	shift_reg.erase(0,1);

	if(branch_taken)
		shift_reg = shift_reg + "1";
	else shift_reg = shift_reg + "0";

	return shift_reg;
}

int ReadTraceFile(int m, int n, Table *BHistoryTable, int rows, int columns, int &file_elements, int access_bits)
{
	const char * filename = "trace.txt";
    ifstream infile;
    string curr_address;
    string nxt_address;
	int count = 0;
	string shiftReg = "0";
	
	bool prediction_res = false;
	bool branch_taken = false;

	if(m==2) shiftReg = "00";

	file_elements = 0;

    infile.open(filename,ios::in);
    if (!infile.is_open()) {
        cerr << "Can't open file " << filename << "!\n";
    }
	else {
       
	//	cout << "curr_address  jmp_address" << endl;
        while (infile >> curr_address >> nxt_address) {
    //        cout << curr_address << "\t\t" << nxt_address << endl;
			file_elements++;
			//if(file_elements==3960) break;
        
			string binaryAddr = HexToBinary(curr_address);
			std::reverse( binaryAddr.begin(),  binaryAddr.end());
			//if(m>0){
			while(binaryAddr.size() > access_bits)
				binaryAddr.pop_back();
			//}
			if(m>0)
				binaryAddr = binaryAddr + shiftReg;
		
			int TableIndex = (int) strtol(StrToChar(binaryAddr), NULL, 2);
		
			prediction_res = Prediction(BHistoryTable, curr_address, n, TableIndex, columns, BinStrtoInt(shiftReg));

			if((strtoul( StrToChar(curr_address), NULL, 16) + 8) == strtoul(StrToChar(nxt_address), NULL, 16))  
				branch_taken = false;

			else branch_taken = true;

			if(branch_taken == prediction_res) count++;
			
			UpdateHistoryTable(BHistoryTable, TableIndex, n, BinStrtoInt(shiftReg), branch_taken);

			shiftReg = UpadateShiftReg( shiftReg, branch_taken);
	
		}
		
        infile.close();
    }

	return count;
}

bool Prediction(Table *BHistoryTable, string curr_address, int n, int index, int columns, int shift_reg)
{
	if(BHistoryTable[index].address == curr_address)
	{
		int	pred_result = BHistoryTable[index].pred_val[shift_reg];
		if(pred_result < (int)( pow(2, n)/2)) return false;
		else return true;
	}

	else
	{
		BHistoryTable[index].address = curr_address;
		
		for(int i=0;i<columns;i++) 
			BHistoryTable[index].pred_val[i] = 0;
	
		return false;
	}
}


void UpdateHistoryTable(Table *BHistoryTable, int index, int n, int shift_reg, bool branch)
{
	if(branch)
	{
		if(BHistoryTable[index].pred_val[shift_reg] != (int)(pow(2,n)-1))
			BHistoryTable[index].pred_val[shift_reg]++;
	}

	else
	{
		if(BHistoryTable[index].pred_val[shift_reg] != 0)
			BHistoryTable[index].pred_val[shift_reg]--;
	}

}

char* StrToChar(string str)
{
	char *a=new char[str.size()+1];
	a[str.size()]=0;
	memcpy(a,str.c_str(),str.size());
	return a;
}

double Log2( double n )  
{  
    // log(n)/log(2) is log2.  
    return log( n ) / log( 2 );  
}
