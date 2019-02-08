/*
Simulator Program
AC21009(CW3) - Manchester Baby - Group 16
Andrew Brodrick
Peter West
Peter Short
Juri Ilmjarv
*/

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>

#include "baby.h"

using namespace std;

Accumulator accum;
Store st;
Operation op;
Control ct;

string fileName = "AssemblerOutput.txt"; //Could also be BabyTest1-MC.txt for testing
string mnemonic = "";

//Constructor
Operation::Operation()
{
	for (int i = 0; i<32; i++)
	{
		for (int j = 0; j<32; j++)
		{
			store[i][j] = 0;    //set 32x32 store to 0
		}
	}

	accumulator = "00000000000000000000000000000000";
	CI = "00000000000000000000000000000000";
	PI = "00000000000000000000000000000000";
}

//Operation Function to carry out normal maths of base^exponent, not the same in cpp as : base^exponent 
int Operation::int_pow(int base, int exp)
{
	int result = 1;
	while (exp)
	{
		if (exp & 1)
			result *= base;
		exp /= 2;
		base *= base;
	}
	return result;
}

//binary to decimal string

int Operation::binToDecimal(string binary)
{
	//Will only do 2s complament if binary is above 4
	if ((binary[0] == 1) && (binary.size() > 4)) {
		string tmp = binary;
		tmp.erase(0, 1);


		//if binary is 1111
		//The number starts with 1, so it's negative, so we find the complement of 1111, which is 0000. invert 0's and 1's
		//Other example binary is 1100111000
		int tmpSize = tmp.size();
		for (int i = 0; i < tmpSize; i++) {
			if (tmp[i] == '1') {
				tmp[i] = '0';
			}
			else {
				tmp[i] = '1';
			}
		}
		//Add 1 to 0000, and we obtain 0001.

		//(To add 1 to say 0011000111, flip all the bits from the rightmost 0 bit 
		//(we get 0011001000)

		//

		int location = -1;
		//starts loop at final item in string
		for (int i = tmp.size() - 1; i > -1; i--) {
			if (tmp[i] == '0') {
				location = i;
				break;
			}
		}
		if (location == -1) {
			//no rightmost 0 found
			//all ones
			// therefore binary to int is -1
			return -1;

		}
		else {
			//flip
			// 0011000111 we need to flip all the bits after the rightmost 0 bit (we get 0011000000). 
			//Finally, flip the rightmost 0 bit also (we get 0011001000) and then have added 1 
			int tmpSize = tmp.size();
			for (int o = location; o < tmpSize; o++) {
				if (tmp[o] == '0') {
					tmp[o] = '1';
				}
				else {
					tmp[0] = '0';
				}

			}
		}
		binary = tmp;
	}

	int value = 0;
	int bin = 0;
	for (int i = binary.length() - 1; i > -1; i--) {
		if (binary[i] == '0') {
			bin = 0;
		}
		else {
			bin = 1;
		}
		value = value + (bin*Operation::int_pow(2, i));
	}
		return value;

}

//binary to decimal int
int Operation::binToDecimal(int binary)
{
	int value = binary;

	int decimalNumber = 0, i = 0, remainder;

	while (value != 0)
	{
		remainder = value % 10;
		value /= 10;
		decimalNumber += remainder*(int)pow(2, i);

		++i;
	}
	return decimalNumber;
}

//decimal string to binary
string Operation::decToBin(string dec)
{
	int value = atoi(dec.c_str()); // converts string into integer.

	return Operation::decToBin(value);
}

//binary to decimal int
string Operation::decToBin(int dec)
{
	string binary;
	if (dec < 0) {
		binary = decToBin((op.int_pow(2, 31) + dec));

	}
	else {
		int binNumb = 0;
		int remainder, i = 1, n = dec;
		while (n != 0)
		{
			remainder = n % 2;
			n /= 2;
			binNumb += remainder*i;
			i *= 10;
		}
		binary = std::bitset< 31 >(dec).to_string(); // string conversion
	}
	return binary;
}

string Accumulator::getAccumulator()
{
	return accumulator;
}

string Accumulator::getCI()
{
	return CI;
}

string Accumulator::getPI()
{
	return PI;
}

//mutator
void Accumulator::setAccum(string newAccum)
{
	accumulator = newAccum;
}

//mutator
void Accumulator::setPI(string newPI)
{
	PI = newPI;
}

//mutator
void Accumulator::setCI(string newCI)
{
	CI = newCI;
}

// Get opcode from line.
int Store::getOpCode(int num_of_line)
{
	string opcode;

	for (int i = 13; i<16; i++)
	{
		opcode += to_string(store[num_of_line][i]);
	}

	return op.binToDecimal(opcode);
}

//get operand from line
int Store::getOperand(int num_of_line)
{
	string operand;

	for (int i = 0; i <= 4; i++)
	{
		operand += to_string(store[num_of_line][i]);
	}

	return op.binToDecimal(operand);

}

//get lBabyTest1-Assembler.txtine from store
string Store::takeLineFromStore(int num_of_line)
{
	string instruction;

	for (int i = 0; i<32; i++)
	{
		instruction += to_string(store[num_of_line][i]);
	}
	return instruction;
}

//set line from store
void Store::setLineFromStore(int num_of_line, string line)
{
	//string instruction;
	int l = 0;
	for (int i = 0; i<32; i++)
	{
		if (line[i] == '0') {
			l = 0;
		}
		else {
			l = 1;
		}
		store[num_of_line][i] = l;
	}
}

