#pragma once

#include <fstream>
#include <string>
#include <map>

class LZWHandler
{
private:
	std::ifstream m_Input;
	std::ofstream m_Output;
	std::string m_InputName, m_OutputName;

	std::map<std::string, int> m_Dict;
	std::map<int, std::string> m_ReverseDict;

public:
	LZWHandler(std::string input, std::string output);
	void saveEncoded();
	void saveDecoded();
};