#pragma once
#include <iostream>
#include <string>
#include "Input.h"
#include "World.h"
#include "ReadJson.h"

using namespace std;

class Actions
{
private:
	ReadJson r;
	Input in;
	World worldGame;

	int GetMatNumOfRows() const;

public:
	Actions(Input in) {
		this->in = in;
	}

	void BuildWorld();

	void BuildStart();

	pair<int, int> DoInput();

	void DoAction();

	int GetCountSetNameResourceForResource(Square& square);


};

