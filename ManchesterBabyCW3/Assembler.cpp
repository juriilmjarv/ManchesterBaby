/*
	Assembler Program
	AC21009(CW3) - Manchester Baby - Group 16
	Andrew Brodrick
	Peter West
	Peter Short
	Juri Ilmjarv 
*/

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <bitset>

using namespace std;

string codeFile[256];
int negationValue = -1;

//instruction set for the program
//the first column is the opcode value
//The second column is if the opcode value can take in an integer using a simple Yes/No answer to populate a bool
//the third is the binary value of the opcode
string instructionSet[13][3] = {
{"VAR", "Yes", "NULL"},
{"JMP", "Yes", "0000"},
{"JRP", "Yes", "1000"},
{"LDN", "Yes", "0100"},
{"STO", "Yes", "1100"},
{"SUB", "Yes", "0010"},
{"CMP", "No", "0110"},
{"STP", "No", "1110"},
{"ADD", "Yes", "0001"},
{"MUL", "Yes", "0101"},
{"DIV", "Yes", "0011"},
{"MVT", "Yes", "1111"},
{"MVF", "Yes", "0111"},
};

//only uses the code file for now(will be able to take in extra programs
int loadCodeFile(string file)
{
	//open a file input stream using the file name passed into the function
	ifstream inputFile(file);

	//if the program cannot find the file then
	if(!inputFile)
	{
		//display an error to the user
		cout << "ERROR: The program cannot open the file.\n";
		return 1;
		
	}
	
	string line;
	int iterate = 0;
	int ignoreValue = 0;

	//while loop searches through each line of code in the file to make sure it is not null
	while(getline(inputFile, line))
	{
		//ignore value variable is set to the value of not null
		ignoreValue = line.find_first_not_of("\n \t");
		
		//if the character at the start of the line is a ; then it can be ignored as it is a comment
		if(line.at(ignoreValue) != (';'))
		{
			//checks the line to see if there is a space in it before adding to the code file
			if(iterate < 256)
			{
				//the line is then stored in the code file and the negationValue value is increased
				codeFile[iterate] = line;
				iterate++;
				cout << "The current line has been added to the text file\t" << line << endl;
				negationValue++;
			}
			else
			{
				//if the memory store is full then an error message is displayed
				cout << "ERROR: You are trying to add to a full memory store\n";
				return 1;
			}
		}
	}

	//closes the file stream
	inputFile.close();

	return 0;
}

int convertCode() 
{
	//for the number of times
  	for (int i=0; i<256; i++) 
	{
		int lengthOfLine = codeFile[i].size();
    	string currentLine = codeFile[i];
    	int endOfLine = lengthOfLine; 
    	
		//if the current line is null then
    	if (currentLine.at(0) == '#') 
		{
			//code has been finished converting
			return 1;
		}

		//for each line length
    	for(int j=0;j<lengthOfLine; j++) 
		{
			//create a new char with the value of the current line at each charcter
    	  	char currentCharacter = currentLine.at(j);
			
			//if the character is a ; then 
   			if (currentCharacter == ';') 
			{
				//populate the endOfLine variable with the full line
				endOfLine = j;
     		}
    	}
		
		//removes the comment from the file leaving the actual code itself
    	codeFile[i] = currentLine.substr(0,endOfLine);
	}	
	
	return 0;	
}

//Converts a decimal value to binary and saves as a string
string convertDecToBinary(int decValue, string typeOfString)
{
	string binValue;
	string temporaryValue;

	//if the type of string is an operand then 
	if (typeOfString == "operand")
	{
		//using an 8 bit bitset convert the decimal value passed into the function to a string 
		//set the binary value to eight 0's
		temporaryValue = bitset<8>(decValue).to_string();
		binValue = "00000000";
	} 
	else 
	{
		//using a 32 bit bitset convert the decimal value passed into the function to a string 
		//set the binary value to thirty two 0's
		temporaryValue = bitset<32>(decValue).to_string();
		binValue = "00000000000000000000000000000000";
	}

	//for every unsigned values in the loop
	for(unsigned i=0; i<temporaryValue.length(); i++)
	{
		//if the temporary value is equal to 0 then
		if (temporaryValue[temporaryValue.length() - 1 - i] == '0')
		{
			//set the binary value to 0
			binValue[i] = '0';
		}
		else
		{
			//set the binary value to 1
			binValue[i] = '1';
		}
	}

	//return the full binary value
	return binValue;
}

