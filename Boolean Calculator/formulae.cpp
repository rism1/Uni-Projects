#include "formulae.h"
//-----------------------------------------------------
FormulaNode* Formula::Postfix2Tree_(const char* str) {
	int index = 0; // Íîìåð ýëåìåíòà âî âõîäíîé ñòðîêå
	std::stack<FormulaNode*> S;
	char ch;
	FormulaNode* result, * left, * right;
	//& v > < + = | ^ ~
	try {
		while ((ch = str[index]) != '\0') {
			left = right = nullptr;
			switch (ch) {
			case '&':
				if (S.empty()) throw 1; right = S.top(); S.pop();
				if (S.empty()) throw 1; left = S.top(); S.pop();
				result = new AndNode(left, right);
				break;
			case 'v':
				if (S.empty()) throw 1; right = S.top(); S.pop();
				if (S.empty()) throw 1; left = S.top(); S.pop();
				result = new OrNode(left, right);
				break;
			case '>':
				if (S.empty()) throw 1; right = S.top(); S.pop();
				if (S.empty()) throw 1; left = S.top(); S.pop();
				result = new ImplicationNode(left, right);
				break;
			case '<':
				if (S.empty()) throw 1; right = S.top(); S.pop();
				if (S.empty()) throw 1; left = S.top(); S.pop();
				result = new ReverseImplicationNode(left, right);
				break;
			case '+':
				if (S.empty()) throw 1; right = S.top(); S.pop();
				if (S.empty()) throw 1; left = S.top(); S.pop();
				result = new XorNode(left, right);
				break;
			case '=':
				if (S.empty()) throw 1; right = S.top(); S.pop();
				if (S.empty()) throw 1; left = S.top(); S.pop();
				result = new EqualityNode(left, right);
				break;
			case '|':
				if (S.empty()) throw 1; right = S.top(); S.pop();
				if (S.empty()) throw 1; left = S.top(); S.pop();
				result = new ShefferStrokeNode(left, right);
				break;
			case '^':
				if (S.empty()) throw 1; right = S.top(); S.pop();
				if (S.empty()) throw 1; left = S.top(); S.pop();
				result = new NorNode(left, right);
				break;
			case '~':
				if (S.empty()) throw 1; left = S.top(); S.pop();
				result = new NotNode(left);
				break;

			default:
				if (ch >= '0' && ch <= '9')
					result = new NumNode(ch - '0');
				else if ((ch >= 'a' && ch <= 'z') ||
					(ch >= 'A' && ch <= 'Z'))
					result = new ParamNode(ch);
				else {
					if (S.empty()) throw 1;
					left = S.top();
					S.pop();
					result = new FuncNode(ch, left); // Çäåñü áóäóò ôóíêöèè
				}

			}
			S.push(result);
			++index;
		}
		if (S.size() > 1) {
			left = right = nullptr;
			throw 2;
		}
		result = S.top();
		return result;
	}
	catch (int E) {
		if (left) delete left;
		if (right) delete right;
		while (!S.empty()) {
			left = S.top();
			S.pop();
			delete left;
		}
		throw ErrorPostfix(str, index);
	}
}

//-----------------------------------------------------
/*const unsigned char Formula::ActionsTable[][11] = {
	//\0 + - * / ^ ( ) P F =
	 { 7,2,2,2,2,2,2,5,1,2,2},// \0
	 { 3,3,3,2,2,2,2,3,1,2,9},// +
	 { 3,3,3,2,2,2,2,3,1,2,9},// -
	 { 3,3,3,3,3,2,2,3,1,2,9},// *
	 { 3,3,3,3,3,2,2,3,1,2,9},// /
	 { 3,3,3,3,3,3,2,3,1,2,9},// ^
	 { 6,2,2,2,2,2,2,4,1,2,9},// (
	 { 3,3,3,3,3,3,2,3,1,8,9},// F
	 { 3,2,2,2,2,2,2,5,1,2,9} // =

};
//-----------------------------------------------------
int Formula::ActionsRowNumber(char ch) {
	switch (ch) {
	case 0: return 0;
	case '+':return 1;
	case '-':return 2;
	case '*':return 3;
	case '/':return 4;
	case '^':return 5;
	case '(':return 6;
	case '=': return 8;
	}
	return 7;
}
//-----------------------------------------------------
int Formula::ActionsColNumber(char ch) {
	switch (ch) {
	case 0: return 0;
	case '+':return 1;
	case '-':return 2;
	case '*':return 3;
	case '/':return 4;
	case '^':return 5;
	case '(':return 6;
	case ')':return 7;
	case '=':return 10;
	}
	if (ch >= 'a' && ch <= 'z') return 8;
	if (ch >= 'A' && ch <= 'Z') return 8;
	if (ch >= '0' && ch <= '9') return 8;
	return 9;
}
//-----------------------------------------------------
*/

