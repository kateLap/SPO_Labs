#include <stdio.h>
#include <string>
#include <conio.h>
#include <windows.h>
#include <iostream>

using namespace std;

const string tea = "Tea";
const string meat = "Meat";
const string pasta = "Pasta";
const string soop = "Soop";

int main(int argc, char** argv)
{

	SYSTEMTIME lt;

	string product(argv[1]);
	int money = stoi(argv[2]);
	cout << "Your product's: " << product << endl;
	cout << "Your money:" << money << endl;
	_getch();

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