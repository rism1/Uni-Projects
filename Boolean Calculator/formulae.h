#pragma once
#include <string>
#include <stack>
#include <cmath>
#include <vector>
#include "formulaenode.h"
#include "errors.h"

class Formula {
	FormulaNode* root;
	FormulaNode* Postfix2Tree_(const char*);
	void Infix2Postfix_(const char*, char*);
	void InfixFilter_(const char*, char*);

	static const unsigned char ActionsTable[][13];
	static int ActionsRowNumber(char ch);
	static int ActionsColNumber(char ch);
public:
	Formula(const char* str) {
		size_t Len = strlen(str);
		Len = (Len < 10) ? 20 : 2 * Len;
		char* str_infix = new char[Len];
		char* str_postfix = new char[Len];
		for (int i = 0; i < 26; ++i)
		{
			m_Arr[i] = 0;
		}
		try {
			InfixFilter_(str, str_infix);
			Infix2Postfix_(str_infix, str_postfix);

			for (int i = 0; i < Len; ++i)
			{
				if (str_postfix[i] >= 'A' && str_postfix[i] <= 'Z')
				{
					++m_Arr[str_postfix[i] - 'A'];
				}
			}
			for (int i = 0; i < 26; ++i)
			{
				if (m_Arr[i] > 0)
				{
					++m_VariableCount;
				}
			}

			root = Postfix2Tree_(str_postfix);
		}
		catch (...) {
			delete[] str_postfix;
			delete[] str_infix;
			root = nullptr;
			throw;
		}
		delete[] str_infix;
		delete[] str_postfix;
	}

	void printTable();

	std::string saveDnf();

	std::string saveCnf();

	std::string saveZh();

	std::vector<bool> getCompleteness();

	double calc()const {
		if (root) return root->calc();
		return 0;
	}
	std::string str()const {
		if (root) return root->str();
		return std::string("");
	}
	int m_VariableCount = 0;
	int m_Arr[26];
};
