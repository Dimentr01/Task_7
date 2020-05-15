#include "QS.h"
#include <cstdlib>

int GenTimeInput(int timeBegin, int timeEnd)
{
	return rand() % (timeEnd - timeBegin + 1) + timeBegin;
}

int GenTimeWork(int timeBegin, int timeEnd)
{
	return rand() % (timeWorkBegin - timeBegin + 1) + timeBegin;
}

int GetMinValue(int val1, int val2)
{
	return val1 < val2 ? val1 : val2;
}

double GetRand(){
	return double(rand()) / RAND_MAX;
}

double GenTimeInputDouble(double timeEnd, double timeBegin)
{
	auto rnd = GetRand();
	return (timeEnd - timeBegin) * rnd + timeBegin;
}

double GenTimeWorkDouble(double timeEnd, double timeBegin)
{
	auto rnd = GetRand();
	return (timeEnd - timeBegin) * rnd + timeBegin;
}