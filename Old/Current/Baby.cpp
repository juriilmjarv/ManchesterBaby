//#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>

#include "baby.h"

using namespace std;

Accumulator accum;
Store st;
Operation op;
Control ct;

string fileName = "BabyTest1 - MC.txt"; //Important //Implemented by PW 17/11
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


//binary to decimal string

int Operation::binToDecimal(string binary)
{
	int value = atoi(binary.c_str()); // converst string into integer.

	return Operation::binToDecimal(value);
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
		decimalNumber += remainder*pow(2, i);

		++i;
	}

	return decimalNumber;
}


//decimal string to binary
string Operation::decToBin(string dec)
{
	int value = atoi(dec.c_str()); // converst string into integer.

	return Operation::decToBin(value);
}

///////////////////////////////////////
//binary to decimal int
string Operation::decToBin(int dec)
{
	string binary;
	int mask = 1;
	if (dec < 0){
		//2^31+(-23)
		int out = pow(2, 31);
		std::cout << "2^31 : pow(2, 31)" << out << std::endl;
		binary = "1" + decToBin(pow(2, 31)+dec);
	}else{
		for (int i = 0; i < 32; i++)
		{
			if ((mask&dec) >= 1)
				binary = "1" + binary;
			else
				binary = "0" + binary;
			mask <<= 1;
		}
	}
	return binary;
}
//////////////////////////////////////////////////

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

	for (int i = 0; i<=4; i++)
	{
		operand += to_string(store[num_of_line][i]);
	}

	return op.binToDecimal(operand);
	
}

//get line from store
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
		l = (int)(line[i]);
		store[num_of_line][i] = l;//atoi((line[i]).c_str()); // ctnverst string into integer;
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
			cout << store[i][j] << " ";
		}
		cout << endl;
	}


	cout << "--------------------------------------------------------------" << endl;
	cout << "Accumulator: " << accumulator << endl;
	cout << "CI: " << CI << endl;
	cout << "PI: " << PI << endl;
	cout << "--------------------------------------------------------------" << endl;
}

//read from file
void Operation::readFromFile()
{
	fstream myfile;
	int num_of_lines = 0;
	string line;
	myfile.open("BabyTest1-MC.txt");

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{

			for (int i = 0; i<line.length(); i++)
			{
				if (line[i] - '0' == -35) break; // skips the end of file character
				store[num_of_lines][i] = line[i] - '0';
			}
			num_of_lines++;

		}
	}
	else {

		if (debug) { std::cout << "Cannot open file" << std::endl; };
	}
}
/*
void Operation::JMP(int num_of_line)
{
	cout << "num_of_line: " <<num_of_line  << endl;
    cout << "Doing JMP: " << endl;
    cout << decToBin(num_of_line)<< endl;
    //cout << accum.getCI() << endl;
    
}
*/


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

/*
void Accumulator::run()
{

   int num_of_line = 0;

   while(num_of_line != 32)
   {
	int opcode = st.getOpCode(num_of_line);

	if(opcode == 0)
	{
        //op.JMP(num_of_line);
        //ct.incrementCI(num_of_line);
        accum.setCI(op.decToBin(num_of_line));
        for (int i = 0; i < CI.size(); ++i)
        {
        	cout << CI[i];
        }
        cout << endl;
		num_of_line++;
	}
	else if(opcode == 1)
	{
      cout << "num_of_line: " <<num_of_line  << endl;
      cout << "doing JRP" << endl;
      //ct.incrementCI(num_of_line);
      num_of_line++;
	}
	else if(opcode == 2)
	{
      cout << "num_of_line: " <<num_of_line  << endl;
      cout << "doing LDN" << endl;
      //ct.incrementCI(num_of_line);
      num_of_line++;
      //break;
	}
	else if(opcode == 3)
	{
      cout << "num_of_line: " <<num_of_line  << endl;
      cout << "doing STO" << endl;
      //ct.incrementCI(num_of_line);
      num_of_line++;
	}
	else if(opcode == 4 || opcode == 5)
	{
      cout << "num_of_line: " <<num_of_line  << endl;
      cout << "doing SUB" << endl;
      //ct.incrementCI(num_of_line);
      num_of_line++;
	}
	else if(opcode == 6)
	{
      cout << "num_of_line: " <<num_of_line  << endl;
      cout << "doing CMP" << endl;
      //ct.incrementCI(num_of_line);
      num_of_line++;
      //break;
	}
	else if(opcode == 7)
	{
      cout << "num_of_line: " <<num_of_line  << endl;
      cout << "doing STOP" << endl;
      //ct.incrementCI(num_of_line);
      num_of_line++;
	}

	else if(num_of_line == 32)
	{
		break;
	}

   }

}

*/