//takes in the variable number passed from the scan function and compiles it into binary
string compileCode(int variableNumber) 
{
	string variableData[variableNumber][4];
	variableNumber = 0;

	//for every value in the code file
	for(int i=negationValue; i>=0; i--) 
	{
		//create a new current character string with the value of the for statement
		//create new int values for position found and start of line
		//create a new size_t variable for the founfd at value
		string currentCharacter = codeFile[i];
		int positionFound = 0;
		int startOfLine = 0;
		size_t foundAt;

		//populate the found at variable with a search for the : character
		foundAt = currentCharacter.find(':');
		
		//if the size_t variable is not the greatest possible value in the size of the code then
		if (foundAt != string::npos) 
		{
			//set the value of position found variable to the first : character + 1 in the code file
			positionFound = (currentCharacter.find_first_of(':')+1);
		}
	
		//set the start of line variable to the opposite value of position found
		//create a placeholder variable for the first three characters in a line in the code file
		startOfLine = currentCharacter.find_first_not_of(" \t",positionFound);
		string placeholder = currentCharacter.substr(startOfLine,3);

		//if the size_t variable is not the greatest possible value in the size of the code
		//and the placeho;der varibale is equal to VAR then
		if ((foundAt != string::npos) && (placeholder == "VAR")) 
		{
			//set the position found variable to the first : character in the code file
			//set the varibale data value of the position found
			positionFound = (currentCharacter.find_first_of(':'));			
			variableData[variableNumber][0] = currentCharacter.substr(0,positionFound);

			//set the position found variable to the placeholder value
			//set position found to position found + 3
			positionFound = currentCharacter.find(placeholder);
			positionFound = (positionFound + 3);

			//set the found at VAR variable to the value of the opostite of the postion found
			//cerate a new string for the decimal value and populate it with the value of the any decimals in the code file
			foundAt = currentCharacter.find_first_not_of("-1234567890 \t",positionFound);
			string decValue = currentCharacter.substr(positionFound,11);

			//create a new int for the value and popluate it with the decimal value and change it to int value
			int value = atoi(decValue.c_str());
			
			//for both decinal values, convert to binary using the convertDecToBinary function
			//for the third cgange the value of the for statement to a string
			variableData[variableNumber][1] = convertDecToBinary(value,"variable");
			variableData[variableNumber][2] = convertDecToBinary(i,"operand");				
			variableData[variableNumber][3] = to_string(i);
			variableNumber++;
		}
		
		//set all lines in the code file to binary
		codeFile[i] = "00000000000000000000000000000000";

		//if the placeholder is equal to VAR then
		if (placeholder == "VAR") 
		{
			//for every variable numbers in the code file
			for (int j=0; j<variableNumber;j++) 
			{
				//create a new int for the value and change the variable data to a string
				int value = atoi(variableData[j][3].c_str());
				
				//if the value of the if statement is equal to the value of the variable data then
				if (i == value) 
				{
					//set each line of code to the value of the first variable data
					codeFile[i] = variableData[j][1];
				}
			}
		} 
		else 
		{
			//for every line in the code file
			for (int k=0; k<13; k++) 
			{
				//if the placeholder is equal to any of the instructions in the set then
				if (placeholder == instructionSet[k][0]) 
				{
					//replace each line in the code file with the binary value of the instruction set
					codeFile[i].replace(13,4,instructionSet[k][2]);
		
					//if the second column of the instruction set is equal to Yes then 
					if (instructionSet[k][1] == "Yes") 
					{
						//create a new string to compare values
						string compareValues = "";

						//populate the position found variable with a search for placeholder
						//set position found to position found + 4
						//set position found to the opposite of position found
						//populate the compare values variable with each value found
						//set position found to the first tab
						//set compare values to the length of the line
						positionFound = currentCharacter.find(placeholder);
						positionFound = (positionFound + 4);
						positionFound = currentCharacter.find_first_not_of(" \t",positionFound);
						compareValues = (currentCharacter.substr(positionFound,32));
						positionFound = compareValues.find_first_of(" \t");
						compareValues = compareValues.substr(0,positionFound);

						//for every variable number 
						for (int l=0; l<variableNumber;l++) 
						{
							//if the value of compare values is equal to the variable data value then
							if (compareValues==variableData[l][0]) 
							{
								//replace the line with the third variable data 
								codeFile[i].replace(0,8,variableData[l][2]);
							}
						}					
					}
				}
			}
		}
	}
	
	//return success message
	return "The file has been compiled successfully, the output will now be saved to an external file....";
}


