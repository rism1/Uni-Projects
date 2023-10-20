#pragma once
#include <cstring>
#include <cmath>
#include "errors.h"

typedef double (*pfun)(double);

double sign(double);

class FunctionsTable {
public:
	struct Func {
		char ShortName;
		const char* Name;
		pfun fun;
	};
	static int FunNumberByShortName(char);
	static int FunNumberByName(const char*);
	static const Func& get(int i) { 
		if (i<sizeFTable && i>=0)
			return FTable[i]; 
		throw ErrorFunctionsTable();
	}
private:
	static const Func FTable[];
	static const int sizeFTable;
};
