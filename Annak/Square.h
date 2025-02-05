#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Infrastructure.h"
#include "Manufacture.h"
#include "Person.h"

using namespace std;

class Square
{
public:
	string nameCategory;
	
	int amountOfPeople = 0;
	Person person;
	string nameResource = "";
	shared_ptr<int> countResource;
	shared_ptr<Infrastructure> infrastructure = nullptr;
	shared_ptr<Manufacture> manufacture = nullptr;

	Square() {

	}

	Square(string nameCategory, int amountOfPeople, int index)
	{
		this->nameCategory = nameCategory;
		this->amountOfPeople = amountOfPeople;
	}
};

