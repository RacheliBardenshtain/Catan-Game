#pragma once
#include <ostream>
#include <vector>
#include "ReadJson.h"
#include "Square.h"
#include "Infrastructure.h"
using namespace std;

class World
{
private:
	ReadJson r;
protected:
	vector<vector<Square>> worldMap;
	int rows;
	int columns;

	int countVillages = 0;
	int countRoads = 0;
	int countCities = 0;

public:
	string GetCategoryByNumber(int num);
	string GetValueByIndex(int row, int column);
	void setWorldMap(vector<vector<Square>> map);
	void SetAmountOfPeopleInInfrastructureOrInSquare(int index1, int index2, int numOfPeople);
	void SetResource(int index1, int index2, int count, string nameOfResource = "");
	void Work(int index1, int index2);
	int GetAmountOfResource(int index1, int index2, string nameResources);
	int GetAmountOfPeopleByIndex(int index1, int index2);
	void SetInfrastructure(string itemToBuild, int index1, int index2, pair<int, int> size, string stage);
	int CountInfrastructure(string infrastructure);
	bool ValidToBuild(string itemToBuild, int index1, int index2, pair<int, int> sizeOfInfrastructure);
	int GetAmountOfPeople(int index1, int index2);
	void MakeEmptyInfrastructure(int index1, int index2);
	void SetManufactureName(int index1, int index2, string name);
	bool IsExistManufacture(int index1, int index2, string manufacture);
	bool IsExistSomeManufacture(int index1, int index2);
	string GetNameOfResource(int index1, int index2);
	void UpdateResorcesItsRaining(int amount);
	void MovePersonToInfrastructure(int fromIndex1, int fromIndex2, int toIndex1, int toIndex2);
	void MoveManufactureToInfrastructure(int fromIndex1, int fromIndex2, int toIndex1, int toIndex2);
	void TakeResourcesToManufacture(int fromIndex1, int fromIndex2, int toIndex1, int toIndex2);
};

