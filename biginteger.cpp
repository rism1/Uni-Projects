#include "biginteger.h"

const int INT_SIZE = sizeof(uint32_t); // 4-byte base int size

BigInteger::BigInteger()
{
	m_Sign = false;
	m_Storage = new uint32_t[1];
	m_Storage[0] = 0;
	m_Size = 1;
}

BigInteger::BigInteger(int32_t value)
{
	uint32_t digit;
	if (value < 0)
	{
		m_Sign = true;
		digit = (uint32_t)(value * (-1));
	}
	else
	{
		digit = (uint32_t)value;
		m_Sign = false;
	}
	m_Size = 1;
	m_Storage = new uint32_t[1];
	m_Storage[0] = digit;
}

BigInteger::BigInteger(uint32_t value)
{
	m_Sign = false;
	m_Size = 1;
	m_Storage = new uint32_t[1];
	m_Storage[0] = value;
}

BigInteger::BigInteger(unsigned long int value)
{
	m_Sign = false;
	uint64_t n = sizeof(uint64_t) / INT_SIZE;
	m_Size = n;
	m_Storage = new uint32_t[n];
	uint32_t maxUInt32 = UINT32_MAX;
	for (uint32_t i = 0; i < n; ++i)
	{
		m_Storage[i] = value % maxUInt32;
		value /= maxUInt32;
	}
	removeLeadingZeros();
}

BigInteger::BigInteger(uint64_t value)
{
	m_Sign = false;
	uint64_t n = sizeof(uint64_t) / INT_SIZE;
	m_Size = n;
	m_Storage = new uint32_t[n];
	uint32_t maxUInt32 = UINT32_MAX;
	for (uint32_t i = 0; i < n; ++i)
	{
		m_Storage[i] = value % maxUInt32;
		value /= maxUInt32;
	}
	removeLeadingZeros();
}

BigInteger::BigInteger(int64_t value)
{
	if (value < 0)
	{
		m_Sign = true;
		value = ~value + 1;
	}
	else
	{
		m_Sign = false;
	}
	uint64_t n = sizeof(int64_t) / INT_SIZE;
	m_Size = n;
	m_Storage = new uint32_t[n];
	uint32_t maxUInt32 = UINT32_MAX;
	for (uint64_t i = 0; i < n; ++i)
	{
		m_Storage[i] = value % maxUInt32;
		value /= maxUInt32;
	}
	removeLeadingZeros();
}

BigInteger::BigInteger(const std::string& strValue)
{
	if (strValue == "" || strValue == "-")
	{
		throw std::invalid_argument("String is empty!");
	}
	uint64_t startPos = 0;
	if (strValue[0] == '-')
	{
		m_Sign = true;
		++startPos;
	}
	else
	{
		m_Sign = false;
	}

	m_Size = 1;
	m_Storage = new uint32_t[m_Size];
	m_Storage[0] = 0;

	for (uint64_t i = startPos; i < strValue.size(); ++i)
	{
		if (strValue[i] < '0' || strValue[i] > '9')
		{
			throw std::invalid_argument("String should consist of digits!");
		}
		rightRadixShift(10);
		radixRise(charToUInt32(strValue[i]));
	}
}

BigInteger::BigInteger(const BigInteger& bigInt)
{
	m_Sign = bigInt.m_Sign;
	m_Size = bigInt.m_Size;
	m_Storage = new uint32_t[m_Size];
	
	for (uint64_t i = 0; i < m_Size; ++i)
	{
		m_Storage[i] = bigInt.m_Storage[i];
	}
}

BigInteger::BigInteger(BigInteger&& bigInt) noexcept
{
	m_Sign = bigInt.m_Sign;
	m_Size = bigInt.m_Size;
	m_Storage = bigInt.m_Storage;

	bigInt.m_Sign = false;
	bigInt.m_Size = 0;
	bigInt.m_Storage = nullptr;

}

BigInteger::~BigInteger()
{
	delete[] m_Storage;
}

