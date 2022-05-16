#pragma once

#include <string>
#include <algorithm>
#include <iostream>

/*
Friend functions could be outside the class without the keyword but I think it is more neat to place it within the class
Also, I couldn't clear all lnt-arithmetic-overflow warnings because of essential casts
(and precompiler warning ingore commands looked somewhat weird in the code)
*/

class BigInteger
{
public:

	BigInteger();
	BigInteger(int value);
	BigInteger(unsigned int value);
	BigInteger(unsigned long int value);
	BigInteger(long long int value);
	BigInteger(unsigned long long  int value);
	BigInteger(const std::string& strValue);
	BigInteger(const BigInteger& bigInt);
	BigInteger(BigInteger&& bigInt) noexcept;
	~BigInteger();

	BigInteger& operator=	(const BigInteger& bigInt);
	BigInteger& operator=	(BigInteger&& bigInt) noexcept;
	
	BigInteger flip();
	int8_t compare(const BigInteger& bigInt) const;
	bool modulusBinaryCompare(const BigInteger& bigInt);
	void leftRadixShift(uint32_t param);
	void rightRadixShift(uint32_t param);
	void radixRise(uint64_t shift);
	void divideBy(const BigInteger& right);

	uint32_t charToUInt32(char character);
	void expandStorage(uint64_t size);
	void shrinkStorage(uint64_t size);	
	void resizeStorage(uint64_t size);
	void removeLeadingZeros();

	friend bool operator>	(const BigInteger& left, const BigInteger& right);
	friend bool operator>=	(const BigInteger& left, const BigInteger& right);
	friend bool operator<	(const BigInteger& left, const BigInteger& right);
	friend bool operator<=	(const BigInteger& left, const BigInteger& right);
	friend bool operator==	(const BigInteger& left, const BigInteger& right);
	friend bool operator!=	(const BigInteger& left, const BigInteger& right);

	BigInteger& operator+=		(const BigInteger& bigInt);
	BigInteger& operator-=		(const BigInteger& bigInt);
	BigInteger& operator*=		(const BigInteger& bigInt);
	BigInteger& operator/=		(const BigInteger& bigInt);
	BigInteger& operator%=		(const BigInteger& bigInt);
	BigInteger& operator&=		(const BigInteger& bigInt);
	BigInteger& operator|=		(const BigInteger& bigInt);
	BigInteger& operator^=		(const BigInteger& bigInt);
	BigInteger& operator<<=		(uint64_t bias);
	BigInteger& operator>>=		(uint64_t bias);

	friend BigInteger operator+		(BigInteger left, const BigInteger& right);
	friend BigInteger operator-		(BigInteger left, const BigInteger& right);
	friend BigInteger operator*		(BigInteger left, const BigInteger& right);
	friend BigInteger operator/		(BigInteger left, const BigInteger& right);
	friend BigInteger operator%		(BigInteger left, const BigInteger& right);
	friend BigInteger operator&		(BigInteger left, const BigInteger& right);
	friend BigInteger operator|		(BigInteger left, const BigInteger& right);
	friend BigInteger operator^		(BigInteger left, const BigInteger& right);
	friend BigInteger operator<<	(BigInteger left, uint64_t right);
	friend BigInteger operator>>	(BigInteger left, uint64_t right);

	BigInteger operator~();
	BigInteger operator-() const;
	BigInteger operator+() const;

	BigInteger& operator++();
	BigInteger& operator--();
	BigInteger	operator++(int);
	BigInteger	operator--(int);

	friend std::string to_string(const BigInteger& bigInt);

	void printRadix();

private:
	uint32_t* m_Storage;
	uint64_t m_Size;
	bool m_Sign;
};