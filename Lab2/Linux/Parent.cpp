#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <signal.h>
#include <sys/sem.h>

using namespace std;

#define PROCESS_LIMIT 6

using namespace std;

struct Person{
    string product;
    string money;
};

Person persons[PROCESS_LIMIT]={
        {"Tea", "80"}, {"Meat", "7"}, {"Soop", "7"},
        {"Pasta", "12"}, {"Coffee", "10"}, {"Meat", "10"}
};

struct sigaction printSignal, endSignal;

int printFlag = 0;
int flagEnd = 1;

void resultOutput(int);

void canPrint(int signo)
{
    printFlag = 1;
}

void setEndFlag(int signo)
{
    flagEnd = 1;
}

int main(int argc, char* argv[])
{
    int status;
    initscr();

    printSignal.sa_handler = canPrint;
    sigaction(SIGUSR1, &printSignal, NULL);

    endSignal.sa_handler = setEndFlag;
    sigaction(SIGUSR2, &endSignal, NULL);

    int currentProcess = atoi(argv[0]);

    string strProduct = persons[currentProcess-1].product;
    string strMoney = persons[currentProcess-1].money;

    char putString[256];

    char stringSet [PROCESS_LIMIT][20]= {
            {"FIRST_person\n"}, {"SECOND_person\n"}, {"THIRD_person\n"},
            {"FOURTH_person\n"}, {"FIFTH_person\n"}, {"SIXTH_person\n"}
    };

    switch(currentProcess) {
        case 1:
            sprintf(putString, "%s ", stringSet[0]);
            break;
        case 2:
            sprintf(putString, "%s ", stringSet[1]);
            break;
        case 3:
            sprintf(putString, "%s ", stringSet[2]);
            break;
        case 4:
            sprintf(putString, "%s ", stringSet[3]);
            break;
        case 5:
            sprintf(putString, "%s ", stringSet[4]);
            break;
        case 6:
            sprintf(putString, "%s ", stringSet[3]);
            break;
    }

        if (atoi(argv[0]) == 1) kill(getppid(), SIGUSR2); //если это первый дочерний процесс,
                                                          //то в управляющий процесс flagEnd = 1

    if(currentProcess == 1)
    {
        cout<<"MENU:\r\n-----\r\n";

        cout << "1.Tea 5$\r\n";
        cout << "2.Meat 10$\r\n";
        cout << "3.Pasta 4$\r\n";
        cout << "4.Soop 8$\r\n\r\n";
        sleep(2);
    }

        flagEnd = 0;

        while (!flagEnd)
        {
            usleep(100000);

            if (printFlag)
            {
                pid_t pid = fork();

                if (pid == -1) cout << "Error.";

                if (pid == 0)  //переопределение дочернего процесса
                {
                    if (execlp("./child", strProduct.c_str(), strMoney.c_str(), NULL) == -1)
                        cout << "Error.";
                }
                else {
                    usleep(100000);
                    if (printFlag) {
                        for (int i = 0; i < strlen(putString); i++) {
                            if (flagEnd) {
                                printFlag = 0;
                                kill(getppid(), SIGUSR2);  //уведомление родителя о завершении работы дочернего процесса
                                kill(pid, SIGUSR2);
                                waitpid(pid, &status, 0);
                                return 0;
                            }
                            printw("%c", putString[i]);
                            refresh();
                            usleep(150000);
                        }
                        refresh();
                        printFlag = 0;  //завершение работы данного процесса

                        kill(pid, SIGUSR1); //уведомление дочернего процесса о возможности вывода строки
                        waitpid(pid, &status, 0);
                        resultOutput(status);

                        kill(getppid(), SIGUSR2);
                    }
                }
            }
        }
        return 0;
}


void resultOutput(int status)
{
    if (WIFEXITED(status)) {
        int result = (int) WEXITSTATUS(status);

        switch (result) {
            case 0:
                cout << "Not enough money!" << "\r\n" <<"\r\n" ;
                break;
            case 1:
                cout << "Tea for you!" << "\r\n"<<"\r\n";
                break;
            case 2:
                cout << "Meat for you!" << "\r\n"<<"\r\n";
                break;
            case 3:
                cout << "Pasta for you!" << "\r\n"<<"\r\n";
                break;
            case 4:
                cout << "Soop for you!" << "\r\n"<<"\r\n";
                break;
            case 5:
                cout << "Product doesn't exist!" << "\r\n"<<"\r\n";
                break;
        }
        usleep(100000);
    }
}