BigInteger& BigInteger::operator=(const BigInteger& bigInt)
{
	if (this != &bigInt)
	{
		if (m_Storage)
		{
			delete[] m_Storage;
		}
		m_Size = bigInt.m_Size;
		m_Sign = bigInt.m_Sign;
		m_Storage = new uint32_t[m_Size];

		for (uint64_t i = 0; i < m_Size; ++i)
		{
			m_Storage[i] = bigInt.m_Storage[i];
		}
	}
	return *this;
}

BigInteger& BigInteger::operator=(BigInteger&& bigInt) noexcept
{
	if (this != &bigInt)
	{
		if (m_Storage)
		{
			delete m_Storage;
		}

		m_Size = bigInt.m_Size;
		m_Sign = bigInt.m_Sign;
		m_Storage = bigInt.m_Storage;

		bigInt.m_Size = 0;
		bigInt.m_Sign = false;
		bigInt.m_Storage = nullptr;
	}
	return *this;
}

BigInteger BigInteger::flip()
{
	for (uint32_t i = 0; i < m_Size; ++i)
	{
		m_Storage[i] = ~m_Storage[i];
	}
	return *this;
}

int8_t BigInteger::compare(const BigInteger& bigInt) const
{
	// Return values: 1 - object is greater; 0 - are equal; -1 - argument is greater 
	
	if (m_Size == 1 && m_Storage[0] == 0 && bigInt.m_Size == 1 && bigInt.m_Storage[0] == 0)
	{
		return 0;
	}

	if (m_Size > bigInt.m_Size)
	{
		if (m_Sign == false)
			return 1;
		else
			return -1;
	}
	else if (m_Size < bigInt.m_Size)
	{
		if (bigInt.m_Sign == false)
			return -1;
		else
			return 1;
	}
	else
	{
		for (uint64_t i = m_Size; i--;)
		{
			if (m_Storage[i] > bigInt.m_Storage[i])
			{
				if (m_Sign == false)
					return 1;
				else
					return -1;
			}
			else if (m_Storage[i] < bigInt.m_Storage[i])
			{
				if (bigInt.m_Sign == false)
					return -1;
				else
					return 1;
			}
			else
			{
				if (i == 0)
				{
					if (m_Sign && !bigInt.m_Sign)
						return -1;
					else if (!m_Sign && bigInt.m_Sign)
						return 1;
					else
						return 0;
				}
			}
		}
	}
	return 0;	// No path here
}

bool BigInteger::modulusBinaryCompare(const BigInteger& bigInt)
{
	if (m_Size > bigInt.m_Size)
	{
		return true;
	}
	int64_t delta = 0;
	if (m_Size == bigInt.m_Size)
	{
		for (uint64_t i = m_Size; i--;)
		{
			delta = (int64_t)m_Storage[i] - (int64_t)bigInt.m_Storage[i];
			if (delta != 0)
				break;
			if (i == 0)
				break;
		}
		if (delta < 0)
			return false;
		return true;
	}
	return false;
}

void BigInteger::leftRadixShift(uint32_t param)
{
	uint64_t shift = 0;
	for (uint64_t i = m_Size; i--;)
	{
		uint64_t value = (m_Storage[i] + (shift << (INT_SIZE * 8)));
		m_Storage[i] = (uint32_t) (value / param);
		shift = value % param;
		if (i == 0)
		{
			break;
		}
	}
	removeLeadingZeros();
}

void BigInteger::rightRadixShift(uint32_t param)
{
	uint64_t shift = 0;
	for (uint64_t i = 0; i < m_Size; ++i)
	{
		uint64_t value = (uint64_t)m_Storage[i] * (uint64_t)param + shift;
		m_Storage[i] = value;
		shift = (value >> (INT_SIZE * 8));
	}
	if (shift)
	{
		expandStorage(1);
		m_Storage[m_Size - 1] = shift;
	}
}

