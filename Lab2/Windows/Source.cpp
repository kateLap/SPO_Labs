#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

#include <string>
#include <iostream>

using namespace std;

#define PROCESS_LIMIT 6
PROCESS_INFORMATION CreateDefaultProcess(string appName, string cmdArg);//функци€ создани€ дочернего процесса

PROCESS_INFORMATION processes[PROCESS_LIMIT];

char stringSet[PROCESS_LIMIT][15] = {
	{ "FIRST_person" },{ "SECOND_person" },{ "THIRD_person" },
	{ "FOURTH_person" },{ "FIFTH_person" },{ "SIXTH_person" }
};

//управл€ющий процесс
int main(int argc, char** argv)
{
		char ch;
		int processCount = 0;
		HANDLE hEvent = CreateEvent(NULL, FALSE, TRUE, "CanWriteEvent"); //автосбрасывание, сигнальное состо€ние
		HANDLE hCloseEvents[PROCESS_LIMIT];

	AGAIN:

		do
		{
			rewind(stdin);
			ch = _getch();
		} while (ch != '+' && ch != '-' && ch != 'q');

		switch (ch)
		{
		case '+':
		{
			char buffer[20];
			sprintf(buffer, " %s %d", stringSet[processCount], processCount);		//создание индивидуальной строки дл€ событи€ закрыти€
			hCloseEvents[processCount] = CreateEvent(NULL, FALSE, FALSE, buffer);	//автосбрасывание, несигнальное состо€ние
			string appName("Spovm.exe");
			string cmd(buffer);
			processes[processCount] = CreateDefaultProcess(appName, cmd);
			processCount++;
			goto AGAIN;
		}
		case '-':
		{
			WaitForSingleObject(hEvent, INFINITE);	//пока допишетс€ последн€€ уникальна€ строка
			SetEvent(hCloseEvents[--processCount]);	//событие дл€ закрыти€ дочернего процесса
			SetEvent(hEvent);						//следующий процесс может выполн€ть свою работу
			goto AGAIN;
		}
		default:
		{
			ResetEvent(hEvent); //работа процессов окончена
			for (int i = 0; i < processCount; i++)
			{
				TerminateProcess(processes[i].hProcess, EXIT_FAILURE); //завершение процесса(дескриптор дочернего процесса, код выхода)
			}
			cout << endl << "Exit" << endl;

			_getch();
			ExitProcess(0);
		}
		}
}

PROCESS_INFORMATION CreateDefaultProcess(string appName, string cmdArg)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	string cmdLine = appName + " " + cmdArg;

	if (!CreateProcess(
		NULL,
		const_cast<char*>(cmdLine.c_str()),
		NULL, 
		NULL,
		TRUE, 
		NULL,//CREATE_NEW_CONSOLE
		NULL, 
		NULL,
		&si, 
		&pi
	))
	{
		cout << "Create process failed" << endl;
	}
	return pi;
}