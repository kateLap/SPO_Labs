#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int productNumber = 4;

class Product
{
	string productName;
	int productPrice;
public:
	Product(){}
	~Product(){}
	Product(string name, int price)
	{
		productName = name;
		productPrice = price;
	}
	int GetPrice()
	{
		return productPrice;
	}

	friend ostream & operator << (ostream & out, Product & product)
	{
		return out << product.productName << " " << product.productPrice << endl;
	}
	friend istream & operator >> (istream & in, Product & product)
	{
		in >> product.productName;
		in >> product.productPrice;
		return in;
	}
};

class Restaurant
{
	Product products[productNumber];
public:
	Restaurant()
	{
	}
	~Restaurant() {}
	void PrintMenu()
	{
		cout << "MENU:" << endl;
		for (int i = 0; i < productNumber; i++)
		{
			cout << i+1 << ".";
			cout << products[i];
		}
		cout << "---------------" << endl;
	}
	void MenuRead(string fileName)
	{
		ifstream filestream;
		filestream.open(fileName);
		
		for (int i = 0; i < productNumber; i++)
		{
			filestream >> products[i];
		}

		filestream.close();
	}
	int ProductPrice(int i)
	{
		return products[i].GetPrice();
	}
};