void BigInteger::radixRise(uint64_t shift)
{
	for (uint64_t i = 0; i < m_Size; ++i)
	{
		uint64_t value = (uint64_t)m_Storage[i] + shift;
		m_Storage[i] = value;
		shift = (value >> (INT_SIZE * 8));
		if (!shift)
		{
			break;
		}
	}
	if (shift)
	{
		expandStorage(1);
		m_Storage[m_Size - 1] = shift;
	}
}

void BigInteger::divideBy(const BigInteger& right)
{
	uint32_t maxUint32 = UINT32_MAX;
	uint64_t maxUint32_ = (uint64_t)maxUint32 + 1;
	uint64_t param = (uint32_t)(maxUint32_ / (right.m_Storage[right.m_Size - 1] + 1));

	BigInteger tempLeft = *this;
	BigInteger tempRight = right;

	tempLeft.rightRadixShift(param);
	tempRight.rightRadixShift(param);
	tempLeft.removeLeadingZeros();
	tempRight.removeLeadingZeros();
	
	uint64_t sizeLeft = tempLeft.m_Size;
	uint64_t sizeRight = tempRight.m_Size;
	uint64_t delta = sizeLeft - sizeRight + 1;

	resizeStorage(delta);

	BigInteger newLeft = BigInteger();
	newLeft.resizeStorage(sizeRight + 1);
	BigInteger newRight = BigInteger();

	

	for (uint64_t i = sizeLeft - sizeRight, j = 0; j < newLeft.m_Size; ++i, ++j)
	{
		if (i < sizeLeft)
			newLeft.m_Storage[j] = tempLeft.m_Storage[i];
		else
			newLeft.m_Storage[j] = 0;
	}

	if (sizeLeft < tempLeft.m_Size)
		newLeft.m_Storage[sizeRight] = tempLeft.m_Storage[sizeLeft];
	else
		newLeft.m_Storage[sizeRight] = 0;

	for (uint64_t i = 0; i < delta; ++i)
	{
		newLeft.m_Storage[0] = tempLeft.m_Storage[sizeLeft - sizeRight - i];
		uint64_t position = delta - i - 1;

		uint64_t temp1;
		if (sizeRight < newLeft.m_Size)
			temp1 = newLeft.m_Storage[sizeRight] * maxUint32_;
		else
			temp1 = 0;

		uint64_t temp2;

		if (sizeRight - 1 < newLeft.m_Size)
			temp2 = newLeft.m_Storage[sizeRight - 1];
		else
			temp2 = 0;
		uint64_t temp3 = (temp1 + temp2) / (uint64_t)tempRight.m_Storage[tempRight.m_Size - 1];

		uint32_t temp;
		if (temp3 < maxUint32)
			temp = temp3;
		else
			temp = maxUint32;

		newRight = tempRight;
		newRight.rightRadixShift(temp);

		while (newLeft < newRight)
		{
			uint32_t shift = 0;
			for (uint64_t i = 0; i < tempRight.m_Size; ++i)
			{
				uint64_t value = (uint64_t)tempRight.m_Storage[i] + (uint64_t)shift;
				shift = newRight.m_Storage[i] < value;	// false = 0; true = 1
				newRight.m_Storage[i] -= value;
			}
			if (shift)
			{
				newRight.m_Storage[tempRight.m_Size] -= shift;
			}
			--temp;
		}

		uint64_t carry = 0;
		uint64_t maxSize;

		if (newLeft.m_Size < newRight.m_Size)
			maxSize = newLeft.m_Size;
		else
			maxSize = newRight.m_Size;

		for (uint64_t j = 0; j < maxSize; ++j)
		{
			uint64_t temp4 = (uint64_t)carry + newRight.m_Storage[j];
			carry = (uint64_t) (newLeft.m_Storage[j] < temp4); // false = 0; true = 1
			newLeft.m_Storage[j] -= temp4;
		}

		for (uint64_t j = sizeRight; j > 0; --j)
		{
			newLeft.m_Storage[j] = newLeft.m_Storage[j - 1];
		}
		m_Storage[position] = temp;
	}
	removeLeadingZeros();
}

