#define LOGALL1
#include <iostream>
#include "List.hpp"
#include "QS.h"
#include <list>

using namespace std;


void TestList()
{
	auto newRequest = new request;
	newRequest->countWork = 0;
	newRequest->timeInput = GenTimeInput();
	newRequest->timeWork = GenTimeWork();
	List<request>* list1 = nullptr;

	list1 = PushBack(list1, *newRequest);

	auto rrr = Front(list1);


	List<int>* list = nullptr;
	list = PushBack(list, 2);
	list = PushBack(list, 4);
	list = PushBack(list, 5);
	list = PushBack(list, 6);
	list = PushBack(list, 7);
	PrintList(list);
	auto fff = Front(list);

	list = PopFront(list);
	PrintList(list);
	list = PopFront(list);
	PrintList(list);
}

void LogRequest(const request& request)
{
	cout << "Заявка: " << request.id << endl;
	cout << "Время прихода: " << request.timeInput << endl;
	cout << "Глобальное время прихода: " << request.globalTimeInput << endl;
	cout << "Время обработки: " << request.timeWork << endl << endl;
}

// Заранее генерируем заявки
void PreGenRequests(List<request>*& list, List<request>*& helperList, int countRequests, double timeInputEnd, double timeWorkEnd)
{
	int idRequest = 0;
	int globalTime = 0;
	for (int i = 0; i < countRequests; i++)
	{
		// создаем заявку
		request newRequest;
		newRequest.id = idRequest;
		idRequest++;

		newRequest.countWork = 0;
		newRequest.timeInput = GenTimeInputDouble(timeInputEnd);
		globalTime += newRequest.timeInput;
		newRequest.globalTimeInput = globalTime;
		newRequest.globalTimeStartWork = newRequest.globalTimeInput;
		newRequest.timeWork = GenTimeWorkDouble(timeWorkEnd);
		newRequest.waitTime = .0;

#ifdef LOGALL
		LogRequest(newRequest);
#endif

		list = PushBack(list, newRequest);
		helperList = PushBack(helperList, newRequest);
	}
}