//print the store, accumulatur, CI and PI
void Operation::print()
{
	cout << "=========================STORE=================================";
	cout << "\n";
	cout << endl;

	for (int i = 0; i<32; i++)
	{

		for (int j = 0; j<32; j++)
		{ 
			if (store[i][j] == 1) {
				cout << "\u25A0 ";
			}else{
				cout << "\u25A1 ";
			}
			cout << " ";
		}
		cout << endl;
	}


	cout << "--------------------------------------------------------------" << endl;
	cout << "Accumulator: " << accumulator << endl;
	cout << "CI: " << CI << endl;
	//<< " : "<< binToDecimal(accum.getCI()) << endl;
	cout << "PI: " << PI << endl;
	cout << "--------------------------------------------------------------" << endl;
}

//read from file
int Operation::readFromFile()
{
	fstream myfile;
	int num_of_lines = 0;
	string line;
	//fileName = "BabyTest1-MC.txt" by default, unless change of name passed into program as parameter
	myfile.open(fileName);

	if (myfile.is_open())
	{
		int lineLength = 0;
		while (getline(myfile, line))
		{
			lineLength = line.length();
			for (int i = 0; i<lineLength; i++)
			{
				if (line[i] - '0' == -35) break; // skips the end of file character
				store[num_of_lines][i] = line[i] - '0';
				if ((store[num_of_lines][i] != 1) && (store[num_of_lines][i] != 0)) {
					return 0;
				}
			}
			num_of_lines++;

		}
	}
	else {
		std::cout << "Cannot open file" << std::endl;
	}
	return num_of_lines;
}


string Control::incrementCI(int num_of_line)
{
	accum.setCI(op.decToBin(num_of_line));
	return CI;
}

string Control::incrementPI(int num_of_line)
{
	accum.setPI(st.takeLineFromStore(num_of_line));
	return PI;
}

bool Accumulator::runInstructionSet(int num_of_line) { //function is opctde translated binary from chrs 13-15  //bool exit;

	switch (st.getOpCode(num_of_line)) {
	case 0:
		mnemonic = "JMP";
		st.num_of_line = st.getOperand(num_of_line);
		break;
	case 1:
		accum.setCI(op.decToBin(op.binToDecimal(accum.getCI()) + op.binToDecimal((st.takeLineFromStore(st.getOperand(st.num_of_line)))))); /*JRP*/
		mnemonic = "JRP";
		break;
	case 2:
		accum.setAccum(op.decToBin(-op.binToDecimal(st.takeLineFromStore(st.num_of_line)))); /*LDN*/
		mnemonic = "LDN";
		break;
	case 3:
		//int Store::getOperand(int num_of_line)
		st.setLineFromStore(st.getOperand(num_of_line), accum.getAccumulator()); /*STO*/
		mnemonic = "STO";
		break;
	case 4:
		std::cout << accum.getAccumulator() << std::endl;
		accum.setAccum(op.decToBin(op.binToDecimal(accum.getAccumulator()) - op.binToDecimal(st.takeLineFromStore(st.num_of_line))));/*SUB*/
		mnemonic = "SUB";
		break;
	case 5:
		accum.setAccum(op.decToBin(op.binToDecimal(accum.getAccumulator()) - op.binToDecimal(st.takeLineFromStore(st.num_of_line))));/*SUB*/
		mnemonic = "SUB";
		break;
	case 6:
		if (op.binToDecimal(accum.getAccumulator()) < 0) { ct.incrementCI(st.num_of_line); }/*CMP*/
		mnemonic = "CMP";
		break;
	case 7:
		mnemonic = "STP";
		break;
	default:
		mnemonic = "Error";
		return false;
	}
	return true;
}

int main(int argc, char** argv) {

	if (argc == 2) {
		fileName = (string)argv[1];
		std::cout << "Loading Assembler Output File : " << fileName << std::endl;
		std::cout << "" << std::endl;
	}

	//fetch
	//int loop = 32 by default :: number of lines
	int loop = op.readFromFile();
	if (loop == 0) {
		std::cout << "Assembler Output File " << fileName << " compiled incorrectly. Please make file again and make sure it is in binary format." << std::endl;
	}
	else {
		if (loop < 0) {
			loop = 0;
		}
		else if (loop > 32) {
			//store can only store 32x32
			loop = 32;
		}

		op.print();
		for (int i = 1; i <= loop; i++) {
			ct.incrementPI(st.num_of_line);
			ct.incrementCI(st.num_of_line);

			//decode
			//execute
			accum.runInstructionSet(st.num_of_line);
			//display
			if (i == loop || !mnemonic.compare("STP")) {
				std::cout << "Line " << st.num_of_line << " : Final Output: - " << std::endl;
			}
			else {
				std::cout << "Line " << st.num_of_line << " - " << std::endl;
			}

			std::cout << mnemonic << std::endl;

			op.print();
			if (!mnemonic.compare("STP")) {
				std::cout << "STP Operation detected" << std::endl;
				std::cout << "" << std::endl;
				std::cout << ":Press Enter Key to Finish:" << std::endl;
				string pause;
				getline(cin, pause);
				return 0;
			}

			st.num_of_line++;
		}


		std::cout << "Instructions complete" << std::endl;
		std::cout << "" << std::endl;
	}
	std::cout << ":Press Enter Key to Finish:" << std::endl;
	string pause;
	getline(cin, pause);
}