const unsigned char Formula::ActionsTable[][13] = {
//	 \0 & v > < + = | ^ ~ ( ) P
	{ 7,2,2,2,2,2,2,2,2,2,2,5,1 }, // \0
	{ 3,3,3,3,3,3,3,3,3,2,2,3,1 }, // &
	{ 3,2,3,3,3,3,3,3,3,2,2,3,1 }, // v
	{ 3,2,3,3,3,3,3,3,3,2,2,3,1 }, // >
	{ 3,2,3,3,3,3,3,3,3,2,2,3,1 }, // <
	{ 3,2,3,3,3,3,3,3,3,2,2,3,1 }, // +
	{ 3,2,3,3,3,3,3,3,3,2,2,3,1 }, // =
	{ 3,2,3,3,3,3,3,3,3,2,2,3,1 }, // |
	{ 3,2,3,3,3,3,3,3,3,2,2,3,1 }, // ^
	{ 3,3,3,3,3,3,3,3,3,3,2,3,1 }, // ~
	{ 6,2,2,2,2,2,2,2,2,2,2,4,1 }, // (
};
//-----------------------------------------------------
int Formula::ActionsRowNumber(char ch) {
	switch (ch) {
	case 0: return 0;
	case '&':return 1;
	case 'v':return 2;
	case '>':return 3;
	case '<':return 4;
	case '+':return 5;
	case '=':return 6;
	case '|': return 7;
	case '^': return 8;
	case '~': return 9;
	case '(': return 10;
	}
	
}
//-----------------------------------------------------
int Formula::ActionsColNumber(char ch) {
	switch (ch) {
	case 0: return 0;
	case '&':return 1;
	case 'v':return 2;
	case '>':return 3;
	case '<':return 4;
	case '+':return 5;
	case '=':return 6;
	case '|':return 7;
	case '^':return 8;
	case '~':return 9;
	case '(':return 10;
	case ')':return 11;
	}
	if (ch >= '0' && ch <= '9') return 12;
	if (ch >= 'A' && ch <= 'Z') return 12;
}

void Formula::printTable()
{
	for (int i = 0; i < 26; ++i)
	{
		if (m_Arr[i] > 0)
		{
			auto pos = global::Workspace.find(ParamValue('A' + i, 0));
			if (pos == global::Workspace.end())
			{
				global::Workspace.insert(ParamValue('A' + i, 0));
			}
			std::cout << "x" << i << "\t";
		}
	}
	std::cout << "F\n";
	for (unsigned long long int i = 0; i < pow(2, m_VariableCount); ++i)
	{
		std::vector<int> temp;
		unsigned long long int one = 1;
		for (int j = 25, z = 0; j >= 0; --j)
		{
			if (m_Arr[j] > 0)
			{
				auto pos = global::Workspace.find(ParamValue('A' + j, 0));
				*pos = ParamValue('A' + j, (i >> z) & one);
				temp.push_back((i >> z) & one);
				//std::cout << ((i >> z) & unsigned long long int(1)) << "\t";
				++z;
			}
		}
		for (int j = temp.size() - 1; j >= 0; --j)
		{
			std::cout << temp[j] << "\t";
		}

		int result = calc();
		std::cout << result << "\n";
	}
}

