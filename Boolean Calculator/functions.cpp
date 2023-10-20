#include "functions.h"

const FunctionsTable::Func FunctionsTable::FTable[] = {
		{'�', "arcsin", std::asin},
		{'�', "arccos", std::acos},
		{'�', "sin", std::sin},
		{'�', "cos", std::cos},
		{'�', "arctg", std::atan},
		{'�', "tg", std::tan},
		{'�', "ln", std::log},
		{'�', "exp", std::exp},
		{'�', "abs", std::fabs},
		{'�', "sh", std::sinh},
		{'�', "ch", std::cosh},
		{'�', "sqrt", std::sqrt},
		{'�', "sign", sign}
};
const int FunctionsTable::sizeFTable = sizeof(FunctionsTable::FTable) / sizeof(FunctionsTable::FTable[0]);


int FunctionsTable::FunNumberByShortName(char s) {
	for (int i = 0; i < sizeFTable; ++i) {
		if (FTable[i].ShortName == s)
			return i;
	}
	throw ErrorUnknownFunction(s);
}

int FunctionsTable::FunNumberByName(const char* s) {
	for (int i = 0; i < sizeFTable; ++i) {
		if (strcmp(FTable[i].Name, s) == 0)
			return i;
	}
	throw ErrorUnknownFunction(s);
}

double sign(double x) { return x == 0 ? 0 : (x < 0 ? -1.0 : 1.0); }

