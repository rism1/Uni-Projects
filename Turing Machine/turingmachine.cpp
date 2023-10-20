#include <iostream>
#include "turingmachine.h"
TuringMachine::TuringMachine(int length, std::string& program)
{
	m_TapeLength = length;
	m_ProgramLength = 0;

	for (int i = 0; i < program.length(); ++i)
	{
		if (program[i] == '\n')
		{
			++m_ProgramLength;
		}
	}
	m_Program = new State[m_ProgramLength];

	// q11 0 -> q10 0 L
	int count = 0;
	int program_count = -1;
	bool outside = true;
	int start = -1;
	State state = State();
	for (int i = 0; i < program.length(); ++i)
	{
		if (program[i] != ' ' && program[i] != '\n')
		{
			if (outside)
			{
				outside = false;
				++count;
			}

			switch (count)
			{
			case 1:
				if (program[i] >= '0' && program[i] <= '9')
				{
					if (start == -1)
						start = i;
					if (program[i+1] < '0' || program[i+1] > '9')
					{
						state.iQ = strToInt(program, start, i);
						start = -1;
					}
				}
				break;

			case 2:
				if (program[i] == '0')
					state.iS = false;
				if (program[i] == '1')
					state.iS = true;
				break;

			case 4:
				if (program[i] >= '0' && program[i] <= '9')
				{
					if (start == -1)
						start = i;
					if (program[i + 1] < '0' || program[i + 1] > '9')
					{
						state.oQ = strToInt(program, start, i);
						start = -1;
					}
				}
				break;

			case 5:
				if (program[i] == '0')
					state.oS = false;
				if (program[i] == '1')
					state.oS = true;
				break;

			case 6:
				state.dir = program[i];
				break;
			}
			
		}
		else
		{
			if (!outside)
			{
				outside = true;
			}
		}

		if (program[i] == '\n')
		{
			count = 0;
			++program_count;
			m_Program[program_count] = state;

		}
	}

}

TuringMachine::TuringMachine(const TuringMachine& TM)
{
	delete this->m_Program;
	m_ProgramLength = TM.m_ProgramLength;
	m_TapeLength = TM.m_TapeLength;
	this->m_Program = new State[m_ProgramLength];
	for (int i = 0; i < m_ProgramLength; ++i)
	{
		m_Program[i] = TM.m_Program[i];
	}
}

TuringMachine::~TuringMachine()
{
	delete[] m_Program;
}

TuringMachine& TuringMachine::operator=(const TuringMachine& TM)
{
	delete this->m_Program;
	m_ProgramLength = TM.m_ProgramLength;
	m_TapeLength = TM.m_TapeLength;
	this->m_Program = new State[m_ProgramLength];
	for (int i = 0; i < m_ProgramLength; ++i)
	{
		m_Program[i] = TM.m_Program[i];
	}
	return *this;
}

TuringMachine& TuringMachine::operator=(TuringMachine&& TM)
{
	delete this->m_Program;
	m_ProgramLength = TM.m_ProgramLength;
	m_TapeLength = TM.m_TapeLength;
	this->m_Program = new State[m_ProgramLength];
	for (int i = 0; i < m_ProgramLength; ++i)
	{
		m_Program[i] = TM.m_Program[i];
	}
	return *this;

	delete[] TM.m_Program;
	TM.m_ProgramLength = -1;
	TM.m_TapeLength= -1;
	return *this;
}

int TuringMachine::strToInt(std::string str, int first, int last) // Digits from first to last inclusively
{
	int len = last - first + 1;
	int power = 1;
	int value = 0;
	for (int i = 1; i < len; i++)
	{
		power *= 10;
	}
	for (int i = first; i <= last; i++)
	{
		int digit = (int)(str[i]) - (int)'0';
		value += power * digit;
		power /= 10;
	}
	return value;
}

std::string TuringMachine::calc(std::string const& str, int maxSteps)
{
	std::string tape = str;
	int currentQ = m_Program[0].iQ;
	int head = 0;
	bool isFinished = false;
	for (long long int i = 0; i < maxSteps; ++i)
	{
		for (int j = 0; j < m_ProgramLength; ++j)
		{
			if (m_Program[j].iQ == currentQ && m_Program[j].iS == (tape[head] - '0'))
			{
				currentQ = m_Program[j].oQ;
				tape[head] = char(m_Program[j].oS + '0');

				if (m_Program[j].dir == 'L')
				{
					if (str.length() < m_TapeLength)
					{
						if (head == 0)
							tape = "0" + tape;
						else
							--head;
					}
					else
						head = 0;
				}
				else if (m_Program[j].dir == 'R')
				{
					if (str.length() < m_TapeLength)
					{
						if (head + 1 == tape.length())
							tape += "0";
						++head;
					}
					else
						head = 0;
					
				}
				break;
			}
			if (j == m_ProgramLength - 1)
			{
				isFinished = true;
				break;
			}
		}

		if (isFinished == true)
		{
			break;
		}

		if (i == maxSteps - 1)
		{
			tape = "Not applicable";
			return tape;
		}

	}

	int lastPos = -1;
	for (int i = head; i < tape.length(); ++i)
	{
		if (tape[i] == '1')
		{
			lastPos = i;
		}
	}
	std::string returnTape = "";
	for (int j = head; head <= lastPos; ++head)
	{
		returnTape += tape[j];
	}
	return returnTape;
}
