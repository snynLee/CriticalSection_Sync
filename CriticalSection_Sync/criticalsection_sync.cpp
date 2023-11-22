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

// 숫자 입력 및 출력
void Input()
{
	bool ValidInput = false;
	
	do {
		try {
			for (int i = 0; i < NUMBER; i++)
			{
				cout << i + 1 << "번째 숫자를 입력하세요 : ";
				cin >> numbers[i];

				// 음수 입력
				if (numbers[i] < 0)
					throw string("음수 불가!");
				// 문자 입력
				if (!cin)
				{
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					throw string("문자 불가!");
				}

			}

			ValidInput = true;
		}
		catch (string error) {
			cout << error << endl;
		}
	} while (!ValidInput);

	cout << "=========================" << endl;
	cout << "입력한 숫자 = { ";

	for (int j = 0; j < NUMBER; j++)
		cout << numbers[j] << " ";

	cout << "}" << endl;
}

// 합계
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

	// 스레드 생성

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

	// 스레드 대기
	WaitForMultipleObjects(NUM_OF_THREAD, hThread, TRUE, INFINITE);

	// 스레드 종료
	for (DWORD i = 0; i < NUM_OF_THREAD; i++)
	{
		CloseHandle(hThread[i]);
	}

	DeleteCriticalSection(&hCriticalSection);

	cout << "합계 = " << sum << endl;

	avg = static_cast<float>(sum) / NUMBER;

	cout << "평균 = " << avg << endl;

	return 0;
}