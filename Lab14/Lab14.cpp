#include <iostream>
#include <Windows.h>
#include <cstdio>

int array[10];
int length = sizeof(array) / sizeof(int);

DWORD WINAPI func(LPVOID param) 
{
    int id = (int)param;
    int sum = 0;

    srand(GetTickCount() + id * 100);


    for (int i = 0; i < 10; i++)
    {
        if (id == 1) {
            int x = rand() % (150 - (-150) + 1) + (-150);
            array[i] = x;
        }
        if (id == 2) {
            if ((length - 1) != i) {
                std::cout << "поток: " << id << " на позиции " << i << " = " << array[i] << std::endl;
                sum += array[i];
            }
            else {
                std::cout << "поток: " << id << " на позиции " << i << " = " << sum << " (сумма всех эл.) " << std::endl;
            }
        }


        Sleep(1000);
    }

 

    return 0;
}

int main()
{
    HANDLE threads[2];

    for (int i = 0; i < 2; i++) 
    {
        threads[i] = CreateThread(NULL, 0, func, (LPVOID)(uintptr_t)(i + 1), 0, NULL);
        if (threads[i] == NULL) {
            std::cerr << "Ошибка создания потока " << i + 1 << "\n";
            return GetLastError();
        }
    }

    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    for (int i = 0; i < 2; i++) {
        CloseHandle(threads[i]);
    }

    return 0;
}