uint32_t BigInteger::charToUInt32(char character)
{
	return (uint32_t)(character - '0');
}

void BigInteger::expandStorage(uint64_t add)
{
	if (add == 0)
	{
		return;
	}
	uint32_t* newStorage = new uint32_t[m_Size + add];
	for (uint64_t i = 0; i < m_Size; ++i)
	{
		newStorage[i] = m_Storage[i];
	}
	delete[] m_Storage;
	m_Storage = newStorage;
	m_Size += add;
}

void BigInteger::shrinkStorage(uint64_t reduce)
{
	if (reduce >= m_Size)
	{
		return;
	}
	uint64_t newSize = m_Size - reduce;
	uint32_t* newStorage = new uint32_t[newSize];
	for (uint64_t i = 0; i < newSize; ++i)
	{
		newStorage[i] = m_Storage[i];
	}
	delete[] m_Storage;
	m_Storage = newStorage;
	m_Size -= reduce;
}

void BigInteger::resizeStorage(uint64_t size)
{
	if (m_Size == size)
	{
		return;
	}
	uint32_t* newStorage = new uint32_t[size];

	if (m_Size > size)
	{
		for (uint64_t i = 0; i < size; ++i)
		{
			newStorage[i] = m_Storage[i];
		}
	}
	else
	{
		for (uint64_t i = 0; i < size; ++i)
		{
			if (i < m_Size)
				newStorage[i] = m_Storage[i];
			else
				newStorage[i] = 0;
		}	
	}

	delete[] m_Storage;
	m_Storage = newStorage;
	m_Size = size;
}

void BigInteger::removeLeadingZeros()
{
	uint64_t size = m_Size;
	while (m_Size > 1 && m_Storage[size - 1] == 0)
	{
		shrinkStorage(1);
		--size;
	}
}

bool operator>(const BigInteger& left, const BigInteger& right)
{
	if (left.compare(right) == 1)
		return 1;
	else
		return 0;
}

bool operator>=(const BigInteger& left, const BigInteger& right)
{
	if (left.compare(right) >= 0)
		return 1;
	else
		return 0;
}

bool operator<(const BigInteger& left, const BigInteger& right)
{
	if (left.compare(right) == -1)
		return 1;
	else
		return 0;
}

bool operator<=(const BigInteger& left, const BigInteger& right)
{
	if (left.compare(right) <= 0)
		return 1;
	else
		return 0;
}

bool operator==(const BigInteger& left, const BigInteger& right)
{
	if (left.compare(right) == 0)
		return 1;
	else
		return 0;
}

bool operator!=(const BigInteger& left, const BigInteger& right)
{
	if (left.compare(right) != 0)
		return 1;
	else
		return 0;
}

