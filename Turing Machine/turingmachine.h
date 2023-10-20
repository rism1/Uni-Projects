#pragma once

#include <string>

struct State
{
	int iQ, iS, oQ, oS;
	char dir;

	State() : iQ(-1), iS(-1), oQ(-1), oS(-1), dir('/') {};
	State(int inputQ, int inputS, int outputQ, int outputS, char direction) : iQ(inputQ), iS(inputS), oQ(outputQ), oS(outputS), dir(direction) {};
};

class TuringMachine
{
public:

	TuringMachine(int length, std::string& program);
	TuringMachine(const TuringMachine& TM);
	~TuringMachine();

	TuringMachine& operator=(const TuringMachine& TM);
	TuringMachine& operator=(TuringMachine&& TM);

	std::string calc(std::string const& str, int maxSteps);

	int strToInt(std::string str, int first, int last);
	int m_ProgramLength;
	State* m_Program;
private:
	int m_TapeLength;
};