std::string Formula::saveDnf()
{
	std::string answer = "";
	for (int i = 0; i < 26; ++i)
	{
		if (m_Arr[i] > 0)
		{
			auto pos = global::Workspace.find(ParamValue('A' + i, 0));
			if (pos == global::Workspace.end())
			{
				global::Workspace.insert(ParamValue('A' + i, 0));
			}
		}
	}
	for (unsigned long long int i = 0, count = 0; i < pow(2, m_VariableCount); ++i)
	{
		std::vector<std::pair<int, int>> temp;
		unsigned long long int one = 1;
		for (int j = 0, z = 0; j < 26; ++j)
		{
			if (m_Arr[j] > 0)
			{
				auto pos = global::Workspace.find(ParamValue('A' + j, 0));
				*pos = ParamValue('A' + j, (i >> z) & one);
				temp.push_back(std::make_pair(j, ((i >> z) & one)));
				++z;
			}
		}

		int result = calc();
		if (result == 1)
		{
			if (count != 0)
			{
				//std::cout << " v ";
				answer += " v ";
			}
			++count;
			//std::cout << "(";
			answer += "(";
			for (int i = 0; i < temp.size(); ++i)
			{
				if (temp[i].second == 1)
				{
					//std::cout << "x" << temp[i].first;
					answer += ("x" + std::to_string(temp[i].first));
				}
				else if (temp[i].second == 0)
				{
					//std::cout << "~x" << temp[i].first;
					answer += ("~x" + std::to_string(temp[i].first));
				}
				if (i != temp.size() - 1)
				{
					//std::cout << " & ";
					answer += " & ";
				}
			}
			//std::cout << ")";
			answer += ")";
		}
	}
	//std::cout << "\n";
	return answer;
}

std::string Formula::saveCnf()
{
	std::string answer = "";
	for (int i = 0; i < 26; ++i)
	{
		if (m_Arr[i] > 0)
		{
			auto pos = global::Workspace.find(ParamValue('A' + i, 0));
			if (pos == global::Workspace.end())
			{
				global::Workspace.insert(ParamValue('A' + i, 0));
			}
		}
	}
	for (unsigned long long int i = 0, count = 0; i < pow(2, m_VariableCount); ++i)
	{
		std::vector<std::pair<int, int>> temp;
		unsigned long long int one = 1;
		for (int j = 0, z = 0; j < 26; ++j)
		{
			if (m_Arr[j] > 0)
			{
				auto pos = global::Workspace.find(ParamValue('A' + j, 0));
				*pos = ParamValue('A' + j, (i >> z) & one);
				temp.push_back(std::make_pair(j, ((i >> z) & one)));
				++z;
			}
		}

		int result = calc();
		if (result == 1)
		{
			if (count != 0)
			{
				//std::cout << " & ";
				answer += " & ";
			}
			++count;
			//std::cout << "(";
			answer += "(";
			for (int i = 0; i < temp.size(); ++i)
			{
				if (temp[i].second == 1)
				{
					//std::cout << "~x" << temp[i].first;
					answer += ("~x" + std::to_string(temp[i].first));
				}
				else if (temp[i].second == 0)
				{
					//std::cout << "x" << temp[i].first;
					answer += ("x" + std::to_string(temp[i].first));
				}
				if (i != temp.size() - 1)
				{
					//std::cout << " v ";
					answer += " v ";
				}
			}
			//std::cout << ")";
			answer += ")";
		}
	}
	//std::cout << "\n";
	return answer;
}
	
std::string Formula::saveZh()
{
	std::string answer;
	for (int i = 0; i < 26; ++i)
	{
		if (m_Arr[i] > 0)
		{
			auto pos = global::Workspace.find(ParamValue('A' + i, 0));
			if (pos == global::Workspace.end())
			{
				global::Workspace.insert(ParamValue('A' + i, 0));
			}
		}
	}
	std::vector<int> first;
	unsigned long long int one = 1;
	for (unsigned long long int i = 0; i < pow(2, m_VariableCount); ++i)
	{
		for (int j = 25, z = 0; j >= 0; --j)
		{
			if (m_Arr[j] > 0)
			{
				auto pos = global::Workspace.find(ParamValue('A' + j, 0));
				*pos = ParamValue('A' + j, (i >> z) & one);
				++z;
			}
		}
		int result = calc();
		first.push_back(result);
	}

	std::vector<std::vector<int>> vecs;
	vecs.push_back(first);
	for (int i = pow(2, m_VariableCount) - 1, count = 0; i > 0; --i, ++count)
	{
		vecs.push_back(std::vector<int>());
		for (int j = 0; j < i; ++j)
		{
			vecs[count + 1].push_back(!vecs[count][j] != !vecs[count][j + 1]);
		}
	}
	int entries = 0;
	for (int i = 0; i < vecs.size(); ++i)
	{
		if (vecs[i][0])
		{
			if (i == 0)
			{
				//std::cout << "(1)";
				answer += "1";
				++entries;
			}
			else
			{
				if (entries == 0)
				{
					//std::cout << "(";
					answer += "(";
				}
				else
				{
					//std::cout << " + (";
					answer += " + (";
				}
				++entries;
				std::vector<int> vec;
				for (int j = 0; j < m_VariableCount; ++j)
				{
					int temp = i;
					if ((temp >> j) & 1)
					{
						for (int z = 25, count = 0; z >= 0; --z)
						{
							if (m_Arr[z] > 0)
							{
								if (j == count)
								{
									//std::cout << "x" << z << " & ";
									vec.push_back(z);
									break;
								}
								++count;
							}
						}
					}
				}
				for (int i = vec.size() - 1; i >= 0; --i)
				{
					if (i == 0)
					{
						//std::cout << "x" << vec[i];
						answer += ("x" + std::to_string(vec[i]));
					}
					else
					{
						//std::cout << "x" << vec[i] << " & ";
						answer += ("x" + std::to_string(vec[i]) + " & ");
					}
				}
				//std::cout << ")";
				answer += ")";

			}

		}
	}
	//std::cout << "\n";
	return answer;
}

