#include <iostream>
#include <windows.h>
#include <vector>
#include <process.h>
using namespace std;

CRITICAL_SECTION cs;
int k;
int A;
int i_;
long long result = 1;

HANDLE Event_1 = CreateEvent(NULL, FALSE, FALSE, NULL);
HANDLE Event_2 = CreateEvent(NULL, FALSE, FALSE, NULL);

DWORD WINAPI Thread2Function(LPVOID lpParam) {
	vector<double>& a = *static_cast<vector<double>*>(lpParam);

	cout << " interval : ";
	cin >> i_;

	cout << endl;

	vector<double> smaller;
	vector<double> greater;

	for (double num : a) {
		if (num < A) {
			smaller.push_back(num);
		}
		else {
			greater.push_back(num);
		}
	}

	a.clear();

	a.insert(a.end(), greater.begin(), greater.end());
	a.insert(a.end(), smaller.begin(), smaller.end());


	cout << " a : ";
	for (int i = 0; i < a.size(); i++) {
		cout << a[i] << " ";
		Sleep(i_ * 1000);
	}

	cout << endl;

	SetEvent(Event_1);
	return 0;
}

DWORD WINAPI Thread3Function(LPVOID lpParam) {
	vector<double>& a = *static_cast<vector<double>*>(lpParam);

	EnterCriticalSection(&cs);

	WaitForSingleObject(Event_2, INFINITE);

	for (size_t i = k; i < a.size(); ++i) {
		result *= a[i];
	}

	LeaveCriticalSection(&cs);

	return 0;
}

int main() {
	A = 0;
	k = 0;
	std::vector<double> a;

	setlocale(LC_ALL, "RUSSIAN");

	cout << " Введите размер массива: ";
	int n;
	cin >> n;

	cout << " Введите элементы : ";
	for (int i = 0; i < n; ++i) {
		double element;
		cin >> element;
		a.push_back(element);
	}

	cout << " Размер массива : " << n;

	cout << endl;

	cout << " Массив : ";
	for (int i = 0; i < n; ++i) {
		cout << a[i] << " ";
	}

	cout << endl;

	InitializeCriticalSection(&cs);

	HANDLE hThread2 = CreateThread(NULL, 0, Thread2Function, &a, CREATE_SUSPENDED, NULL);

	cout << " Введите значение A : ";
	cin >> A;
	cout << " Введите значение k : ";
	cin >> k;
	cout << endl;

	ResumeThread(hThread2);

	HANDLE hThread3 = CreateThread(NULL, 0, Thread3Function, &a, 0 , NULL);


	WaitForSingleObject(Event_1, INFINITE);

	cout << " Массив итог : ";
	for (int i = 0; i < n; ++i) {
		cout << a[i] << " ";
	}

	cout << endl;

	SetEvent(Event_2);


	EnterCriticalSection(&cs);

	cout << "Итоговое произведение: " << result << std::endl;

	LeaveCriticalSection(&cs);

	WaitForSingleObject(hThread3, INFINITE);

	CloseHandle(Event_1);
	CloseHandle(Event_2);
	CloseHandle(hThread2);
	CloseHandle(hThread3);
	DeleteCriticalSection(&cs);

	return 0;
}