//scans the code for any errors
string scanCode() 
{
	string errorString = "";
	int variableNumber = 0;
		
	//for each value in the code
	for(int i=negationValue; i>=0; i--) 
	{
		//create new ints for a position found and the start of the line with the value of 0
		//create a new size_t (the unsigned int value of the size of function) witht he value of : in the code
		int positionFound = 0;
		size_t foundAt = codeFile[i].find(':');
		int startOfLine = 0;
		
		//create a new blank string for the variable name
		string variableName = "";

		//the size_t variable is not the greatest possible value in the size of the code
		if (foundAt != string::npos) 
		{
			//populate the position found variable with the value of the first value after the : in the code
			positionFound = (codeFile[i].find_first_of(':')+1);
		}

		//create a new bool for if the value is correct
		bool isCorrect = false;

		//populate the start of line int with the value of the first non null line
		//create a placeholder variable for the first three characters in a line in the code file
		startOfLine = codeFile[i].find_first_not_of(" \t",positionFound);
		string placeholder = codeFile[i].substr(startOfLine,3);

		//for each line of code in the code file
		for (int j=0; j<13 ; j++) 
		{
			//if the placeholder is equal to the value of any opcode values in the instruction set
			if (placeholder == instructionSet[j][0]) 
			{
				//set is correct to true
				isCorrect = true;
			}
		}

		//if is correct is false then
		if (isCorrect == false) 
		{
			//display an error message to the user stating which line and what the problem is.
			errorString = "ERROR: Line ";
			errorString += to_string(i);
			errorString += ". There is a problem with opcode: ";
			errorString += placeholder;
			return errorString;
		}
		
		//if the placeholder variable is equal to VAR in the instruction set(NULL value) then
		if (placeholder == "VAR") 
		{
			//create another size_t variable for the var value with the same population as last time
			size_t foundAtVar = codeFile[i].find(':');

			//if the size_t variable is not the greatest possible value in the size of the code then
			if (foundAtVar != string::npos) 
			{
				//populate the position found variable with the value of the first value after the : in the code
				positionFound = (codeFile[i].find_first_of(':'));
			}
			
			//set the varibale name to the position found value
			//increase the variable number value
			variableName = codeFile[i].substr(0,positionFound);
			variableNumber++;
		}

		//display the code file to the user
		cout << codeFile[i] << endl;

		//for every line of code in the code file
		for (int j=0; j<13; j++) 
		{
			//if the placeholder variable is equal to anything in the first column in the instruction set then
			if (placeholder == instructionSet[j][0]) 
			{
				//if the second column in the instruction set is equal to Yes then
				if (instructionSet[j][1] == "Yes")
				{
					//set the position found value to the placeholder
					//set the position found variable to the new postion found + 3
					positionFound = codeFile[i].find(placeholder);
					positionFound = (positionFound + 3);

					//if the placeholder is equal to VAR then
					if (placeholder == "VAR") 
					{
						//set the found at VAR variable to the value of the opostite of the postion found
						foundAt = codeFile[i].find_first_not_of("-1234567890 \t",positionFound);
						
						//if the size_t variable is not the greatest possible value in the size of the code then
						if (foundAt != string::npos) 
						{
							//display an error message to the user stating which line and what the problem is.
							errorString = "ERROR: Line " + std::to_string(i) + ". There is a problem with the variable number; " + codeFile[i].substr(positionFound,32);
							return errorString;
						}
					}
				}			
			}
		}
	}
	
	//display a success message to the user and state what is happening next
	//populate the error string wiht the value of the output of the compilier function
	//return the error string
	cout << "The scan has been completed successfully, the code will now compile...." << endl;
	errorString = compileCode(variableNumber);
	return errorString;

}

void writeToExternalFile()
{
	//create a new string and populate it with a output file name 
	string newCodeFile = "AssemblerOutput.txt";

	//open a file output stream and pass in the string created
	ofstream outputFile(newCodeFile);
	
	//for everything in the code file
	for(int i=0; i<256; i++)
	{
		//if any line in the file is null
		if (codeFile[i] == "#") 
		{
			//stop as it is most likely the end of file
			break;
		}
		
		//display the final code file
		cout<<codeFile[i]<<endl;
		//insert the code file into the newly created output file
		outputFile << codeFile[i] << "\n";	
	}

	//closes the file stream
	outputFile.close();
}

int main() 
{
	//for every line in the code
  	for (int i=0; i<256; i++) 
	{
		//insert a # if the line is blank
  		codeFile[i] = "#";
  	}

	//create a new strng for any error messages
	string error = "";
	string file;

	cout << "Please enter a file name.\n";
	cin >> file;
	
	//call and run the loadCodeFile function
	loadCodeFile(file);

	//once this has been completed then display a message to the user stating what is happening next
	cout << "\nThe file has finished loading, conversion will now take place...\n\n";

	//call and run the convertCode function
	convertCode();

	//once this has been completed then display a message to the user stating what is happening next
	cout << "\nThe conversion has been completed, the file will now be scanned for errors...\n";

	//if any errors occur then this will be displayed to the user
	error = scanCode();
    
    	cout << error << endl;
	
	//call and run the writeToExternalFile function
	writeToExternalFile();
	
	//display a success message stating the new file name to the user and that the manchester baby simulator will now be run.
	cout << "\nThe output has been written to an external file called AssemblerOutput.txt, The baby simulator will now be opened...\n";
}
