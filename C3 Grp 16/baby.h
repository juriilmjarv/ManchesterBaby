//#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

#ifndef BABY_H_
#define BABY_H_

int store[32][32];

string accumulator;
string CI;
string PI;

class Accumulator {

	public:

		bool runInstructionSet(int num_of_line); //function is decimal from binary chrs 13-15
		string getAccumulator();
		string getCI();
		string getPI();
		void setAccum(string newAccum);
		void setCI(string newCI);
		void setPI(string newPI);

		void run();

};

class Control {

	public:
	
		string incrementCI(int num_of_line);
		string incrementPI(int num_of_line);
};

class Store {

	public:
		int num_of_line = 0;
		string takeLineFromStore(int num_of_line);
		void setLineFromStore(int num_of_line, string line);

		int getOpCode(int num_of_line);
		int getOperand(int num_of_line);
};

class Operation {

	public:
		void print();
		int readFromFile();
		int binToDecimal(string binary);
		int binToDecimal(int binary);
		string decToBin(string dec);
		string decToBin(int dec);
		int int_pow(int base, int exp);

		Operation(); 
};
#endif