BigInteger& BigInteger::operator+=(const BigInteger& bigInt)
{
	if ((m_Sign == bigInt.m_Sign) &&
		(bigInt.m_Size == 1))
	{
		radixRise(bigInt.m_Storage[0]);
		return *this;
	}

	if (m_Sign == bigInt.m_Sign)
	{
		uint64_t shift = 0;
		if (bigInt.m_Size > m_Size)
		{
			resizeStorage(bigInt.m_Size);
		}
		for (uint64_t i = 0; i < bigInt.m_Size; ++i)
		{
			uint64_t value = (uint64_t)bigInt.m_Storage[i] + (uint64_t)m_Storage[i] + shift;
			shift = (value >> (INT_SIZE * 8));
			m_Storage[i] = value;
		}
		if (shift)
		{
			expandStorage(1);
			m_Storage[m_Size - 1] = shift;
		}
		removeLeadingZeros();
	}
	else if (bigInt.m_Sign)
	{
		BigInteger temp = bigInt;
		temp.m_Sign = false;
		*this -= temp;
	}
	else
	{
		BigInteger temp = *this;
		temp.m_Sign = false;
		*this = bigInt - temp;
	}

	return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& bigInt)
{
	if ((m_Sign == bigInt.m_Sign) &&
		(*this == bigInt))
	{
		*this = BigInteger();
		return *this;
	}

	if (m_Sign == bigInt.m_Sign)
	{
		if (modulusBinaryCompare(bigInt))
		{
			uint32_t shift = 0;
			for (uint64_t i = 0; i < bigInt.m_Size; ++i)
			{
				uint64_t value = (uint64_t)bigInt.m_Storage[i] + (uint64_t)shift;
				shift = (uint32_t)(m_Storage[i] < value);	// false = 0; true = 1
				m_Storage[i] -= value;
			}
			if (shift)
			{
				m_Storage[bigInt.m_Size] -= shift;
			}
		}
		else
		{
			BigInteger temp = bigInt - *this;
			*this = temp;
			m_Sign = m_Sign ? 0 : 1;
		}
	}
	else if (bigInt.m_Sign)
	{
		BigInteger temp = bigInt;
		temp.m_Sign = false;
		*this += temp;
	}
	else
	{
		BigInteger temp = bigInt;
		temp.m_Sign = true;
		*this += temp;
	}
	removeLeadingZeros();
	return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& bigInt)
{
	if (bigInt == BigInteger() || *this == BigInteger())
	{
		*this = BigInteger();
		return *this;
	}

	if (bigInt.m_Size == 1)
	{
		rightRadixShift(bigInt.m_Storage[0]);
		if (m_Sign == bigInt.m_Sign)
			m_Sign = false;
		else
			m_Sign = true;
		return *this;
	}

	BigInteger temp = BigInteger();
	temp.resizeStorage(m_Size + bigInt.m_Size + 1);
	if (m_Sign == bigInt.m_Sign)
		temp.m_Sign = false;
	else
		temp.m_Sign = true;

	uint64_t value;
	uint64_t k = 0;
	uint64_t m = 0;
	for (uint64_t i = 0; i < m_Size; ++i)
	{
		uint64_t shift = 0;
		for (uint64_t j = 0; j < bigInt.m_Size; ++j)
		{
			value = (uint64_t)m_Storage[i] * (uint64_t)bigInt.m_Storage[j] + (uint64_t)temp.m_Storage[k] + shift;
			temp.m_Storage[k] = (uint32_t) value;
			shift = (value >> (INT_SIZE * 8));
			++k;
		}
		temp.m_Storage[k] += shift;
		++m;
		k = m;
	}
	temp.removeLeadingZeros();
	*this = temp;
	return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& bigInt)
{
	if (bigInt == BigInteger() || *this == BigInteger())
	{
		return *this;
	}


	if (bigInt.m_Size == 1)
	{
		leftRadixShift(bigInt.m_Storage[0]);
		if (m_Sign == bigInt.m_Sign)
			m_Sign = false;
		else
			m_Sign = true;
		return *this;
	}

	divideBy(bigInt);
	if (m_Sign == bigInt.m_Sign)
		m_Sign = false;
	else
		m_Sign = true;
	return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& bigInt)
{
	*this = *this - (*this / bigInt) * bigInt;
	return *this;
}

BigInteger& BigInteger::operator&=(const BigInteger& bigInt)
{
	if (bigInt.m_Sign)
	{
		BigInteger temp = BigInteger(bigInt);
		temp.flip();
		temp -= 1;

		uint64_t minSize;
		if (m_Size < temp.m_Size)
			minSize = m_Size;
		else
			minSize = temp.m_Size;
		for (uint64_t i = 0; i < minSize; ++i)
		{
			m_Storage[i] &= temp.m_Storage[i];
		}
		resizeStorage(minSize);
		removeLeadingZeros();

		if (m_Storage[m_Size - 1] >= (1u << 31))
		{
			flip();
			*this += 1;
			m_Sign = true;
		}
		return *this;
	}

	uint64_t minSize;
	if (m_Size < bigInt.m_Size)
		minSize = m_Size;
	else
		minSize = bigInt.m_Size;
	for (uint64_t i = 0; i < minSize; ++i)
	{
		m_Storage[i] &= bigInt.m_Storage[i];
	}
	resizeStorage(minSize);
	removeLeadingZeros();

	if (m_Storage[m_Size - 1] >= (1u << 31))
	{
		flip();
		*this += 1;
		m_Sign = true;
	}
	return *this;
}

