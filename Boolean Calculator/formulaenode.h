#pragma once
#include <string>
#include <iostream>
#include <cmath>
#include "errors.h"
#include "avltree.h"

class FormulaNode {
public:
	virtual double calc() const = 0;
	virtual std::string str() const = 0;
	virtual ~FormulaNode() {}
};
//------------------------------------------------------
class NumNode : public FormulaNode {
	const double num;
public:
	NumNode(double x) : num(x) { }
	double calc() const { return num; }
	std::string str() const {
		if (num < 0)
			return std::string("(") + std::to_string(num) + std::string(")");
		return std::to_string(num);
	}
};
//------------------------------------------------------
class BinNode : public FormulaNode {
protected:
	FormulaNode* left, * right;
public:
	BinNode(FormulaNode* L, FormulaNode* R) : left(L), right(R) {}
	~BinNode() {
		delete left;
		delete right;
		left = right = nullptr;
	}
};
//------------------------------------------------------
class AndNode : public BinNode {
public:
	AndNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}
	double calc()const { int valL = left->calc(), valR = right->calc(); return (valL && valR); }
	std::string str() const {
		return left->str() + " & " + right->str();
	}
};
//------------------------------------------------------
class OrNode : public BinNode {
public:
	OrNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}
	double calc()const { int valL = left->calc(), valR = right->calc(); return (valL || valR); }
	std::string str() const {
		return left->str() + " v " + right->str();
	}
};
//------------------------------------------------------
class ImplicationNode : public BinNode {
public:
	ImplicationNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}
	double calc()const { int valL = left->calc(), valR = right->calc(); return !valL || valR; }
	std::string str() const {
		return left->str() + " > " + right->str();
	}
};
//------------------------------------------------------
class ReverseImplicationNode : public BinNode {
public:
	ReverseImplicationNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}
	double calc()const { int valL = left->calc(), valR = right->calc(); return valL || !valR; }
	std::string str() const {
		return left->str() + " < " + right->str();
	}
};
//------------------------------------------------------
class XorNode : public BinNode {
public:
	XorNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}
	double calc()const { int valL = left->calc(), valR = right->calc(); return !valL != !valR; }
	std::string str() const {
		return left->str() + " + " + right->str();
	}
};
//------------------------------------------------------
class EqualityNode: public BinNode {
public:
	EqualityNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}
	double calc()const { int valL = left->calc(), valR = right->calc(); return valL == valR; }
	std::string str() const {
		return left->str() + " = " + right->str();
	}
};
//------------------------------------------------------
class ShefferStrokeNode : public BinNode {
public:
	ShefferStrokeNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}
	double calc()const { int valL = left->calc(), valR = right->calc(); return !(valL && valR); }
	std::string str() const {
		return left->str() + " | " + right->str();
	}
};
//------------------------------------------------------
class NorNode : public BinNode {
public:
	NorNode(FormulaNode* L, FormulaNode* R) : BinNode(L, R) {}
	double calc()const { int valL = left->calc(), valR = right->calc(); return !(valL || valR); }
	std::string str() const {
		return left->str() + " ^ " + right->str();
	}
};

//------------------------------------------------------
class UnarNode : public FormulaNode {
protected:
	FormulaNode* next;
public:
	UnarNode(FormulaNode* node) : next(node) {}
	~UnarNode() { if (next) delete next; next = nullptr; }
};
//------------------------------------------------------
class NotNode : public UnarNode {
public:
	NotNode(FormulaNode* node) : UnarNode(node) {}
	double calc()const { return !next->calc(); }
	std::string str() const {
		return " ~(" + next->str() + ")";
	}

};

//------------------------------------------------------
class ParamValue {
	char ch;
	double val;
public:
	ParamValue(char c, double value) : ch(c), val(value) {}
	double getValue() const { return val; }
	bool operator<(const ParamValue& X)const { return ch < X.ch; }
	bool operator==(const ParamValue& X) const { return ch == X.ch; }
	ParamValue& operator=(const ParamValue& X) { ch = X.ch; val = X.val; return *this; }
};
//------------------------------------------------------
namespace global {
	extern AVLTree<ParamValue> Workspace;
}
//------------------------------------------------------
class ParamNode : public FormulaNode {
	const char ch;
public:
	ParamNode(char s) : ch(s) { }
	double calc() const {
		auto pos=global::Workspace.find(ParamValue(ch,0));
		if (pos != global::Workspace.end()) {
			return (*pos).getValue();
		} else {
			double tmp;
			std::cout << "x" << ch - 'A' << " = ";
			std::cin >> tmp;
			std::cin.ignore();
			global::Workspace.insert(ParamValue(ch, tmp));
			return tmp;
		}
	}
	std::string str() const {
		std::string result = "";
		result += ("x" + std::to_string(ch-'A'));
		return result;
	}
};

//------------------------------------------------------
#include "functions.h"
class FuncNode : public UnarNode {
	int funcNumber;
public:
	FuncNode(char s, FormulaNode* node) : UnarNode(node) {
		funcNumber = FunctionsTable::FunNumberByShortName(s);
	}
	FuncNode(const char *s, FormulaNode* node) : UnarNode(node) {
		funcNumber = FunctionsTable::FunNumberByName(s);
	}
	double calc() const {
		//return (FTable[funcNumber].fun) (next->calc());
		return (FunctionsTable::get(funcNumber).fun) (next->calc());
	}

	std::string str()const {
/*		return std::string(FTable[funcNumber].Name)
			+ "(" + next->str() + ")";
*/
		return std::string(FunctionsTable::get(funcNumber).Name)
			+ "(" + next->str() + ")";
	}
};

//------------------------------------------------------
class AssignmentNode : public FormulaNode {
	ParamNode* left;
	FormulaNode* right;
public:
	AssignmentNode(FormulaNode* L, FormulaNode* R)
		: right(R), left(nullptr) {
		left = dynamic_cast<ParamNode*>(L);
		if (left == nullptr)
			throw ErrorRValue();
	}
	std::string str() const {
		return left->str() + " := " + right->str();
	}

	double calc() const {
		double result = right->calc();
		char paramName = left->str()[0];
		auto pos = global::Workspace.find(ParamValue(paramName,0));
		if (pos != global::Workspace.end()) {
			global::Workspace.erase(pos);
		}
		global::Workspace.insert(ParamValue(paramName, result));
		return result;
	}

	~AssignmentNode() {
		delete right;
		delete left;
		right = left = nullptr;
	}
};
//------------------------------------------------------






