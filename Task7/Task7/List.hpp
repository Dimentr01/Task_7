#pragma once
#include <iostream>

template <typename T1>
struct List
{
	T1 value;
	List* next;
	List* prev;
};


template<typename T1>
void InsertInstead(List<T1>* value, const T1& item)
{
	auto prevValue = value->prev;
		
	List<T1>* newItem = new List<T1>;
	newItem->value = item;
	newItem->next = nullptr;
	newItem->prev = nullptr;

	prevValue->next = newItem;
	newItem->prev = prevValue;

	newItem->next = value;
	value->prev = newItem;
}

// Add item to the begin
template <typename T1>
List<T1>* PushFront(List<T1>* head, const T1& item)
{
	List<T1>* newItem = new List<T1>;
	newItem->value = item;
	newItem->next = nullptr;
	newItem->prev = nullptr;
	if (head == nullptr)
	{
		return newItem;
	}
	newItem->next = head;
	head->prev = newItem;
	head = newItem;
	return head;
}

// Add item to the end
template <typename T1>
List<T1>* PushBack(List<T1>* head, const T1& item)
{
	List<T1>* newItem = new List<T1>;
	newItem->value = item;
	newItem->next = nullptr;
	newItem->prev = nullptr;
	if (head == nullptr)
	{
		return newItem;
	}
	
	List<T1>* tmp = head;
	while (tmp != nullptr && tmp->next != nullptr)
		tmp = tmp->next;
	
	tmp->next = newItem;
	newItem->prev = tmp;
	
	return head;
}

// Access to the first element
template <typename T1>
T1 Front(List<T1>* head)
{
	return head->value;
}

// Access to the last element
template <typename T1>
T1 Back(List<T1>* head)
{
	if (head->next == nullptr)
		return head->value;
	
	while (head != nullptr && head->next != nullptr)
		head = head->next;
	
	return head->value;
}

// Remove the first element
template <typename T1>
List<T1>* PopFront(List<T1>* head)
{
	if (head == nullptr)
		return nullptr;
	
	if (head->next == nullptr)
	{
		delete head;
		return nullptr;
	}

	List<T1>* temp = head->next;
	temp->prev = nullptr;
	//head = head->next;
	delete head;
	return temp;
}

// Check for elements
template <typename T1>
bool IsEmpty(List<T1>* head)
{
	if (head == nullptr)
		return true;
	return false;
}



template<typename T1>
void PrintList(List<T1>* head)
{
	std::cout << "-----------------------------" << std::endl;
	while(head!=nullptr)
	{
		std::cout << head->value << " ";
		head = head->next;
	}
	std::cout << std::endl << "-----------------------------" << std::endl;
}