#include <iostream>
#include <Windows.h>
#include <cstdio>

int array[10];
int length = sizeof(array) / sizeof(int);
CRITICAL_SECTION cs;

DWORD WINAPI func_add(LPVOID param) 
{
    EnterCriticalSection(&cs);

    srand(GetTickCount());
    int id = (int)param;


    for (int i = 0; i < 10; i++)
    {
        int x = rand() % (150 - (-150) + 1) + (-150);
        array[i] = x;
        std::cout << "поток: " << id << " на позиции " << i << " = " << array[i] << std::endl;
    }
    Sleep(1000);
    LeaveCriticalSection(&cs);
    return 0;
}

DWORD WINAPI func_check(LPVOID param)
{
    EnterCriticalSection(&cs);

    int id = (int)param;
    int sum = 0;

    for (int i = 0; i < 10; i++)
    {
        sum += array[i];
        if ((length - 1) == i) {
            array[i] = sum;
            std::cout << "поток: " << id << " на позиции " << i << " = " << sum << " (сумма всех эл.) " << std::endl;
        }
    }

    LeaveCriticalSection(&cs);
    return 0;
}

int main()
{
    setlocale(LC_ALL, "rus");
    HANDLE threads[2];
    InitializeCriticalSection(&cs);

    threads[0] = CreateThread(NULL, 0, func_add, (LPVOID)(uintptr_t)+1, 0, NULL);
    threads[1] = CreateThread(NULL, 0, func_check, (LPVOID)(uintptr_t)+2, 0, NULL);
    if (threads[0] == NULL || threads[1] == NULL) {
        std::cerr << "Ошибка создания потока " << "\n";
        return GetLastError();
    }

    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    for (int i = 0; i < 2; i++) {
        CloseHandle(threads[i]);
    }

    return 0;
}
