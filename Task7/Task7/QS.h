#pragma once

const int timeInputBegin = 0;
const int timeInputEnd = 10;
const int timeWorkBegin = 1;
const int timeWorkEnd = 2;

const double timeInputBeginDouble = .0;
const double timeInputEndDouble = 10.0;
const double timeWorkBeginDouble = .0;
const double timeWorkEndDouble = 2.0;

struct request
{
	// ��������� ����� �������
	double timeInput;

	// ��������� ����� ������
	double timeWork;

	// �� ������� ������ ������ ���� ������ �������������� �� 0
	double globalTimeInput;

	// �� ������� ������ �������� ������
	// int globalTimeEndWork;

	// �� ������� ������ ������� ������ ������
	double globalTimeStartWork;

	// ���� ���������
	int countWork;

	// ��������
	double waitTime;

	int id;
};

int GenTimeInput(int timeBegin = timeInputBegin, int timeEnd = timeInputEnd);

int GenTimeWork(int timeBegin = timeWorkBegin, int timeEnd = timeWorkEnd);

int GetMinValue(int val1, int val2);

double GenTimeInputDouble(double timeEnd = timeInputEndDouble, double timeBegin = timeInputBeginDouble);

double GenTimeWorkDouble(double timeEnd = timeWorkEndDouble, double timeBegin = timeWorkBeginDouble);
