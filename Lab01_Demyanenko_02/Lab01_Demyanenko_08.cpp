#include "stdafx.h"

/* ------------------------------------------------------------------------ *\
   Декларация класса int4x32 в соответствии с заданием
\* ------------------------------------------------------------------------ */
// Декларация функций, реализованных на языке ассемблера
extern "C" void add4x32(__int32* sum, __int32* a, __int32* b);
extern "C" void sub4x32(__int32* sum, __int32* a, __int32* b);

class int4x32
{
public:
	__int32 _val[4]; // Внутреннее представление четвёрки 32-битных целых чисел со знаком

	// Конструктор
	inline int4x32(__int32 a = 0, __int32 b = 0, __int32 c = 0, __int32 d = 0)
	{
		_val[0] = a;
		_val[1] = b;
		_val[2] = c;
		_val[3] = d;
	}

	// Оператор сложения
	inline int4x32 operator+(int4x32& b)
	{
		int4x32 ret;
		add4x32(ret._val, _val, b._val);
		return ret;
	}

	// Оператор вычитания
	inline int4x32 operator-(int4x32& b)
	{
		int4x32 ret;
		sub4x32(ret._val, _val, b._val);
		return ret;
	}
};

/* ------------------------------------------------------------------------ *\
    Тестовый класс int4x32_test
\* ------------------------------------------------------------------------ */
class int4x32_test
{
private:
	__int32 _val[4]; // Внутреннее представление четвёрки 32-битных целых чисел со знаком

	__int32 addWithSaturation(__int32 a, __int32 b) // Сложение двух __int32 с насыщением
	{
		__int32 res = a + b;
		if (a >= 0 && b >= 0 && res < 0)
			res = 0x7FFFFFFF;
		if (a < 0 && b < 0 && res >= 0)
			res = 0x80000000;
		return res;
	}

	__int32 subWithSaturation(__int32 a, __int32 b) // Вычитание двух __int32 с насыщением
	{
		__int32 res = a - b;
		if (a >= 0 && b < 0 && res < 0)
			res = 0x7FFFFFFF;
		if (a < 0 && b >= 0 && res >= 0)
			res = 0x80000000;
		return res;
	}

public:
	// Конструкторы
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

	// Преобразование типа к int4x32
	operator int4x32()
	{
		return int4x32(_val[0], _val[1], _val[2], _val[3]);
	}

	// Сравнение
	bool operator!=(int4x32& a)
	{
		return ((_val[0] != a._val[0]) || (_val[1] != a._val[1]));
	}

	// Вывод в поток
	friend ostream& operator<<(ostream& os, const int4x32_test& a)
	{
		char string[480];
		sprintf_s(string, "%08I32X %08I32X %08I32X %08I32X ", a._val[0], a._val[1], a._val[2], a._val[3]);
		os << string;
		return os;
	}

	// Присвоение случайного значения
	int4x32_test& rand()
	{
		__int32 r0, r1, r2;
		r0 = ::rand();
		r1 = ::rand();
		r2 = ::rand();
		_val[0] = (r2 << 30) | (r1 << 15) | r0;
		r0 = ::rand();
		r1 = ::rand();
		r2 = ::rand();
		_val[1] = (r2 << 30) | (r1 << 15) | r0;
		r0 = ::rand();
		r1 = ::rand();
		r2 = ::rand();
		_val[2] = (r2 << 30) | (r1 << 15) | r0;
		r0 = ::rand();
		r1 = ::rand();
		r2 = ::rand();
		_val[3] = (r2 << 30) | (r1 << 15) | r0;
		return *this;
	}

	// Оператор сложения
	int4x32_test operator+(int4x32_test& b)
	{
		__int32 v0, v1, v2, v3;
		v0 = addWithSaturation(_val[0], b._val[0]);
		v1 = addWithSaturation(_val[1], b._val[1]);
		v2 = addWithSaturation(_val[2], b._val[2]);
		v3 = addWithSaturation(_val[3], b._val[3]);
		return int4x32_test(v0, v1, v2, v3);
	}
	
	// Оператор вычитания
	int4x32_test operator-(int4x32_test& b)
	{
		__int32 v0, v1, v2, v3;
		v0 = subWithSaturation(_val[0], b._val[0]);
		v1 = subWithSaturation(_val[1], b._val[1]);
		v2 = subWithSaturation(_val[2], b._val[2]);
		v3 = subWithSaturation(_val[3], b._val[3]);
		return int4x32_test(v0, v1, v2, v3);
	}
};

#define TEST_SIZE 1000000
#define REPORT_PERIOD 10000

int _tmain(int argc, _TCHAR* argv[])
{
	int4x32_test a_test, b_test, c_test;
	int4x32 a, b, c;

	long long start, end;
	long long asmTime = 0, cppTime = 0;

	srand((unsigned) time(NULL));

	for (int testNo = 0; testNo < TEST_SIZE; testNo++)
	{
		a = a_test.rand();
		b = b_test.rand();

		start = __rdtsc();
		c = a + b;
		end = __rdtsc();
		asmTime += end - start;
		
		start = __rdtsc();
		c_test = a_test + b_test;
		end = __rdtsc();
		cppTime += end - start;

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

	cout << setw(8) << TEST_SIZE;

	cout << endl << "Addition time: " << asmTime << " (asm)" << endl << setw(15) << " vs " << cppTime << " (C++)" << endl;
    cout << "Time profit: " << (cppTime - asmTime) * 100 / cppTime << "%" << endl << endl;
	asmTime = 0;
	cppTime = 0;

	for (int testNo = 0; testNo < TEST_SIZE; testNo++)
	{
		a = a_test.rand();
		b = b_test.rand();
		
		start = __rdtsc();
		c = a - b;
		end = __rdtsc();
		asmTime += end - start;
		
		start = __rdtsc();
		c_test = a_test - b_test;
		end = __rdtsc();
		cppTime += end - start;

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

	cout << setw(8) << TEST_SIZE;
	
	cout << endl << "Subtraction time: " << asmTime << " (asm)" << endl << setw(18) << " vs " << cppTime << " (C++)" << endl;
    cout << "Time profit: " << (cppTime - asmTime) * 100 / cppTime << "%" << endl << endl;
    
	cout << endl << "Press any key to exit...";
	_getch();
	return 0;
}