std::vector<bool> Formula::getCompleteness()
{
	// [T0, T1, S, M, L]
	std::vector<bool> answer;
	// T0
	for (int i = 0; i < 26; ++i)
	{
		if (m_Arr[i] > 0)
		{
			auto pos = global::Workspace.find(ParamValue('A' + i, 0));
			if (pos == global::Workspace.end())
			{
				global::Workspace.insert(ParamValue('A' + i, 0));
			}
		}
	}
	for (int j = 25, z = 0; j >= 0; --j)
	{
		if (m_Arr[j] > 0)
		{
			auto pos = global::Workspace.find(ParamValue('A' + j, 0));
			*pos = ParamValue('A' + j, 0);
			++z;
		}
	}
	int result = calc();
	if (result == 0)
	{
		answer.push_back(true);
	}
	else if (result == 1)
	{
		answer.push_back(false);
	}

	// T1
	for (int j = 25, z = 0; j >= 0; --j)
	{
		if (m_Arr[j] > 0)
		{
			auto pos = global::Workspace.find(ParamValue('A' + j, 0));
			*pos = ParamValue('A' + j, 1);
			++z;
		}
	}
	result = calc();
	if (result == 1)
	{
		answer.push_back(true);
	}
	else if (result == 0)
	{
		answer.push_back(false);
	}

	// S
	unsigned long long int one = 1;
	for (unsigned long long int i = 0; i < pow(2, m_VariableCount) / 2; ++i)
	{
		for (int j = 25, z = 0; j >= 0; --j)
		{
			if (m_Arr[j] > 0)
			{
				auto pos = global::Workspace.find(ParamValue('A' + j, 0));
				*pos = ParamValue('A' + j, (i >> z) & one);
				++z;
			}
		}
		int first = calc();
		for (int j = 25, z = 0; j >= 0; --j)
		{
			if (m_Arr[j] > 0)
			{
				auto pos = global::Workspace.find(ParamValue('A' + j, 0));
				*pos = ParamValue('A' + j, !((i >> z) & one));
				++z;
			}
		}
		int second = calc();

		if (first == second)
		{
			answer.push_back(false);
			break;
		}
	}
	if (answer.size() == 2)
	{
		answer.push_back(true);
	}

	// M
	std::vector<bool> values;
	for (unsigned long long int i = 0; i < pow(2, m_VariableCount); ++i)
	{
		for (int j = 25, z = 0; j >= 0; --j)
		{
			if (m_Arr[j] > 0)
			{
				auto pos = global::Workspace.find(ParamValue('A' + j, 0));
				*pos = ParamValue('A' + j, (i >> z) & one);
				++z;
			}
		}
		int result = calc();
		values.push_back(result);
	}
	for (int i = 0, j = values.size() / 2; i < values.size() / 2; ++i, ++j)
	{
		if (values[i] > values[j])
		{
			answer.push_back(false);
			break;
		}
	}
	if (answer.size() == 3)
	{
		answer.push_back(true);
	}

	// L
	std::vector<int> first;
	for (unsigned long long int i = 0; i < pow(2, m_VariableCount); ++i)
	{
		for (int j = 25, z = 0; j >= 0; --j)
		{
			if (m_Arr[j] > 0)
			{
				auto pos = global::Workspace.find(ParamValue('A' + j, 0));
				*pos = ParamValue('A' + j, (i >> z) & one);
				++z;
			}
		}
		int result = calc();
		first.push_back(result);
	}
	std::vector<std::vector<int>> vecs;
	vecs.push_back(first);
	for (int i = pow(2, m_VariableCount) - 1, count = 0; i > 0; --i, ++count)
	{
		vecs.push_back(std::vector<int>());
		for (int j = 0; j < i; ++j)
		{
			vecs[count + 1].push_back(!vecs[count][j] != !vecs[count][j + 1]);
		}
	}
	for (int i = 0; i < vecs.size(); ++i)
	{
		if (vecs[i][0])
		{
			if (i == 0)
			{
				//std::cout << "(1)";
			}
			else
			{
				std::vector<int> vec;
				for (int j = 0; j < m_VariableCount; ++j)
				{
					int temp = i;
					if ((temp >> j) & 1)
					{
						for (int z = 25, count = 0; z >= 0; --z)
						{
							if (m_Arr[z] > 0)
							{
								if (j == count)
								{
									//std::cout << "x" << z << " & ";
									vec.push_back(z);
									break;
								}
								++count;
							}
						}
					}
				}
				if (vec.size() > 1)
				{
					answer.push_back(false);
					break;
				}

			}

		}
	}
	if (answer.size() == 4)
	{
		answer.push_back(true);
	}
	return answer;
}
//-----------------------------------------------------

