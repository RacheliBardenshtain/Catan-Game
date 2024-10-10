#pragma once
#include <vector>

using namespace std;

class Person
{
public:
	vector<int> resourcesArr;

	Person() :resourcesArr(4, 0) {
	}
};

