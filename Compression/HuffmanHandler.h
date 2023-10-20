#pragma once
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <iostream>

struct HuffmanNode
{
	long long int freq;
	char id;
	std::string code;
	HuffmanNode* left;
	HuffmanNode* right;
	HuffmanNode()
	{
		left = nullptr;
		right = nullptr;
		freq = -1;
		id = -1;
	}
};

class HuffmanHandler
{
private:
	HuffmanNode* m_Nodes[256];
	HuffmanNode* m_RootNode;
	std::ifstream m_Input;
	std::ofstream m_Output;
	std::string m_InputName, m_OutputName;
	class Comparator
	{
	public:
		bool operator()(const HuffmanNode* left, const HuffmanNode* right) const
		{
			return left->freq > right->freq;
		}
	};
	std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Comparator> m_FrequencyQueue;

private:
	void traverseTree(HuffmanNode* node, std::string code);
	void createBranch(std::string code, char id);
	int binToDec(std::string value);
	std::string decToBin(int value);
	void createFrequencyQueue();
	void createTree();
	void computeCodes();
	void recreateTree();

public:
	HuffmanHandler(std::string input, std::string output);
	HuffmanHandler(HuffmanHandler&& handler);
	~HuffmanHandler();
	
	void saveEncoded();
	void saveDecoded();
};