#include <fstream>
#include <iostream>
#include "turingmachine.h"

int main(int argc, char* argv[]){
	std::string mtName, inputName, outputName;
	if (argc == 4)
	{
		mtName = argv[1];
		inputName = argv[2];
		outputName = argv[3];
	}
	else
	{
		mtName = "MT.txt";
		inputName = "input.txt";
		outputName = "output.txt";
	}

	std::ifstream mtFile(mtName);
	std::ifstream inputFile(inputName);
	std::ofstream outputFile(outputName);

	if (!mtFile.is_open() || !inputFile.is_open() || !outputFile.is_open())
	{
		std::cout << "Cannot open files!\n";
		return -1;
	}

	std::string fileStr("");
	for (std::string line; std::getline(mtFile, line); )
	{
		fileStr += line;
		fileStr += '\n';
	}
	
	int length = 0, maxSteps = 0;
	std::string tape;
	inputFile >> length >> maxSteps >> tape;
		
	TuringMachine TM(length, fileStr);
	outputFile << TM.calc(tape, maxSteps);
}
