#include "LZWHandler.h"

#include <iostream>

void LZWHandler::saveEncoded()
{
	m_Input.open(m_InputName, std::ios::binary);
	m_Output.open(m_OutputName, std::ios::binary);

	if (!m_Input.is_open())
	{
		std::cout << "ERROR: CANNOT OPEN AN INPUT FILE!\n";
		return;
	}
	if (!m_Output.is_open())
	{
		std::cout << "ERROR: CANNOT OPEN AN OUTPUT FILE!\n";
		return;
	}

	m_Output.put('L');

	char id;
	int code = m_Dict.size();
	std::string currentStr;

	id = m_Input.get();
	while (!m_Input.eof())
	{
		std::map<std::string, int>::iterator mapIterator;
		if ((mapIterator = m_Dict.find(currentStr + id)) != m_Dict.end())
		{
			currentStr += id;
		}
		else
		{
			m_Dict[currentStr + id] = code;
			++code;
			m_Output.write((char*)&m_Dict[currentStr], sizeof(m_Dict[currentStr]));
			currentStr = id;
		}
		id = m_Input.get();
	}

	std::map<std::string, int>::iterator mapIterator;
	if ((mapIterator = m_Dict.find(currentStr)) != m_Dict.end())
	{
		m_Output.write((char*)&mapIterator->second, sizeof(mapIterator->second));
	}

	m_Input.close();
	m_Output.close();
}

void LZWHandler::saveDecoded()
{
	m_Input.open(m_InputName, std::ios::binary);
	m_Output.open(m_OutputName, std::ios::binary);

	if (!m_Input.is_open())
	{
		std::cout << "ERROR: CANNOT OPEN AN INPUT FILE!\n";
		return;
	}
	if (!m_Output.is_open())
	{
		std::cout << "ERROR: CANNOT OPEN AN OUTPUT FILE!\n";
		return;
	}

	m_Input.get();

	int code;
	int prevCode;
	std::string result;

	m_Input.read((char*)&code, sizeof(code));
	m_Output << m_ReverseDict[code];
	prevCode = code;

	m_Input.read((char*)&code, sizeof(code));
	while (!m_Input.eof())
	{
		int size = m_ReverseDict.size();
		if (code < size)
		{
			m_Output << m_ReverseDict[code];
			m_ReverseDict[size] = m_ReverseDict[prevCode] + m_ReverseDict[code][0];
			prevCode = code;
		}
		else
		{
			result = m_ReverseDict[prevCode] + m_ReverseDict[prevCode][0];
			m_Output << result;
			m_ReverseDict[size] = result;
			prevCode = code;
		}
		m_Input.read((char*)&code, sizeof(code));
	}
}

LZWHandler::LZWHandler(std::string input, std::string output)
{
	m_InputName = input;
	m_OutputName = output;

	// Initialize dict
	std::string str;
	for (int i = 0; i <= 255; ++i)
	{
		str = (unsigned char)i;
		m_Dict[str] = i;
	}
	/*m_Dict["\n"] = code++;
	m_Dict["\r"] = code++;
	std::string temp = "";
	temp = temp.append(1, '\x1a');
	m_Dict[temp] = code;*/

	// Initialize reverse dict
	for (int i = 0; i <= 255; ++i)
	{
		m_ReverseDict[i] = (unsigned char)i;
	}
	//m_ReverseDict[code++] = "\n";
	//m_ReverseDict[code++] = "\r";
	//m_ReverseDict[code] = temp;
}