bool Accumulator::runInstructionSet(int num_of_line) { //function is opctde translated binary from chrs 13-15


										//bool exit;
	
	switch (st.getOpCode(num_of_line)) {
	case 0:
	    st.num_of_line++;
		//accum.setCI(st.takeLineFromStore(st.num_of_line));/*JMP*/
		mnemonic = "JMP";
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
		st.setLineFromStore(num_of_line, accum.getAccumulator()); /*STO*/
		mnemonic = "STO";
		break;
	case 4:
		//Is this producing ctrrect output???
		std::cout << accum.getAccumulator() << std::endl;
		accum.setAccum(op.decToBin(op.binToDecimal(accum.getAccumulator()) - op.binToDecimal(st.takeLineFromStore(st.num_of_line))));/*SUB*/
		mnemonic = "SUB";
		break;
	case 5:
		//Is this producing ctrrect output???
		//A = A - S
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


int tester() {
/*
	op.readFromFile();

	st.num_of_line = 0;
	std::cout << "Test 0 : " << accum.runInstructionSet(st.num_of_line) << std::endl;

	st.num_of_line = 1;
	std::cout << "Test 1 : " << accum.runInstructionSet(st.num_of_line) << std::endl;

	st.num_of_line = 2;
	std::cout << "Test 2 : " << accum.runInstructionSet(st.num_of_line) << std::endl;

	st.num_of_line = 3;
	std::cout << "Test 3 : " << accum.runInstructionSet(st.num_of_line) << std::endl;

	st.num_of_line = 4;
	std::cout << "Test 4 : " << accum.runInstructionSet(st.num_of_line) << std::endl;

	st.num_of_line = 5;
	std::cout << "Test 5 : " << accum.runInstructionSet(st.num_of_line) << std::endl;

	st.num_of_line = 6;
	std::cout << "Test 6 : " << accum.runInstructionSet(st.num_of_line) << std::endl;

	st.num_of_line = 7;
	std::cout << "Test 7 : " << accum.runInstructionSet(st.num_of_line) << std::endl;

	st.num_of_line = 8;
	std::cout << "Test 8 : fail? : " << accum.runInstructionSet(st.num_of_line) << std::endl;

	st.num_of_line = 128;
	std::cout << "Test 128 : fail? : " << accum.runInstructionSet(st.num_of_line) << std::endl;

	std::cout << "Print before read: " << std::endl;
	op.print();

	std::cout << "Number of lines from file " << op.readFromFile();


	std::cout << "Print after read: " << std::endl;
	op.print();
/*)
	cout << "Take line 0 from store: " << st.takeLineFromStore(5) << endl;
	cout << "get opctde: " << st.getOpctde(1) << endl;
	cout << "get operand: " << st.getOperand(1) << endl;
	cout << "translate 5th line opctde to decimal: " << std::to_string(op.binToDecimal(st.getOpCode(5))) << endl;
	cout << "get Accumulator: " << accum.getAccumulator() << endl;
	cout << "get CI: " << accum.getCI() << endl;
	cout << "get PI: " << ct.getPI() << endl;
*/
	//accum.run();
	//string newPI = "00000000000000000000000000000000";
	//cout << "set new PI: " << accum.setPI(newPI) << endl;


	return 0;
}


////

int main(int argc, char** argv) {

	if (argc == 1) {
		fileName = (string)argv[0];
	}

	if (debug) {
		tester();
	}

	//fetch
	//int loop = op.readFromFile();
	op.readFromFile();
	int loop = 32;

	op.print();
	for(int i = 1; i <= loop; i++) {
		ct.incrementPI(st.num_of_line);
		ct.incrementCI(st.num_of_line);

		//dectde
		//execute
		accum.runInstructionSet(st.num_of_line);
			//display
		if (i == loop || !mnemonic.compare("STP")) {
			std::cout << "Line " << st.num_of_line << " : Final Output: - " << std::endl;
		}else {
			std::cout << "Line " << st.num_of_line << " - " << std::endl;
		}

		std::cout << mnemonic << std::endl;
		
		op.print();
		if (!mnemonic.compare("STP")) {
			std::cout << "STP Operation detected" << std::endl;

			std::cout<<op.decToBin(-23);
			return 1;
		}

 		st.num_of_line++;
	}
	std::cout << "Instructions complete" << std::endl;

}



