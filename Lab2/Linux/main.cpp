#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ncurses.h>
#include <iostream>
#include <cstring>

using namespace std;

#define PROCESS_LIMIT 6

char stringSet [PROCESS_LIMIT][15]= {
        {"FIRST_PERSON\n"}, {"SECOND_PERSON\n"}, {"THIRD_PERSON\n"},
        {"FOURTH_PERSON\n"}, {"FIFTH_PERSON\n"}, {"SIXTH_PERSON\n"}
};

int processInit();
void newProcess();
void deleteProcess();
void nextProcess();
void deleteProcesses();//завершение программы

int printFlag = 0;
int flagEnd = 1;


void canPrint(int sign) //установка флага для работы процесса
{
    printFlag = 1;
}

void setEndFlag(int sign) //установка флага для завершения процесса
{
    flagEnd = 1;
}

pid_t childPid[PROCESS_LIMIT]; //массив идентификаторов процессов
int currentProcessIndex = 0; //индекс текущего дочернего процесса
int processNumber = 0; //номер дочернего процесса
int flag = 0;

struct sigaction printSignal, endSignal;

int main()
{
    initscr();
    curs_set(0);
    clear();
    noecho();
    refresh();

    processInit();

    clear();
    endwin();
    return 0;
}

int processInit()
{
    printSignal.sa_handler = canPrint; //установка функции-обработчика
    sigaction(SIGUSR1, &printSignal, NULL);

    endSignal.sa_handler = setEndFlag;
    sigaction(SIGUSR2, &endSignal, NULL);

    char c = 0;

    while (c != 'q')
    {
        c = getchar();
        if (c == '+') newProcess();
        if (c == '-') deleteProcess();

        nextProcess();
    }
    deleteProcesses();
    refresh();
}

void newProcess()
{
    if (processNumber < PROCESS_LIMIT)
    {
        childPid[processNumber] = fork();
        processNumber++;

        {
            if (childPid[processNumber - 1] == 0)   //переопределение для дочернего процесса
            {
                char sizeStr[15];
                sprintf(sizeStr, "%d", processNumber);
                if (execlp("./Parent", sizeStr, NULL) == -1)
                    cout << "Error.";
            }
        }
    }
}

void deleteProcess()
{
    if (processNumber > 0) //если дочерние процессы существуют
    {
        kill(childPid[processNumber - 1], SIGUSR2); //сигнал о завершении последнего дочернего процесса
        waitpid(childPid[processNumber - 1], NULL, NULL);

        processNumber--;

        if (currentProcessIndex >= processNumber) //если текущий работающий элемент - последний
        {
            currentProcessIndex = 0;
            flag = 1;
            flagEnd = 1;//установка флага (разрешение работы следующего процесса)
        }
    }
}

void nextProcess()
{
    if (flagEnd && processNumber > 0)
    {
        flagEnd = 0;    //начало существования следующего дочернего процесса
        if (currentProcessIndex >= processNumber - 1) //если текущий элемент был последним, то переходим на первый
            currentProcessIndex = 0;
        else if (!flag) currentProcessIndex++; //если не установлен флаг перехода на первый элемент
                                               //при удалении последнего процесса
        flag = 0;
        kill(childPid[currentProcessIndex], SIGUSR1); //сигнал о начале работы следующего процесса
    }
}

void deleteProcesses()
{
    if (childPid[processNumber - 1] != 0)   //для родительского процесса
        while (processNumber >= 0)
        {
            kill(childPid[processNumber - 1], SIGUSR2);
            waitpid(childPid[processNumber - 1], NULL, NULL);
            processNumber--;
        }
}