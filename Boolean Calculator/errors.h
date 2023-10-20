#pragma once
#include <string>

class Error {
public:
	virtual std::string what() const { 
		return std::string("Unknown error"); 
	}
};
//--------------------------------------------------------------------------
class ErrorDivideByZero : public Error {
public:
	std::string what()const{ 
		return std::string("Divide by zero");  
	}
};
//--------------------------------------------------------------------------
class ErrorPostfix : public Error {
	std::string str;
	int index;
public:
	ErrorPostfix(const char* s, int position) 
		: str(s), index(position) {}
	std::string what()const {
		return std::string("Error in postfix string '") + str
			 + std::string("' at position ") 
			 + std::to_string(index+1);
	}
};
//--------------------------------------------------------------------------
class ErrorUnknownFunction : public Error {
	std::string funname;
public:
	ErrorUnknownFunction(const char* s) : funname(s){}
	ErrorUnknownFunction(char s) : funname(1, s) {}
	std::string what()const { 
		return std::string("Unknown function name ") + funname; 
	}
};
//--------------------------------------------------------------------------
class ErrorFunctionsTable : public Error {
public:
	std::string what()const {
		return std::string("Functions Table: Out of bounds");
	}
};
//--------------------------------------------------------------------------
class ErrorRValue : public Error {
public:
	std::string what()const {
		return std::string("Error RValue");
	}
};
//--------------------------------------------------------------------------
class ErrorBracketsClose : public Error {
	std::string str;
	int index;
public:
	ErrorBracketsClose(const char* s, int position) : str(s), index(position) {}
	std::string what()const {
		return std::string("Error with brackets. There are no ( for ) in '") + str
			+ std::string("' at position ") + std::to_string(index + 1);
	}
};
//--------------------------------------------------------------------------
class ErrorBracketsOpen : public Error {
	std::string str;
	int index;
public:
	ErrorBracketsOpen(const char* s, int position) : str(s), index(position) {}
	std::string what()const {
		return std::string("Error with brackets. There are no ) for ( in '") + str
			+ std::string("' at position ") + std::to_string(index + 1);
	}
};
//--------------------------------------------------------------------------
class ErrorNoID: public Error {
	std::string str;
	int index;
public:
	ErrorNoID(const char* s, int position) : str(s), index(position) {}
	std::string what()const {
		return std::string("Variable X doesn't have an identification number in '") + str
			+ std::string("' at position ") + std::to_string(index + 1);
	}
};
//--------------------------------------------------------------------------
class InfixError : public Error {
	std::string str;
public:
	InfixError(const char* s="Unknown character") : str(s) {}
	std::string what()const {
		return str;
	}
};