BigInteger& BigInteger::operator|=(const BigInteger& bigInt)
{
	if (bigInt.m_Sign)
	{
		BigInteger temp = BigInteger(bigInt);
		temp.flip();
		temp -= 1;

		uint64_t maxSize;
		if (m_Size > temp.m_Size)
			maxSize = m_Size;
		else
			maxSize = temp.m_Size;
		resizeStorage(maxSize);
		for (uint64_t i = 0; i < maxSize; ++i)
		{
			m_Storage[i] |= temp.m_Storage[i];
		}
		removeLeadingZeros();

		if (m_Storage[m_Size - 1] >= (1u << 31))
		{
			flip();
			*this += 1;
			m_Sign = true;
		}
		return *this;
	}

	uint64_t maxSize;
	if (m_Size > bigInt.m_Size)
		maxSize = m_Size;
	else
		maxSize = bigInt.m_Size;
	resizeStorage(maxSize);

	for (uint64_t i = 0; i < maxSize; ++i)
	{
		m_Storage[i] |= bigInt.m_Storage[i];
	}
	removeLeadingZeros();
	
	if (m_Storage[m_Size - 1] >= (1u << 31))
	{
		flip();
		*this += 1;
		m_Sign = true;
	}
	return *this;
}

BigInteger& BigInteger::operator^=(const BigInteger& bigInt)
{
	if (bigInt.m_Sign)
	{
		BigInteger temp = BigInteger(bigInt);
		temp.flip();
		temp -= 1;

		uint64_t maxSize;
		if (m_Size > temp.m_Size)
			maxSize = m_Size;
		else
			maxSize = temp.m_Size;
		resizeStorage(maxSize);

		for (uint64_t i = 0; i < maxSize; ++i)
		{
			m_Storage[i] ^= temp.m_Storage[i];
		}
		removeLeadingZeros();

		if (m_Storage[m_Size - 1] >= (1u << 31))
		{
			flip();
			*this += 1;
			m_Sign = true;
		}
		return *this;
	}

	uint64_t maxSize;
	if (m_Size > bigInt.m_Size)
		maxSize = m_Size;
	else
		maxSize = bigInt.m_Size;
	resizeStorage(maxSize);

	for (uint64_t i = 0; i < maxSize; ++i)
	{
		m_Storage[i] ^= bigInt.m_Storage[i];
	}
	removeLeadingZeros();

	if (m_Storage[m_Size - 1] >= (1u << 31))
	{
		flip();
		*this += 1;
		m_Sign = true;
	}
	return *this;
}

BigInteger& BigInteger::operator<<=(uint64_t bias)
{
	uint64_t temp;
	while (bias > 0)
	{
		if (bias >= 16)
		{
			temp = 16;
		}
		else {
			temp = bias;
		}
		bias -= temp;
		uint64_t value;
		uint64_t shift = 0;
		for (uint64_t i = 0; i < m_Size; ++i)
		{
			value = (((uint64_t)(m_Storage[i])) << temp);
			m_Storage[i] = (uint32_t)((value & ((1ULL << 33) - 1)) + shift);
			shift = (value >> (INT_SIZE * 8));
		}
		if (shift != 0)
		{
			expandStorage(1);
			m_Storage[m_Size - 1] = (uint32_t) shift;
		}
	}
	return *this;
}

