#include <iostream>
#include <Windows.h>
#include <process.h>
#include <numeric> 

using namespace std;

#define NUMBER 12
#define NUM_OF_THREAD 3

CRITICAL_SECTION	hCriticalSection;

int numbers[NUMBER];

int sum = 0;
float avg = 0;

// ���� �Է� �� ���
void Input()
{
	bool ValidInput = false;
	
	do {
		try {
			for (int i = 0; i < NUMBER; i++)
			{
				cout << i + 1 << "��° ���ڸ� �Է��ϼ��� : ";
				cin >> numbers[i];

				// ���� �Է�
				if (numbers[i] < 0)
					throw string("���� �Ұ�!");
				// ���� �Է�
				if (!cin)
				{
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					throw string("���� �Ұ�!");
				}

			}

			ValidInput = true;
		}
		catch (string error) {
			cout << error << endl;
		}
	} while (!ValidInput);

	cout << "=========================" << endl;
	cout << "�Է��� ���� = { ";

	for (int j = 0; j < NUMBER; j++)
		cout << numbers[j] << " ";

	cout << "}" << endl;
}

// �հ�
DWORD WINAPI ThreadSumProc(PVOID lpParam)
{
	DWORD* nPtr = (DWORD*)lpParam;

	DWORD head = *nPtr;
	DWORD tail = *(nPtr + 1);

	EnterCriticalSection(&hCriticalSection);

	for (DWORD i = head; i <= tail; i++)
		sum += numbers[i];

	LeaveCriticalSection(&hCriticalSection);

	return 0;
}

int main()
{
	HANDLE hThread[NUM_OF_THREAD];
	DWORD hThreadID[NUM_OF_THREAD];

	DWORD paramThread[] = { 0,3,4,7,8,11 };

	InitializeCriticalSection(&hCriticalSection);

	Input();

	// ������ ����

	for (DWORD i = 0; i < NUM_OF_THREAD; i++)
	{
		hThread[i] = 
			CreateThread(
				NULL, 0,
				ThreadSumProc,
				(LPVOID)(&paramThread[i * 2]),
				0, &hThreadID[i]
			);

		if (hThread[i] == NULL)
		{
			cout << "Thread creation fault!" << endl;
			return -1;
		}
	}

	// ������ ���
	WaitForMultipleObjects(NUM_OF_THREAD, hThread, TRUE, INFINITE);

	// ������ ����
	for (DWORD i = 0; i < NUM_OF_THREAD; i++)
	{
		CloseHandle(hThread[i]);
	}

	DeleteCriticalSection(&hCriticalSection);

	cout << "�հ� = " << sum << endl;

	avg = static_cast<float>(sum) / NUMBER;

	cout << "��� = " << avg << endl;

	return 0;
}