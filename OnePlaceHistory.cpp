// OnePlaceHistory.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
using namespace std;
#include <iostream>
#include <fstream>
#include "math.h"
#include <stdio.h>
#include <iomanip> 

struct Table
{
	int* pred_val;
};

void UpdateHistoryTable(Table *BHistoryTable, int n, int shift_reg, bool branch);
char* StrToChar(string str);
int BinStrtoInt(string binStr);
bool Prediction(Table *BHistoryTable, int n, int shift_reg);
int ReadTraceFile(int m, int n, Table *BHistoryTable, int columns, int &file_elements);
string UpadateShiftReg(string shift_reg, bool branch_taken);

int main()
{

	int m, n;
	int columns; //number of rows, columns and bits to access BHTable
	long double result = 0.0;
	int count = 0;
	int file_elements = 0;

	n=2;
	cout<<"please give m " << endl;
	cin >> m; //read from user
	getchar();
	cout << "m = " << m <<  endl;

	columns = (int)pow(2, m);
	
	cout <<  "columns = " << columns << ", "  << endl;

	Table *BHistoryTable = new Table; //The Branch History Table
	BHistoryTable->pred_val = new int[columns];

	 //first all prediction values = 0
		for(int j=0; j<columns; j++)
		{
			BHistoryTable->pred_val[j] = 0;
		}

	count = ReadTraceFile(m, n, BHistoryTable, columns, file_elements); //read file
	
	result = (long double) ((double)(count*100)/file_elements);

	cout << " rate is " <<  std::setprecision(7) <<  result << "%" << endl;

	cout<<"Please press enter button to exit" << endl;
	getchar();
	return 0;
}

void UpdateHistoryTable(Table *BHistoryTable, int n, int shift_reg, bool branch)
{
	if(branch)
	{
		if(BHistoryTable->pred_val[shift_reg] != (int)(pow(2,n)-1))
			BHistoryTable->pred_val[shift_reg]++;
	}

	else
	{
		if(BHistoryTable->pred_val[shift_reg] != 0)
			BHistoryTable->pred_val[shift_reg]--;
	}

}

int ReadTraceFile(int m, int n, Table *BHistoryTable, int columns, int &file_elements)
{
	const char * filename = "test.txt";
    ifstream infile;
    string curr_address;
    string nxt_address;
	int count = 0;
	string shiftReg = "0";
	int N = 0;

	bool prediction_res = false;
	bool branch_taken = false;

	while(N<m-1)
	{
		shiftReg = shiftReg + "0";
		N++;
	}
	file_elements = 0;

    infile.open(filename,ios::in);
    if (!infile.is_open()) {
        cerr << "Can't open file " << filename << "!\n";
    }
	else {
       
        while (infile >> curr_address >> nxt_address) {

			file_elements++;
			prediction_res = Prediction(BHistoryTable, n, BinStrtoInt(shiftReg));

			if((strtoul( StrToChar(curr_address), NULL, 16) + 8) == strtoul(StrToChar(nxt_address), NULL, 16))  
				branch_taken = false;

			else branch_taken = true;

			if(branch_taken == prediction_res) count++;
			
			UpdateHistoryTable(BHistoryTable, n, BinStrtoInt(shiftReg), branch_taken);

			shiftReg = UpadateShiftReg( shiftReg, branch_taken);
	
		}
		
        infile.close();
    }

	return count;
}

bool Prediction(Table *BHistoryTable, int n, int shift_reg)
{
	
		int	pred_result = BHistoryTable->pred_val[shift_reg];

		if(pred_result < (int)( pow(2, n)/2)) return false;
		else return true;
	
}

string UpadateShiftReg(string shift_reg, bool branch_taken)
{
	shift_reg.erase(0,1);

	if(branch_taken)
		shift_reg = shift_reg + "1";
	else shift_reg = shift_reg + "0";

	return shift_reg;
}


char* StrToChar(string str)
{
	char *a=new char[str.size()+1];
	a[str.size()]=0;
	memcpy(a,str.c_str(),str.size());
	return a;
}

int BinStrtoInt(string binStr)
{
   int res = 0;
   for( int i=0; i<binStr.length(); i++ ) res = 2*res + binStr[i] - '0';
   
   return res;

}
