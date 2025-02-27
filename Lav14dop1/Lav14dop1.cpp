#include <iostream>
#include <Windows.h>
#include <cstdio>

long array[10] = {0};
int length = sizeof(array) / sizeof(int);
CRITICAL_SECTION cs;

DWORD WINAPI func_add(LPVOID param)
{
    srand(GetTickCount());
    int id = (int)param;


    for (int i = 0; i < 10; i++)
    {
        EnterCriticalSection(&cs);
        int x = rand() % (150 - (-150) + 1) + (-150);
        InterlockedExchangeAdd(&array[i], x);
        //std::cout << "поток: " << id << " на позиции " << i << " = " << array[i] << std::endl;
        LeaveCriticalSection(&cs);
    }
    return 0;
}

DWORD WINAPI func_check(LPVOID param)
{

    int id = (int)param;

    //std::cout << "отрицательные элменты заменены на 0" << std::endl;

    for (int i = 0; i < 10; i++)
    {
        EnterCriticalSection(&cs);
        if (array[i] < 0) {
            int zero = 0;
            InterlockedExchange(&array[i], zero);
        }
        //std::cout << "поток: " << id << " на позиции " << i << " = " << array[i] << std::endl;
        LeaveCriticalSection(&cs);
    }
    return 0;
}

int main()
{
    setlocale(LC_ALL, "rus");
    HANDLE threads[2];
    InitializeCriticalSection(&cs);

    std::cout << "начальный массив: ";

    for (int i = 0; i < 10; i++)
    {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;

    threads[0] = CreateThread(NULL, 0, func_add, (LPVOID)(uintptr_t)+1, 0, NULL);

    std::cout << "заполненный массив: ";

    for (int i = 0; i < 10; i++)
    {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;

    threads[1] = CreateThread(NULL, 0, func_check, (LPVOID)(uintptr_t)+2, 0, NULL);

    std::cout << "измненный массив: ";

    for (int i = 0; i < 10; i++)
    {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;

    if (threads[0] == NULL || threads[1] == NULL) {
        std::cerr << "Ошибка создания потока " << "\n";
        return GetLastError();
    }

    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    for (int i = 0; i < 2; i++) {
        CloseHandle(threads[i]);
        DeleteCriticalSection(&cs);
    }

    return 0;
}
