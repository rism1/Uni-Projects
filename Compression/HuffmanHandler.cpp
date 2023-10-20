#include "HuffmanHandler.h"

void HuffmanHandler::traverseTree(HuffmanNode* node, std::string code)
{
	if (node != nullptr && node->left == nullptr && node->right == nullptr)
	{
		node->code = code;
	}
	else
	{
		traverseTree(node->left, code + "0");
		traverseTree(node->right, code + "1");
	}
}

void HuffmanHandler::createBranch(std::string code, char id)
{
	HuffmanNode* current = m_RootNode;
	for (int i = 0; i < code.size(); ++i)
	{
		if (code[i] == '0')
		{
			if (current->left == nullptr)
				current->left = new HuffmanNode;
			current = current->left;
		}
		else if (code[i] == '1')
		{
			if (current->right == nullptr)
				current->right = new HuffmanNode;
			current = current->right;
		}
	}
	current->id = id;
}

int HuffmanHandler::binToDec(std::string value)
{
	int result = 0;
	for (int i = 0; i < value.size(); ++i)
		result = result * 2 + value[i] - '0';
	return result;
}

std::string HuffmanHandler::decToBin(int value)
{
	std::string result = "";
	std::string temp = "";
	while (value)
	{
		temp += ('0' + (value % 2));
		value /= 2;
	}
	result.append(8 - temp.size(), '0');
	for (int i = temp.size() - 1; i >= 0; --i)
	{
		result += temp[i];
	}
	return result;
}

HuffmanHandler::HuffmanHandler(std::string input, std::string output)
{
	m_InputName = input;
	m_OutputName = output;
	for (int i = 0; i < 256; ++i)
	{
		m_Nodes[i] = new HuffmanNode;
		m_Nodes[i]->id = i;
		m_Nodes[i]->freq = 0;
	}
}

HuffmanHandler::HuffmanHandler(HuffmanHandler&& handler)
{
	for (int i = 0; i < 256; ++i)
	{
		m_Nodes[i] = handler.m_Nodes[i];
		delete handler.m_Nodes[i];
	}
	m_InputName = handler.m_InputName;
	m_OutputName = handler.m_OutputName;
	handler.m_InputName.clear();
	handler.m_OutputName.clear();
}

HuffmanHandler::~HuffmanHandler()
{
	m_InputName.clear();
	m_OutputName.clear();
	for (int i = 0; i < 256; ++i)
	{
		delete m_Nodes[i];
	}
}

void HuffmanHandler::createFrequencyQueue()
{

	m_Input.open(m_InputName, std::ios::binary);
	if (!m_Input.is_open())
	{
		std::cout << "ERROR: CANNOT OPEN AN INPUT FILE!\n";
		return;
	}
	int id = 0;
	m_Input.read(reinterpret_cast<char*>(&id), 1);
	//m_Input.get(id);
	while (!m_Input.eof())
	{
		m_Nodes[id]->freq++;
		m_Input.read(reinterpret_cast<char*>(&id), 1);
		//m_Input.get(id);
	}
	m_Input.close();
	for (int i = 0; i < 256; ++i)
	{
		if (m_Nodes[i]->freq)
		{
			m_FrequencyQueue.push(m_Nodes[i]);
		}
	}
}

void HuffmanHandler::createTree()
{
	std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Comparator> tempQueue(m_FrequencyQueue);
	if (tempQueue.size() == 1)
	{
		m_RootNode = new HuffmanNode;
		m_RootNode->left = tempQueue.top();
		m_RootNode->freq = tempQueue.top()->freq;
		m_RootNode->right = new HuffmanNode();
	}
	while (tempQueue.size() > 1)
	{
		m_RootNode = new HuffmanNode;
		m_RootNode->freq = 0;
		m_RootNode->left = tempQueue.top();
		m_RootNode->freq += tempQueue.top()->freq;
		tempQueue.pop();
		m_RootNode->right = tempQueue.top();
		m_RootNode->freq += tempQueue.top()->freq;
		tempQueue.pop();
		tempQueue.push(m_RootNode);
	}
}

void HuffmanHandler::computeCodes()
{
	traverseTree(m_RootNode, "");
}

