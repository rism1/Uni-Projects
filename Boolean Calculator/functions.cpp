#include "functions.h"

const FunctionsTable::Func FunctionsTable::FTable[] = {
		{'ÿ', "arcsin", std::asin},
		{'ý', "arccos", std::acos},
		{'þ', "sin", std::sin},
		{'ú', "cos", std::cos},
		{'ö', "arctg", std::atan},
		{'÷', "tg", std::tan},
		{'ø', "ln", std::log},
		{'ù', "exp", std::exp},
		{'ô', "abs", std::fabs},
		{'æ', "sh", std::sinh},
		{'é', "ch", std::cosh},
		{'á', "sqrt", std::sqrt},
		{'ç', "sign", sign}
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

