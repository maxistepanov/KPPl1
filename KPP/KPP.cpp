#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <thread>
#include <mutex>
#include <windows.h>
#include <condition_variable>
#include <atomic>

using namespace std;

mutex synchronize_mutex;
condition_variable cv;
int id, t1, t2;
atomic <int> value;

void Mutex();
void MutexWork(int num);

void Semaphore();
void SemaphoreWork(int num, int max);

void Atomic();
void AtomicPlus();
void AtomicMinus();

int main()
{
	setlocale(LC_ALL, "rus");

	int mode;
	srand(time(0));

	do
	{
		id = 0;

		system("cls");
		cout << "�������� ����� ����:\n";
		cout << "1: �������\n";
		cout << "2: �������\n";
		cout << "3: ��������� ��������\n";
		cout << "4: �����\n";

		cin >> mode;

		switch (mode)
		{
		case 1: Mutex(); break;
		case 2: Semaphore(); break;
		case 3: Atomic(); break;
		}
	} while (mode < 4);

	return 0;
}


////MUTEX////
void Mutex()
{
	system("cls");
	int count;

	cout << "������� ���������� ��������: ";
	cin >> count;

	cout << "������� ��������� ������� t1 � t2: ";
	cin >> t1 >> t2;

	thread **opers = new thread*[count];
	for (int i = 0; i < count; i++)
		opers[i] = new thread(MutexWork, i);

	for (int i = 0; i < count; i++)
	{
		opers[i]->join();
		delete opers[i];
	}
	delete opers;

	system("pause");
}

void MutexWork(int num)
{
	int t = t1 + rand() % t2;
	
	//cout << "�������� " << num << " �������\n";
	synchronize_mutex.lock();
	cout << "������� " << num << " ����������� ������������\n";
	Sleep(t * 1000);
	synchronize_mutex.unlock();
}

////SEMAPHORE////
void Semaphore()
{
	system("cls");

	int count = 8;

	cout << "������� ��������� ������� t1 � t2: ";
	cin >> t1 >> t2;

	thread **opers = new thread*[count];
	for (int i = 0; i < count; i++)
		opers[i] = new thread(SemaphoreWork, i + 1, count);

	for (int i = 0; i < count; i++)
	{
		opers[i]->join();
		delete opers[i];
	}
	delete opers;

	system("pause");
}

void SemaphoreWork(int num, int max)
{
	int t = t1 + rand() % t2;
	unique_lock<std::mutex> lck(synchronize_mutex);
	while (num != max - id) { cv.wait(lck); }

	id++;
	cout << "������� " << num << "/" << max << " ����������� ������������\n";
	Sleep(t * 1000);
	cv.notify_all();
}

////ATOMIC////
void Atomic()
{
	system("cls");
	value = 100;


	cout << "���� ������� ����� � ������ ������� � ��������� 500 �����������, � ����� ���������� 300 �����������\n" << endl;
	thread oper1(AtomicPlus);
	thread oper2(AtomicMinus);

	oper1.join();
	oper2.join();

	cout << "���������: " << value << endl;
	system("pause");
}

void AtomicPlus()
{
	for (int i = 0; i < 500; i++)
	{
		value++;
		if (i>1&&i % 50==0)
		{
			cout << "��������� \t" << value << " �����������\n";
		}
	}

}

void AtomicMinus()
{
	for (int i = 0; i < 300; i++)
	{
		value--;
		if (i>1&&i % 50==0)
		{
			cout << "���������� \t" << value << " �����������\n";
		}
	}
}