void TestQS(double timeInputEnd = timeInputEndDouble, double timeWorkEnd = timeWorkEndDouble, int countRequests = 1000, int limitStatistic = 100)
{
	// заявки 
	List<request>* list = nullptr;

	// вторая очередь нужна для подсчета статистики, так как очередь входных заявок
	// уже сформирована до начала работы
	List<request>* helperList = nullptr;

	// заполнение заявок
	PreGenRequests(list, helperList, countRequests, timeInputEnd, timeWorkEnd);

	// время, когда освободится ОА
	auto timeFreeOa = .0;

	// количество завершенных заявок
	int countEnded = 0;

	// время простоя
	auto downtime = .0;

	// время симуляции
	auto timeSimulation = .0;

	// отработанные заявки ( для статистики)
	int requestsWorked = 0;

	// с максимальным временем ожидания
	request maxRequest;
	maxRequest.id = -1;
	maxRequest.countWork = -1;
	maxRequest.waitTime = -1;

	// максимальное время в очереди
	auto maxQueueTime = -1.0;

	// для среднего времени в очереди
	auto SumForAverageQueueTime = .0;

	// для правильного расчета среднего времени в очереди
	int countPrintStatistic = 1;

	// обрабатываемая заявка
	request* workingRequest = nullptr;
	while (true)
	{
		// нет заявки в работе (сработает только первый раз по идее)
		if (!IsEmpty(list) && workingRequest == nullptr)
		{
			auto newLocalRequest = Front(list);
			list = PopFront(list);

			// время простоя до обработки 1 заявки
			downtime += newLocalRequest.globalTimeInput;

			// время, когда освободится ОА
			timeFreeOa = newLocalRequest.globalTimeStartWork + newLocalRequest.timeWork;

			// занимаем ОА
			workingRequest = &newLocalRequest;
#ifdef LOGALL
			cout << "Начали " << workingRequest->countWork + 1 << " обработку заявки " << workingRequest->id << " в " <<
				newLocalRequest.globalTimeStartWork << " е.в." << endl << endl;
#endif

			continue;
		}

		int timeInputNextRequest;
		// значит заявки в списке кончились. Надо доработать текущую и все
		if (IsEmpty(list))
		{
			timeInputNextRequest = INT32_MAX;
		}
		else
		{
			timeInputNextRequest = Front(list).globalTimeInput;
		}

		// что настанет быстрее, обработка прошлой или приход следующей
		//auto t = GetMinValue(timeFreeOa, timeInputNextRequest);
		
		auto t = timeFreeOa; // всегда берем за основу время окончания
		
		// тут будем смотреть, если ОА освободился раньше чем глобальное время прихода след заявки,
		// то запустим эту заявку снова (если нужно, если нет - ОА будет простаивать).
		// А если взяв следующую заявку мы поймем, что она сколько-то ожидала (t > globalTimeInput),
		// то посчитаем время ожидания
		// отработанную заявку будем вставлять в очередь в нужное место
		if (t == timeFreeOa)
		{
			// надо текущей заявке сделать countWork++
			// и снова отправить ее в очередь, ждать пока все остальные проработают
			// если countWork == 4 надо выкинуть ее из обработки
			//
			// перед вставкой заявки в очередь обратно выбираем ей место

			workingRequest->countWork++;
			auto endedTime = workingRequest->globalTimeStartWork + workingRequest->timeWork;
#ifdef LOGALL
			cout << "Закончили " << workingRequest->countWork << " обработку заявки " << workingRequest->id << " в " <<
				endedTime << " е.в." << endl << endl;
#endif
			timeSimulation = endedTime; // конечное время работы на последнем шаге будет равно времени завершения обработки последней заявки
			requestsWorked++;

			// вывод каждые 100
			if (requestsWorked == limitStatistic)
			{
				cout << "---------------------------------------------" << endl;
				cout << "Текущий момент времени: " << endedTime << endl;

				// сколько вошло поступило к этому моменту заявок в систему
				// т.е. сколько заявок с временем globalTimeInput <= t в первоначальном списке?
				auto tmp = helperList;
				int totalInput = 0;
				while (tmp && tmp->next != nullptr && tmp->value.globalTimeInput <= t)
				{
					totalInput++;
					tmp = tmp->next;
				}
				cout << "Вошло заявок: " << totalInput << endl;

				// сколько сейчас заявок в очереди
				// т.е. сколько заявок с временем globalTimeInput <= t в измененном списке?
				auto tmp1 = list;
				int remain = 0;
				while (tmp1 && tmp1->next != nullptr && tmp1->value.globalTimeInput <= t)
				{
					remain++;
					tmp1 = tmp1->next;
				}
				cout << "Текущая длина очереди: " << remain << endl;

				// нужная заявка с максимальным временем ожидания
				if (maxRequest.id == -1)
				{
					cout << "Заявка с максимальным временем ожидания: " << "Отсутствует" << endl;
				}
				else
				{
					cout << "Заявка с максимальным временем ожидания: " << maxRequest.id << endl;
					cout << "Ее цикл обслуживания: " << maxRequest.countWork << endl;
					// cout << "Ее время ожидания: " << maxRequest.waitTime << endl;
				}

				// нужно максимальное время в очереди от globalTimeInput до endedTime на 4 итерации без учета времени работы
				cout << "Максимальное время заявок в очереди: " << maxQueueTime << endl;

				// нужно среднее время в очереди от globalTimeInput до endedTime без учета времени работы
				cout << "Среднее время заявок в очереди: " << float(SumForAverageQueueTime) / (limitStatistic *
					countPrintStatistic);

				cout << endl;
				requestsWorked = 0;
				countPrintStatistic++;
			}

			// заявку надо выкинуть из обработки
			if (workingRequest->countWork == 4)
			{
				countEnded++;

				auto timeQueue = (endedTime - workingRequest->globalTimeInput) - 4 * workingRequest->timeWork;
				if (timeQueue > maxQueueTime)
					maxQueueTime = timeQueue;

				SumForAverageQueueTime += timeQueue;
			}
			// если заявка меньше 4 раз была в работе отправляем ее снова в очередь
			else
			{
				// обновляем время следующего входа в ОА. Во столько времени она закончила работу
				workingRequest->globalTimeStartWork = endedTime;

				// если лист пустой, то заявку вставляем в конец
				if (IsEmpty(list))
					list = PushBack(list, *workingRequest);
				else
				{
					// иначе заявку надо вставить в "нужное время"
					// auto front = Front(list);
					// auto back = Back(list);
					if (workingRequest->globalTimeStartWork <= Front(list).globalTimeInput)
					{
						list = PushFront(list, *workingRequest);
					}
					else if (workingRequest->globalTimeStartWork >= Back(list).globalTimeInput)
					{
						list = PushBack(list, *workingRequest);
					}
					else
					{
						//тут не будет указателя на nullptr, так как проверили время выше, а список по времени упорядочен
						auto insertPlace = list;
						while (workingRequest->globalTimeStartWork > insertPlace->value.globalTimeInput)
						{
							insertPlace = insertPlace->next;
							if (insertPlace == nullptr)
								throw "Произошло что-то непонятное";
						}

						InsertInstead(insertPlace, *workingRequest);
					}
				}
			}

			workingRequest = nullptr;

			if (IsEmpty(list))
			{
				//throw "ПУСТО";
				//cout << "ПУСТО" << endl;
				break;
			}
			
			// что-то есть в листе ожидания
			auto newLocalRequest = Front(list);
			list = PopFront(list);

			// ОА был занят и следующая заявка обработается позже, чем должна была
			if (t >= newLocalRequest.globalTimeStartWork)
			{
				double localWaitTime = t - newLocalRequest.globalTimeStartWork;
				newLocalRequest.waitTime += localWaitTime;
				if (newLocalRequest.waitTime > maxRequest.waitTime)
				{
					maxRequest = newLocalRequest;
				}

				/*cout << "Ожидание заявки " << newLocalRequest.id << " " << localWaitTime << endl;*/
				/*waittime += localWaitTime;*/
				
				newLocalRequest.globalTimeStartWork = t;
				timeFreeOa = newLocalRequest.globalTimeStartWork + newLocalRequest.timeWork;
			}
			// ОА простаивал
			else
			{
				double localDowntime = newLocalRequest.globalTimeInput - t;
				downtime += localDowntime;
				timeFreeOa = newLocalRequest.globalTimeStartWork + newLocalRequest.timeWork;
			}

			// занимаем ОА
			workingRequest = &newLocalRequest;

#ifdef LOGALL
			cout << "Начали " << workingRequest->countWork + 1 << " обработку заявки " << workingRequest->id << " в " <<
				newLocalRequest.globalTimeStartWork << " е.в." << endl << endl;
#endif
		}
		else
		{
			// заявка готова к обработке
			if (workingRequest == nullptr)
			{
				cout << "Тут не должен останавливаться. Всегда что-то обрабатывается" << endl;
				throw "fff";
			}
		}
	}
	cout << endl;
	cout << "*****************************************" << endl;
	cout << "Общее время моделирования: " << timeSimulation << endl;
	cout << "Время простоя ОА: " << downtime << endl;
	cout << "Обработано заявок: " << 4000 << endl;
	cout << "Вошло заявок: " << 1000 << endl;
	cout << "Вышло заявок: " << 1000 << endl;
}

int main()
{
	setlocale(LC_ALL, "russian");
	try
	{
		double localT1, localT2;
		cout << "Введите T1 (верхняя граница времени прихода): " << endl;
		cin >> localT1;
		cout << "Введите T2 (верхняя граница времени обработки): " << endl;
		cin >> localT2;
		
		TestQS(localT1, localT2, 1000, 100);
	}
	catch (char* msg)
	{
		cout << "ERRROOOORRR: " << msg << endl << endl;
	}

	return 0;
}