void Formula::Infix2Postfix_(const char* instr, char* outstr) {
	int i = 0, j = 0; // i - èíäåêñ âî âõîäíîé ñòðîêå, j - â âûõîäíîé
	int row, col;
	unsigned char action;
	std::stack<char> S;
	do {
		col = ActionsColNumber(instr[i]);
		row = S.empty() ? 0 : ActionsRowNumber(S.top());
		action = ActionsTable[row][col];
		switch (action) {
		case 1: outstr[j] = instr[i]; ++j; ++i; break;
		case 2: S.push(instr[i]); ++i; break;
		case 3: outstr[j] = S.top(); ++j; S.pop(); break;
		case 4: S.pop(); ++i; break;
		case 5: throw ErrorBracketsClose(instr, i); break;
		case 6: throw ErrorBracketsOpen(instr, i); break;
		case 7: outstr[j] = '\0'; break;
		case 8: break;
		}
	} while (action != 7);
}

void Formula::InfixFilter_(const char* instr, char* outstr) {
	int i = 0, j = 0; // i - èíäåêñ âî âõîäíîé ñòðîêå, j - â âûõîäíîé
	char ch, prev = 0;
	char buf[256]; int bufLen = 0;
	while ((ch = instr[i++]) != 0) {
		// Ïðîïóñêàåì ïðîáåë è òàáóëÿöèþ
		if (ch == ' ' || ch == '\t') continue;

		if ((ch == 'x') || (ch >= '0' && ch <= '9')) {
			buf[bufLen++] = ch;
		}
		else {
			// Óíàðíûé ìèíóñ ïðåâðàòèòü â 0-
			if (ch == '-' && (prev == 0 || prev == '(' && bufLen == 0))
				outstr[j++] = '0';
			if (bufLen == 1)
			{
				outstr[j++] = buf[0];
				bufLen = 0;
			}
			else if (bufLen == 2)
			{
				outstr[j++] = (char)('A' + (buf[1] - '0'));
				bufLen = 0;
			}
			else if (bufLen == 3) { // Çíà÷èò â buf èìÿ ïåðåìåííîé/öèôðà è ïåðåíîñèì â âûõîäíþ ñòðîêó
				outstr[j++] = (char)('A' + ((buf[1] - '0') * 10 + (buf[2] - '0')));
				bufLen = 0;
			}
			else if (bufLen > 4) { // Çíà÷èò ôóíêöèÿ. Èùåì êîðîòêîå èìÿ è çàïèñûâàåì åãî â âûõîäíóþ ñòðîêó
				buf[bufLen] = '\0';
				//outstr[j++] = FTable[FunNumberByName(buf)].ShortName;
				outstr[j++] = FunctionsTable::get(FunctionsTable::FunNumberByName(buf)).ShortName;
		
				bufLen = 0;
			}
			outstr[j++] = ch;
		}
		prev = ch;
	}
	if (bufLen > 3) throw InfixError("Function without arguments");
	if (bufLen == 1)
	{
		outstr[j++] = buf[0];
		bufLen = 0;
	}
	if (bufLen == 2)
	{
		outstr[j++] = (char)('A' + (buf[1] - '0'));
		bufLen = 0;
	}
	else if (bufLen == 3)
	{
		outstr[j++] = (char)('A' + ((buf[1] - '0') * 10 + (buf[2] - '0')));
		bufLen = 0;
	}
	outstr[j] = '\0';
}

//-----------------------------------------------------------

