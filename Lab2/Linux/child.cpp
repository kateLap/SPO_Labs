#include <sys/types.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sys/wait.h>

using namespace std;

const string tea = "Tea";
const string meat = "Meat";
const string pasta = "Pasta";
const string soop = "Soop";

struct sigaction printSignal, endSignal;

int printFlag = 0;
int flagEnd = 0;

void canPrint(int signo)
{
    printFlag = 1;
}

void setEndFlag(int signo)
{
    flagEnd = 1;
}


int main(int argc, char** argv)
{
    printSignal.sa_handler = canPrint;
    sigaction(SIGUSR1, &printSignal, NULL);

    endSignal.sa_handler = setEndFlag;
    sigaction(SIGUSR2, &endSignal, NULL);

    string product(argv[0]);
    char c;
    int money = atoi(argv[1]);
    int result;
    initscr();

    while(1) {
        if(flagEnd) break;
        if(printFlag) {
            cout << "\r\n";
            cout << "Product:" << argv[0] << "\r\n";
            cout << "Money:" << money << "\r\n";
            break;
        }
    }

    usleep(1000000);

    if (product == tea)
    {
        if (money >= 5)
            return 1;
        return 0;
    }
    if (product == meat)
    {
        if (money >= 10)
            return 2;
        return 0;
    }
    if (product == pasta)
    {
        if (money > 4)
            return 3;
        return 0;
    }
    if (product == soop)
    {
        if (money >= 8)
            return 4;
        return 0;
    }
    return 5;
}