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
	cout << "������: " << request.id << endl;
	cout << "����� �������: " << request.timeInput << endl;
	cout << "���������� ����� �������: " << request.globalTimeInput << endl;
	cout << "����� ���������: " << request.timeWork << endl << endl;
}

// ������� ���������� ������
void PreGenRequests(List<request>*& list, List<request>*& helperList, int countRequests, double timeInputEnd, double timeWorkEnd)
{
	int idRequest = 0;
	int globalTime = 0;
	for (int i = 0; i < countRequests; i++)
	{
		// ������� ������
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
	// ������ 
	List<request>* list = nullptr;

	// ������ ������� ����� ��� �������� ����������, ��� ��� ������� ������� ������
	// ��� ������������ �� ������ ������
	List<request>* helperList = nullptr;

	// ���������� ������
	PreGenRequests(list, helperList, countRequests, timeInputEnd, timeWorkEnd);

	// �����, ����� ����������� ��
	auto timeFreeOa = .0;

	// ���������� ����������� ������
	int countEnded = 0;

	// ����� �������
	auto downtime = .0;

	// ����� ���������
	auto timeSimulation = .0;

	// ������������ ������ ( ��� ����������)
	int requestsWorked = 0;

	// � ������������ �������� ��������
	request maxRequest;
	maxRequest.id = -1;
	maxRequest.countWork = -1;
	maxRequest.waitTime = -1;

	// ������������ ����� � �������
	auto maxQueueTime = -1.0;

	// ��� �������� ������� � �������
	auto SumForAverageQueueTime = .0;

	// ��� ����������� ������� �������� ������� � �������
	int countPrintStatistic = 1;

	// �������������� ������
	request* workingRequest = nullptr;
	while (true)
	{
		// ��� ������ � ������ (��������� ������ ������ ��� �� ����)
		if (!IsEmpty(list) && workingRequest == nullptr)
		{
			auto newLocalRequest = Front(list);
			list = PopFront(list);

			// ����� ������� �� ��������� 1 ������
			downtime += newLocalRequest.globalTimeInput;

			// �����, ����� ����������� ��
			timeFreeOa = newLocalRequest.globalTimeStartWork + newLocalRequest.timeWork;

			// �������� ��
			workingRequest = &newLocalRequest;
#ifdef LOGALL
			cout << "������ " << workingRequest->countWork + 1 << " ��������� ������ " << workingRequest->id << " � " <<
				newLocalRequest.globalTimeStartWork << " �.�." << endl << endl;
#endif

			continue;
		}

		int timeInputNextRequest;
		// ������ ������ � ������ ���������. ���� ���������� ������� � ���
		if (IsEmpty(list))
		{
			timeInputNextRequest = INT32_MAX;
		}
		else
		{
			timeInputNextRequest = Front(list).globalTimeInput;
		}

		// ��� �������� �������, ��������� ������� ��� ������ ���������
		//auto t = GetMinValue(timeFreeOa, timeInputNextRequest);
		
		auto t = timeFreeOa; // ������ ����� �� ������ ����� ���������
		
		// ��� ����� ��������, ���� �� ����������� ������ ��� ���������� ����� ������� ���� ������,
		// �� �������� ��� ������ ����� (���� �����, ���� ��� - �� ����� �����������).
		// � ���� ���� ��������� ������ �� ������, ��� ��� �������-�� ������� (t > globalTimeInput),
		// �� ��������� ����� ��������
		// ������������ ������ ����� ��������� � ������� � ������ �����
		if (t == timeFreeOa)
		{
			// ���� ������� ������ ������� countWork++
			// � ����� ��������� �� � �������, ����� ���� ��� ��������� �����������
			// ���� countWork == 4 ���� �������� �� �� ���������
			//
			// ����� �������� ������ � ������� ������� �������� �� �����

			workingRequest->countWork++;
			auto endedTime = workingRequest->globalTimeStartWork + workingRequest->timeWork;
#ifdef LOGALL
			cout << "��������� " << workingRequest->countWork << " ��������� ������ " << workingRequest->id << " � " <<
				endedTime << " �.�." << endl << endl;
#endif
			timeSimulation = endedTime; // �������� ����� ������ �� ��������� ���� ����� ����� ������� ���������� ��������� ��������� ������
			requestsWorked++;

			// ����� ������ 100
			if (requestsWorked == limitStatistic)
			{
				cout << "---------------------------------------------" << endl;
				cout << "������� ������ �������: " << endedTime << endl;

				// ������� ����� ��������� � ����� ������� ������ � �������
				// �.�. ������� ������ � �������� globalTimeInput <= t � �������������� ������?
				auto tmp = helperList;
				int totalInput = 0;
				while (tmp && tmp->next != nullptr && tmp->value.globalTimeInput <= t)
				{
					totalInput++;
					tmp = tmp->next;
				}
				cout << "����� ������: " << totalInput << endl;

				// ������� ������ ������ � �������
				// �.�. ������� ������ � �������� globalTimeInput <= t � ���������� ������?
				auto tmp1 = list;
				int remain = 0;
				while (tmp1 && tmp1->next != nullptr && tmp1->value.globalTimeInput <= t)
				{
					remain++;
					tmp1 = tmp1->next;
				}
				cout << "������� ����� �������: " << remain << endl;

				// ������ ������ � ������������ �������� ��������
				if (maxRequest.id == -1)
				{
					cout << "������ � ������������ �������� ��������: " << "�����������" << endl;
				}
				else
				{
					cout << "������ � ������������ �������� ��������: " << maxRequest.id << endl;
					cout << "�� ���� ������������: " << maxRequest.countWork << endl;
					// cout << "�� ����� ��������: " << maxRequest.waitTime << endl;
				}

				// ����� ������������ ����� � ������� �� globalTimeInput �� endedTime �� 4 �������� ��� ����� ������� ������
				cout << "������������ ����� ������ � �������: " << maxQueueTime << endl;

				// ����� ������� ����� � ������� �� globalTimeInput �� endedTime ��� ����� ������� ������
				cout << "������� ����� ������ � �������: " << float(SumForAverageQueueTime) / (limitStatistic *
					countPrintStatistic);

				cout << endl;
				requestsWorked = 0;
				countPrintStatistic++;
			}

			// ������ ���� �������� �� ���������
			if (workingRequest->countWork == 4)
			{
				countEnded++;

				auto timeQueue = (endedTime - workingRequest->globalTimeInput) - 4 * workingRequest->timeWork;
				if (timeQueue > maxQueueTime)
					maxQueueTime = timeQueue;

				SumForAverageQueueTime += timeQueue;
			}
			// ���� ������ ������ 4 ��� ���� � ������ ���������� �� ����� � �������
			else
			{
				// ��������� ����� ���������� ����� � ��. �� ������� ������� ��� ��������� ������
				workingRequest->globalTimeStartWork = endedTime;

				// ���� ���� ������, �� ������ ��������� � �����
				if (IsEmpty(list))
					list = PushBack(list, *workingRequest);
				else
				{
					// ����� ������ ���� �������� � "������ �����"
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
						//��� �� ����� ��������� �� nullptr, ��� ��� ��������� ����� ����, � ������ �� ������� ����������
						auto insertPlace = list;
						while (workingRequest->globalTimeStartWork > insertPlace->value.globalTimeInput)
						{
							insertPlace = insertPlace->next;
							if (insertPlace == nullptr)
								throw "��������� ���-�� ����������";
						}

						InsertInstead(insertPlace, *workingRequest);
					}
				}
			}

			workingRequest = nullptr;

			if (IsEmpty(list))
			{
				//throw "�����";
				//cout << "�����" << endl;
				break;
			}
			
			// ���-�� ���� � ����� ��������
			auto newLocalRequest = Front(list);
			list = PopFront(list);

			// �� ��� ����� � ��������� ������ ������������ �����, ��� ������ ����
			if (t >= newLocalRequest.globalTimeStartWork)
			{
				double localWaitTime = t - newLocalRequest.globalTimeStartWork;
				newLocalRequest.waitTime += localWaitTime;
				if (newLocalRequest.waitTime > maxRequest.waitTime)
				{
					maxRequest = newLocalRequest;
				}

				/*cout << "�������� ������ " << newLocalRequest.id << " " << localWaitTime << endl;*/
				/*waittime += localWaitTime;*/
				
				newLocalRequest.globalTimeStartWork = t;
				timeFreeOa = newLocalRequest.globalTimeStartWork + newLocalRequest.timeWork;
			}
			// �� ����������
			else
			{
				double localDowntime = newLocalRequest.globalTimeInput - t;
				downtime += localDowntime;
				timeFreeOa = newLocalRequest.globalTimeStartWork + newLocalRequest.timeWork;
			}

			// �������� ��
			workingRequest = &newLocalRequest;

#ifdef LOGALL
			cout << "������ " << workingRequest->countWork + 1 << " ��������� ������ " << workingRequest->id << " � " <<
				newLocalRequest.globalTimeStartWork << " �.�." << endl << endl;
#endif
		}
		else
		{
			// ������ ������ � ���������
			if (workingRequest == nullptr)
			{
				cout << "��� �� ������ ���������������. ������ ���-�� ��������������" << endl;
				throw "fff";
			}
		}
	}
	cout << endl;
	cout << "*****************************************" << endl;
	cout << "����� ����� �������������: " << timeSimulation << endl;
	cout << "����� ������� ��: " << downtime << endl;
	cout << "���������� ������: " << 4000 << endl;
	cout << "����� ������: " << 1000 << endl;
	cout << "����� ������: " << 1000 << endl;
}

int main()
{
	setlocale(LC_ALL, "russian");
	try
	{
		double localT1, localT2;
		cout << "������� T1 (������� ������� ������� �������): " << endl;
		cin >> localT1;
		cout << "������� T2 (������� ������� ������� ���������): " << endl;
		cin >> localT2;
		
		TestQS(localT1, localT2, 1000, 100);
	}
	catch (char* msg)
	{
		cout << "ERRROOOORRR: " << msg << endl << endl;
	}

	return 0;
}
