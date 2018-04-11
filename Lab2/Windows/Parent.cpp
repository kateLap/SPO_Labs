#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <string>
#include <iostream>
#include"Restaurant.h"

using namespace std;
#define PROCESS_LIMIT 6

void PrintUniqueString(char* string);

struct Person {
	string product;
	string money;
};

Person persons[PROCESS_LIMIT] = {
	{"Tea", "80"}, {"Meat", "7"}, { "Soop", "7" }, 
	{ "Pasta", "12" },{"Coffee", "10"}, {"Meat", "10"}
};

char stringSet[PROCESS_LIMIT][15] = {
	{ "FIRST_person" },{ "SECOND_person" },{ "THIRD_person" },
	{ "FOURTH_person" },{ "FIFTH_person" },{ "SIXTH_person" }
};

int main(int argc, char** argv)//argv[1]-индивидуальная строка, argv[2]-индекс процесса
{
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "CanWriteEvent");
	char eventName[20];
	sprintf(eventName, " %s %s", argv[1], argv[2]);
	HANDLE hCloseEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, eventName);

	while (TRUE)
	{
		WaitForSingleObject(hEvent, INFINITE);
		if (WaitForSingleObject(hCloseEvent, 100) == WAIT_OBJECT_0) //если событие о закрытии процесса
		{
			SetEvent(hEvent);
			exit(EXIT_SUCCESS);
		}

		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		ZeroMemory(&pi, sizeof(pi));
		si.cb = sizeof(si);

		if (atoi(argv[2]) == 0)
		{
			Restaurant restaurant;
			restaurant.MenuRead("Menu.txt");
			//restaurant.PrintMenu();
		}

		string moneyStr = persons[atoi(argv[2])].money;
		string productStr = persons[atoi(argv[2])].product;

		cout << "Person's money: " << moneyStr << "$" << endl;
		cout << "Person's choise: " << productStr << endl << "----------------" << endl;

		string comLine = "child.exe " + productStr + " " + moneyStr;

		if (!CreateProcess(
			NULL,
			const_cast<char*>(comLine.c_str()),
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&si,
			&pi)
			)
		{
			cout << "Create Process failed:" << GetLastError() << endl;
			_getch();
			return -1;
		}

		while (WaitForSingleObject(pi.hProcess, 50))
		{
			PrintUniqueString(stringSet[atoi(argv[2])]);
		}

		DWORD ret;
		GetExitCodeProcess(pi.hProcess, &ret);
		CloseHandle(pi.hProcess);

		switch (ret)
		{
		case 0:
			cout << "Not enough money!" << endl;
			break;
		case 1:
			cout << "Tea for you!" << endl;
			break;
		case 2:
			cout << "Meat for you!" << endl;
			break;
		case 3:
			cout << "Pasta for you!" << endl;
			break;
		case 4:
			cout << "Soop for you!" << endl;
			break;
		case 5:
			cout << "Product doesn't exist!" << endl;
			break;
		}

		SetEvent(hEvent); //установка события о возможности работы следующего процесса
	}

	return 0;
}

void PrintUniqueString(char* string)
{
	for (int i = 0; i < strlen(string); i++)
	{
		cout << string[i];
		Sleep(200);
	}
	cout << endl;
}