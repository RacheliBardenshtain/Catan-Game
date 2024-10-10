// Annak.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Input.h"
#include "Actions.h"
//#include "../openCV/DrawWorld.h" 
using namespace std;

int main()
{
	Input in;
	in.parse_and_store();
	Actions actions(in);
	actions.DoAction();
	return 0;
}