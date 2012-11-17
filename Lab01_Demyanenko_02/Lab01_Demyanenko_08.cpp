#include "stdafx.h"

#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <iomanip>

using std::ostream;
using std::cout;
using std::endl;
using std::setw;

/* ------------------------------------------------------------------------ *\
   Декларация класса int4x32 в соответствии с заданием
\* ------------------------------------------------------------------------ */
// Декларация функций, реализованных на языке ассемблера
extern "C" void add4x32(__int32* sum,  __int32* a, __int32* b);
extern "C" void sub4x32(__int32* diff, __int32* a, __int32* b);
extern "C" void mul4x32(__int32* prod, __int32* a, __int32* b);
extern "C" void div4x32(__int32* quot, __int32* a, __int32* b);
extern "C" void mod4x32(__int32* rem,  __int32* a, __int32* b);

class int4x32
{
public:
	__int32 _val[4];

	inline int4x32(__int32 a = 0, __int32 b = 0, __int32 c = 0, __int32 d = 0)
	{
		_val[0] = a;
		_val[1] = b;
		_val[2] = c;
		_val[3] = d;
	}

	inline int4x32 operator+(int4x32& b)
	{
		int4x32 ret;
		add4x32(ret._val, _val, b._val);
		return ret;
	}

	inline int4x32 operator-(int4x32& b)
	{
		int4x32 ret;
		sub4x32(ret._val, _val, b._val);
		return ret;
	}

    inline int4x32 operator*(int4x32& b)
    {
        int4x32 ret;
        mul4x32(ret._val, _val, b._val);
		return ret;
    }

    inline int4x32 operator/(int4x32& b)
    {
        int4x32 ret;
        div4x32(ret._val, _val, b._val);
		return ret;
    }

    inline int4x32 operator%(int4x32& b)
    {
        int4x32 ret;
        mod4x32(ret._val, _val, b._val);
		return ret;
    }
};

/* ------------------------------------------------------------------------ *\
    Тестовый класс int4x32_test
\* ------------------------------------------------------------------------ */
class int4x32_test
{
private:
	__int32 _val[4];

	__int32 addWithSaturation(__int32 a, __int32 b) const
	{
		signed long long res64 = (signed long long)a + b;
        __int32 res = (__int32)res64;
        if (res64 > 0x000000007FFFFFFF)
        {
            res = 0x7FFFFFFF;
        }
        if (res64 < (signed long long)0xFFFFFFFF80000000)
        {
            res = 0x80000000;
        }
        return res;
	}

	__int32 subWithSaturation(__int32 a, __int32 b) const
	{
		signed long long res64 = (long long)a - b;
        __int32 res = (__int32)res64;
        if (res64 > 0x000000007FFFFFFF)
        {
            res = 0x7FFFFFFF;
        }
        if (res64 < (signed long long)0xFFFFFFFF80000000)
        {
            res = 0x80000000;
        }
        return res;
	}

    __int32 mulWithSaturation(__int32 a, __int32 b) const
    {
        signed long long res64 = (signed long long)a * b;
        __int32 res = (__int32)res64;
        if (res64 > 0x000000007FFFFFFF)
        {
            res = 0x7FFFFFFF;
        }
        if (res64 < (signed long long)0xFFFFFFFF80000000)
        {
            res = 0x80000000;
        }
        return res;
    }

public:
	int4x32_test()
	{
		_val[0] = 0;
		_val[1] = 0;
		_val[2] = 0;
		_val[3] = 0;
	}

	int4x32_test(int4x32 (a))
	{
		_val[0] = a._val[0];
		_val[1] = a._val[1];
		_val[2] = a._val[2];
		_val[3] = a._val[3];
	}

	int4x32_test(__int32 v0, __int32 v1, __int32 v2, __int32 v3)
	{
		_val[0] = v0;
		_val[1] = v1;
		_val[2] = v2;
		_val[3] = v3;
	}

	operator int4x32()
	{
		return int4x32(_val[0], _val[1], _val[2], _val[3]);
	}

	bool operator!=(int4x32& a)
	{
        return ((_val[0] != a._val[0]) || (_val[1] != a._val[1]) || (_val[2] != a._val[2]) || (_val[3] != a._val[3]));
	}

	friend ostream& operator<<(ostream& os, const int4x32_test& a)
	{
		char string[480];
		sprintf_s(string, "%08I32X %08I32X %08I32X %08I32X ", a._val[0], a._val[1], a._val[2], a._val[3]);
		os << string;
		return os;
	}

