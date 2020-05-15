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
	// случайное время прихода
	double timeInput;

	// случайное время работы
	double timeWork;

	// во сколько заявка должна была начать обрабатываться от 0
	double globalTimeInput;

	// во сколько заявка закончит работу
	// int globalTimeEndWork;

	// во сколько заявка реально начала работу
	double globalTimeStartWork;

	// цикл обработки
	int countWork;

	// ожидание
	double waitTime;

	int id;
};

int GenTimeInput(int timeBegin = timeInputBegin, int timeEnd = timeInputEnd);

int GenTimeWork(int timeBegin = timeWorkBegin, int timeEnd = timeWorkEnd);

int GetMinValue(int val1, int val2);

double GenTimeInputDouble(double timeEnd = timeInputEndDouble, double timeBegin = timeInputBeginDouble);

double GenTimeWorkDouble(double timeEnd = timeWorkEndDouble, double timeBegin = timeWorkBeginDouble);