void HuffmanHandler::recreateTree()
{
	m_Input.open(m_InputName, std::ios::binary);
	if (!m_Input.is_open())
	{
		std::cout << "ERROR: CANNOT OPEN AN INPUT FILE!\n";
	}
	m_Input.ignore(1); // Skip 'H'

	int size = 0;
	m_Input.read(reinterpret_cast<char*>(&size), 1);
	if (size == 0)
	{
		size = 256;
	}
	m_RootNode = new HuffmanNode;
	for (int i = 0; i < size; ++i)
	{
		char id;
		unsigned char codeArr[16];
		m_Input.read(&id, 1);
		m_Input.read(reinterpret_cast<char*>(codeArr), 16);
		std::string codeStr = "";
		for (int i = 0; i < 16; ++i)
		{
			codeStr += decToBin(codeArr[i]);
		}
		int j = 0;
		while (codeStr[j] == '0')
		{
			++j;
		}
		codeStr = codeStr.substr(j + 1);
		createBranch(codeStr, id);
	}
	m_Input.close();
}

void HuffmanHandler::saveEncoded()
{
	createFrequencyQueue();
	createTree();
	computeCodes();

	std::string result = "";
	std::string temp = "";

	result += 'H';
	if (m_FrequencyQueue.size() == 256)
	{
		result += '\0';
	}
	else
	{
		result += (int)m_FrequencyQueue.size();
	}
	std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Comparator> tempQueue(m_FrequencyQueue);
	while (tempQueue.size() > 0)
	{
		HuffmanNode* currentNode = tempQueue.top();
		result += currentNode->id;
		temp.assign(127 - currentNode->code.size(), '0');
		temp += '1';
		temp.append(currentNode->code);
		result += (char)binToDec(temp.substr(0, 8));
		for (int i = 0; i < 15; ++i)
		{
			temp = temp.substr(8);
			result += (char)binToDec(temp.substr(0, 8));
		}
		tempQueue.pop();
	}
	temp.clear();

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

	//m_Output.put('H');

	int id = 0;
	m_Input.read(reinterpret_cast<char*>(&id), 1);
	while (!m_Input.eof())
	{
		temp += m_Nodes[id]->code;
		while (temp.size() > 8)
		{
			result += (char)binToDec(temp.substr(0, 8));
			temp = temp.substr(8);
		}
		m_Input.read(reinterpret_cast<char*>(&id), 1);
	}
	int count = 8 - temp.size();
	if (temp.size() < 8)
	{
		temp.append(count, '0');
	}
	result += (char)binToDec(temp);
	result += (char)count;

	m_Output.write(result.c_str(), result.size());
	m_Input.close();
	m_Output.close();
}

void HuffmanHandler::saveDecoded()
{
	recreateTree();

	m_Input.open(m_InputName, std::ios::binary);
	m_Output.open(m_OutputName, std::ios::binary);
	if (!m_Input.is_open())
	{
		std::cout << "ERROR: CANNOT OPEN AN INPUT FILE!\n";
		return;
	}
	if (!m_Output.is_open())
	{
		std::cout << "ERROR: CANNOT OPEN AN INPUT FILE!\n";
		return;
	}
	m_Input.ignore(1); // Skip 'H'
	unsigned char badSize;
	m_Input.read(reinterpret_cast<char*>(&badSize), 1);
	m_Input.seekg(-1, std::ios::end);
	
	int size;
	if (badSize == 0)
	{
		size = 256;
	}
	else
	{
		size = badSize;
	}


	char count;
	m_Input.read(&count, 1);
	m_Input.seekg(2 + 17 * size, std::ios::beg); // Skip 'H'

	std::vector<unsigned char> str;
	unsigned char strPart;
	m_Input.read(reinterpret_cast<char*>(&strPart), 1);
	while (!m_Input.eof())
	{
		str.push_back(strPart);
		m_Input.read(reinterpret_cast<char*>(&strPart), 1);
	}
	HuffmanNode* current = m_RootNode;
	std::string code;
	for (int i = 0; i < str.size() - 1; ++i)
	{
		code = decToBin(str[i]);
		if (i == str.size() - 2)
			code = code.substr(0, 8 - count);
		for (int j = 0; j < code.size(); ++j)
		{
			if (code[j] == '0')
				current = current->left;
			else
				current = current->right;

			if (current->left == nullptr && current->right == nullptr)
			{
				m_Output.put(current->id);
				current = m_RootNode;
			}
		}
	}
	m_Input.close();
	m_Output.close();
}