BigInteger& BigInteger::operator>>=(uint64_t bias)
{
	if (m_Sign)
	{
		*this = ~*this;
		*this >>= bias;
		*this += BigInteger("1");
		m_Sign = true;
		return *this;
	}
	uint64_t temp;
	while (bias > 0)
	{
		if (bias >= 16)
		{
			temp = 16;
		}
		else
		{
			temp = bias;
		}
		bias -= temp;
		uint64_t shift = 0;
		uint64_t value;
		for (uint64_t i = m_Size; i--;)
		{
			uint64_t shift_ = m_Storage[i] & ((1 << (temp)) - 1);
			value = (((uint64_t) m_Storage[i]) >> temp);
			m_Storage[i] = (uint32_t)((value & ((1ull << 33) - 1)) + (shift << (32 - temp)));
			shift = shift_;
			if (i == 0)
			{
				break;
			}
		}
	}
	removeLeadingZeros();
	return *this;
}

BigInteger operator+(BigInteger left, const BigInteger& right)
{
	left += right;
	return left;
}

BigInteger operator-(BigInteger left, const BigInteger& right)
{
	left -= right;
	return left;
}

BigInteger operator*(BigInteger left, const BigInteger& right)
{
	left *= right;
	return left;
}

BigInteger operator/(BigInteger left, const BigInteger& right)
{
	left /= right;
	return left;
}

BigInteger operator%(BigInteger left, const BigInteger& right)
{
	left %= right;
	return left;
}

BigInteger operator&(BigInteger left, const BigInteger& right)
{
	left &= right;
	return left;
}

BigInteger operator|(BigInteger left, const BigInteger& right)
{
	left |= right;
	return left;
}

BigInteger operator^(BigInteger left, const BigInteger& right)
{
	left ^= right;
	return left;
}

BigInteger operator<<(BigInteger left, uint64_t right)
{
	left <<= right;
	return left;
}

BigInteger operator>>(BigInteger left, uint64_t right)
{
	left >>= right;
	return left;
}

BigInteger BigInteger::operator~()
{
	return -(*this) - BigInteger("1");
}

BigInteger BigInteger::operator-() const
{
	BigInteger bigInt = BigInteger();
	bigInt = *this;
	bigInt.m_Sign = m_Sign ? 0 : 1;
	return bigInt;
}

BigInteger BigInteger::operator+() const
{
	BigInteger bigInt = BigInteger();
	bigInt = *this;
	return bigInt;
}

BigInteger& BigInteger::operator++()
{
	*this += BigInteger("1");
	return *this;	
}

BigInteger& BigInteger::operator--()
{
	*this -= BigInteger("1");
	return *this;
}

BigInteger BigInteger::operator++(int)
{
	BigInteger bigInt = *this;
	++(*this);
	return bigInt;
}

BigInteger BigInteger::operator--(int)
{
	BigInteger bigInt = *this;
	--(*this);
	return bigInt;
}

std::string to_string(const BigInteger& bigInt)
{
	if (bigInt.m_Size == 0 || (bigInt.m_Size == 1 && bigInt.m_Storage[0] == 0))
	{
		return "0";
	}
	else
	{
		BigInteger temp = BigInteger(bigInt);
		std::string str;

		while (!(temp.m_Size == 1 && 
				 temp.m_Storage[0] == 0))
		{
			uint64_t shift = 0;
			for (uint64_t i = temp.m_Size; i--;)
			{
				uint64_t value = (temp.m_Storage[i] + (shift << (INT_SIZE * 8)));
				temp.m_Storage[i] = (uint32_t)(value / 10);
				shift = value % 10;
				if (i == 0)
				{
					break;
				}
			}
			temp.removeLeadingZeros();
			str += std::to_string((uint32_t)shift);
		}
		str += (temp.m_Sign ? "-" : "");
		std::reverse(str.begin(), str.end());
		return str;
	}
}

void BigInteger::printRadix()
{
	for (int i = 0; i < m_Size; ++i)
	{
		std::cout << m_Storage[i] << " ";
	}
	std::cout << "\n";
}