	// Присвоить случайное значение младшим bitsCount битам, дополнить знаком
	int4x32_test& rand(int bitsCount = 32)
	{
        __int32 maxValue = 1 << (bitsCount - 1);
		__int32 r0, r1, r2;
		r0 = ::rand();
		r1 = ::rand();
		r2 = ::rand();
		_val[0] = (r2 << 30) | (r1 << 15) | r0;
        _val[0] %= maxValue;
		r0 = ::rand();
		r1 = ::rand();
		r2 = ::rand();
		_val[1] = (r2 << 30) | (r1 << 15) | r0;
        _val[1] %= maxValue;
		r0 = ::rand();
		r1 = ::rand();
		r2 = ::rand();
		_val[2] = (r2 << 30) | (r1 << 15) | r0;
        _val[2] %= maxValue;
		r0 = ::rand();
		r1 = ::rand();
		r2 = ::rand();
		_val[3] = (r2 << 30) | (r1 << 15) | r0;
        _val[3] %= maxValue;
		return *this;
	}

	int4x32_test operator+(const int4x32_test& b) const
	{
		__int32 v0, v1, v2, v3;
		v0 = addWithSaturation(_val[0], b._val[0]);
		v1 = addWithSaturation(_val[1], b._val[1]);
		v2 = addWithSaturation(_val[2], b._val[2]);
		v3 = addWithSaturation(_val[3], b._val[3]);
		return int4x32_test(v0, v1, v2, v3);
	}
	
	int4x32_test operator-(const int4x32_test& b) const
	{
		__int32 v0, v1, v2, v3;
		v0 = subWithSaturation(_val[0], b._val[0]);
		v1 = subWithSaturation(_val[1], b._val[1]);
		v2 = subWithSaturation(_val[2], b._val[2]);
		v3 = subWithSaturation(_val[3], b._val[3]);
		return int4x32_test(v0, v1, v2, v3);
	}

    int4x32_test operator*(const int4x32_test& b) const
    {
		__int32 v0, v1, v2, v3;
		v0 = mulWithSaturation(_val[0], b._val[0]);
		v1 = mulWithSaturation(_val[1], b._val[1]);
		v2 = mulWithSaturation(_val[2], b._val[2]);
		v3 = mulWithSaturation(_val[3], b._val[3]);
		return int4x32_test(v0, v1, v2, v3);
    }

    int4x32_test operator/(const int4x32_test& b) const
    {
        __int32 v0, v1, v2, v3;
        v0 = _val[0] / b._val[0];
        v1 = _val[1] / b._val[1];
        v2 = _val[2] / b._val[2];
        v3 = _val[3] / b._val[3];
		return int4x32_test(v0, v1, v2, v3);
    }

    int4x32_test operator%(const int4x32_test& b) const
    {
        __int32 v0, v1, v2, v3;
        v0 = _val[0] % b._val[0];
        v1 = _val[1] % b._val[1];
        v2 = _val[2] % b._val[2];
        v3 = _val[3] % b._val[3];
		return int4x32_test(v0, v1, v2, v3);
    }

    bool hasZeros() const
    {
        return _val[0] == 0 || _val[1] == 0 || _val[2] == 0 || _val[3] == 0;
    }
};

#define TESTS_COUNT 1000000 // Количество повторений теста
#define REPORT_PERIOD 10000 // Как часто выводить номер теста

int _tmain(int argc, _TCHAR* argv[])
{
	int4x32_test a_test, b_test, c_test;
	int4x32 a, b, c;

	srand((unsigned) time(NULL));

	for (int testNo = 0; testNo < TESTS_COUNT; testNo++)
	{
        switch (testNo % 5)
        {
        case 0:
            a = a_test.rand();
            b = b_test.rand();
            c = a + b;
            c_test = a_test + b_test;
            break;

        case 1:
            a = a_test.rand();
            b = b_test.rand();
            c = a - b;
            c_test = a_test - b_test;
            break;

        case 2:
            // Уменьшаем множители, чтобы произведение не всегда переполнялось
            a = a_test.rand(17);
            b = b_test.rand(17);
            c = a * b;
            c_test = a_test * b_test;
            break;

        case 3:
            a = a_test.rand();
            do
            {
                b = b_test.rand(16);
            }
            while (b_test.hasZeros());

            c = a / b;
            c_test = a_test / b_test;
            break;

        case 4:
            a = a_test.rand();
            do
            {
                b = b_test.rand(16);
            }
            while (b_test.hasZeros());

            c = a % b;
            c_test = a_test % b_test;
            break;
        }
        
		if (c_test != c)
		{
			cout << endl << endl << "*** ERROR in test " << testNo << " ***" << endl << endl;
			cout << "a      = " << (int4x32_test) a << endl;
			cout << "a_test = " << a_test << endl << endl;
			cout << "b      = " << (int4x32_test) b << endl;
			cout << "b_test = " << b_test << endl << endl;
			cout << "c      = " << (int4x32_test) c << endl;
			cout << "c_test = " << c_test << endl << endl;

			_getch();
			return(-1);
		}

		if (testNo % REPORT_PERIOD == 0)
		{
			cout << setw(8) << testNo;
		}
	}

	cout << setw(8) << TESTS_COUNT;

	cout << endl << "Press any key to exit...";
	_getch();
	return 0;
}

