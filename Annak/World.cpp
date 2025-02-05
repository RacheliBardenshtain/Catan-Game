#include "World.h"
#include <string>
#include <vector>
#include <unordered_map>

void World::setWorldMap(vector<vector<Square>> map) {
	ReadJson r;
	int tile_size_x = r.config["Sizes"]["Tile"][0].get<int>();
	int tile_size_y = r.config["Sizes"]["Tile"][1].get<int>();

	rows = map.size() * tile_size_x;
	columns = map[0].size() * tile_size_y;

	worldMap.resize(rows, vector<Square>(columns));

	int row = -1, column = -1;

	for (int i = 0; i < rows; ++i) {
		if (i % tile_size_x == 0)
			row++;
		column = -1;
		for (int j = 0; j < columns; ++j) {
			if (j % tile_size_y == 0)
				column++;
			worldMap[i][j] = map[row][column];
		}
	}

}

string World::GetValueByIndex(int row, int column) {
	return (worldMap[column - 1][row - 1]).nameCategory;
}

string World::GetNameOfResource(int index1, int index2) {
	return worldMap[index2 - 1][index1 - 1].nameResource;
}

void World::SetResource(int index1, int index2, int count, string nameOfResource) {
	if (GetValueByIndex(index1, index2) == "City" || GetValueByIndex(index1, index2) == "Village") {
		int index = r.getResourceTypes()[nameOfResource];
		int limitResource = r.config["Capacities"][GetValueByIndex(index1, index2)][index];
		if (limitResource >= count)
			worldMap[index2 - 1][index1 - 1].infrastructure->resourcesArr[index] = count;
		else
			worldMap[index2 - 1][index1 - 1].infrastructure->resourcesArr[index] = limitResource;
	}
	else if (worldMap[index2 - 1][index1 - 1].amountOfPeople != 0) {
		int index = r.getResourceTypes()[nameOfResource];
		worldMap[index2 - 1][index1 - 1].person.resourcesArr[index] = count;
	}
	else if (worldMap[index2 - 1][index1 - 1].nameCategory == "Ground" && worldMap[index2 - 1][index1 - 1].manufacture != nullptr) {
		int index = r.getResourceTypes()[nameOfResource];
		if (r.config["Capacities"][worldMap[index2 - 1][index1 - 1].manufacture->nameOfManufacture][index] >= count)
			worldMap[index2 - 1][index1 - 1].manufacture->resourcesArr[index] = count;
	}
	else
		*(worldMap[index2 - 1][index1 - 1].countResource) = count;
}

string World::GetCategoryByNumber(int num) {
	if (num == -1) {
		return "Invalid index";
	}

	string category = "";
	ReadJson r;
	auto tiles = r.getTiles();
	for (auto it = tiles.begin(); it != tiles.end(); ++it) {
		if (it->second == num) {
			category += it->first;
			break;
		}
	}
	return category;
}

void World::SetAmountOfPeopleInInfrastructureOrInSquare(int index1, int index2, int numOfPeople) {
	if (worldMap[index2 - 1][index1 - 1].infrastructure != nullptr) {
		worldMap[index2 - 1][index1 - 1].infrastructure->people.clear();
		int limitPeople = r.config["Capacities"][GetValueByIndex(index1, index2)][4];
		if (limitPeople >= numOfPeople)
		{
			worldMap[index2 - 1][index1 - 1].infrastructure->numOfPeople = numOfPeople;
			for (int i = 0; i < numOfPeople; i++)
				worldMap[index2 - 1][index1 - 1].infrastructure->people.push_back(Person());
		}
		else
		{
			worldMap[index2 - 1][index1 - 1].infrastructure->numOfPeople = limitPeople;
			for (int i = 0; i < limitPeople; i++)
				worldMap[index2 - 1][index1 - 1].infrastructure->people.push_back(Person());
		}
	}
	else
	{
		worldMap[index2 - 1][index1 - 1].amountOfPeople = 1;
		worldMap[index2 - 1][index1 - 1].person = Person();
	}
}

void World::Work(int index1, int index2) {
	*(worldMap[index2 - 1][index1 - 1].countResource) = 0;
}

int World::GetAmountOfResource(int index1, int index2, string nameResources) {
	if (worldMap[index2 - 1][index1 - 1].nameCategory == "City" || worldMap[index2 - 1][index1 - 1].nameCategory == "Village")
		return worldMap[index2 - 1][index1 - 1].infrastructure->resourcesArr[r.getResourceTypes()[nameResources]];
	return *(worldMap[index2 - 1][index1 - 1].countResource);
}

int World::GetAmountOfPeopleByIndex(int index1, int index2) {
	return worldMap[index2 - 1][index1 - 1].amountOfPeople;
}

bool World::ValidToBuild(string itemToBuild, int index1, int index2, pair<int, int> sizeOfInfrastructure) {
	int count = 0;
	// left
	int length = index2 - 1 + sizeOfInfrastructure.first;
	for (int i = index2 - 1; i < length && index2 - 1 + length < rows && index1 - 2 >= 0; i++)
	{
		if (worldMap[i][index1 - 2].infrastructure != nullptr) {
			if (itemToBuild == "Road")
				count++;
			else
				if (worldMap[i][index1 - 2].infrastructure->nameOfInfrastructure == "Road")
					count++;
		}
		else
			count = 0;
		if (count == r.config["Sizes"]["Road"][0])
			return true;
	}

	//right
	count = 0;
	length = index2 - 1 + sizeOfInfrastructure.first;
	int column = index1 - 1 + sizeOfInfrastructure.second;
	for (int i = index2 - 1; i < length && index2 - 1 + length < rows && column < columns; i++)
	{
		if (worldMap[i][index1 - 2].infrastructure != nullptr) {
			if (itemToBuild == "Road")
				count++;
			else
				if (worldMap[i][index1 - 2].infrastructure->nameOfInfrastructure == "Road")
					count++;
		}
		else
			count = 0;
		if (count == r.config["Sizes"]["Road"][0])
			return true;
	}

	//up
	count = 0;
	length = index1 - 1 + sizeOfInfrastructure.first;
	for (int i = index1 - 1; i < length && index1 - 1 + length < rows; i++)
	{
		if (worldMap[i][index1 - 2].infrastructure != nullptr) {
			if (itemToBuild == "Road")
				count++;
			else
				if (worldMap[i][index1 - 2].infrastructure->nameOfInfrastructure == "Road")
					count++;
		}
		else
			count = 0;
		if (count == r.config["Sizes"]["Road"][1])
			return true;
	}

	//down
	count = 0;
	length = index1 - 1 + sizeOfInfrastructure.first;
	int row = index2 - 1 + sizeOfInfrastructure.first;
	for (int i = index1 - 1; i < length && index1 - 1 + length < rows && row < rows; i++)
	{
		if (worldMap[i][index1 - 2].infrastructure != nullptr) {
			if (itemToBuild == "Road")
				count++;
			else
				if (worldMap[i][index1 - 2].infrastructure->nameOfInfrastructure == "Road")
					count++;
		}
		else
			count = 0;
		if (count == r.config["Sizes"]["Road"][1])
			return true;
	}
	return false;
}

void World::SetInfrastructure(string itemToBuild, int index1, int index2, pair<int, int> size, string stage) {
	if (worldMap[index2 - 1][index1 - 1].nameCategory != "Ground")
		return;

	if (stage == "Input") {
		pair<int, int> sizeOfInfrastructure;
		sizeOfInfrastructure.first = r.config["Sizes"][itemToBuild][0];
		sizeOfInfrastructure.second = r.config["Sizes"][itemToBuild][1];
		if (!ValidToBuild(itemToBuild, index1, index2, sizeOfInfrastructure))
			return;
	}

	int countOfPeople = 0;
	if (itemToBuild != "Road")
		countOfPeople = r.config["StartingResources"][itemToBuild];

	shared_ptr<Infrastructure> infrastructure(new Infrastructure(itemToBuild, countOfPeople));

	for (int i = index2 - 1; i < index2 - 1 + size.first; i++)
	{
		for (int j = index1 - 1; j < index1 - 1 + size.second; j++)
		{
			worldMap[i][j].infrastructure = infrastructure;
			worldMap[i][j].nameCategory = itemToBuild;
		}
	}

	if (itemToBuild == "Road")
		countRoads++;
	else
		if (itemToBuild == "Village")
			countVillages++;
		else
			countCities++;
}

int World::CountInfrastructure(string infrastructure) {
	if (infrastructure == "Road")
		return countRoads;
	else
		if (infrastructure == "Village")
			return countVillages;
		else
			return countCities;
}

int World::GetAmountOfPeople(int index1, int index2) {
	if (worldMap[index2 - 1][index1 - 1].infrastructure != nullptr)
		return worldMap[index2 - 1][index1 - 1].infrastructure->numOfPeople;
	else
		return worldMap[index2 - 1][index1 - 1].amountOfPeople;
}

void World::MakeEmptyInfrastructure(int index1, int index2) {
	worldMap[index2 - 1][index1 - 1].amountOfPeople = 0;
	worldMap[index2 - 1][index1 - 1].infrastructure->numOfPeople = 0;
	worldMap[index2 - 1][index1 - 1].manufacture = nullptr;
	worldMap[index2 - 1][index1 - 1].infrastructure->resourcesArr.assign(4, 0);
}

void World::SetManufactureName(int index1, int index2, string name) {
	shared_ptr<Manufacture> manufacture(new Manufacture(name));

	int length = r.config["Sizes"][name][0];
	int width = r.config["Sizes"][name][1];

	for (int i = index2 - 1; i < index2 - 1 + length; i++)
	{
		for (int j = index1 - 1; j < index1 - 1 + width; j++)
		{
			worldMap[i][j].manufacture = manufacture;
		}
	}

}

bool World::IsExistManufacture(int index1, int index2, string manufacture) {
	if (worldMap[index2 - 1][index1 - 1].manufacture != nullptr
		&& (*(worldMap[index2 - 1][index1 - 1].manufacture)).nameOfManufacture == manufacture)
		return true;
	return false;
}

bool World::IsExistSomeManufacture(int index1, int index2) {
	if (worldMap[index2 - 1][index1 - 1].manufacture != nullptr)
		return true;
	return false;
}

void World::UpdateResorcesItsRaining(int amount) {
	int sizeRowOfTail = r.config["Sizes"]["Tile"][0];
	int sizeColumnOfTail = r.config["Sizes"]["Tile"][1];

	int amountOfWoodPerRain = r.config["Rains"]["Wood"];
	int amountOfWoolPerRain = r.config["Rains"]["Wool"];

	for (int i = 0; i < rows; i += sizeRowOfTail)
	{
		for (int j = 0; j < columns; j += sizeColumnOfTail)
		{
			if (worldMap[i][j].nameCategory == "Forest")
				*(worldMap[i][j].countResource) = amount / amountOfWoodPerRain;
			else if (worldMap[i][j].nameCategory == "Field")
				*(worldMap[i][j].countResource) = amount / amountOfWoolPerRain;
		}
	}
}

void World::MovePersonToInfrastructure(int fromIndex1, int fromIndex2, int toIndex1, int toIndex2) {
	if (GetValueByIndex(toIndex1, toIndex2) == "City" || GetValueByIndex(toIndex1, toIndex2) == "Village") {
		int limitPeople = r.config["Capacities"][GetValueByIndex(toIndex1, toIndex2)][4];
		if (limitPeople > worldMap[toIndex2 - 1][toIndex1 - 1].infrastructure->numOfPeople)
		{
			worldMap[toIndex2 - 1][toIndex1 - 1].infrastructure->people.push_back(worldMap[fromIndex2 - 1][fromIndex1 - 1].person);
			worldMap[toIndex2 - 1][toIndex1 - 1].infrastructure->numOfPeople += 1;
			return;
		}
	}
	else {
		worldMap[toIndex2 - 1][toIndex1 - 1].person = worldMap[fromIndex2 - 1][fromIndex1 - 1].person;
		worldMap[toIndex2 - 1][toIndex1 - 1].amountOfPeople = 1;
	}
}

void World::MoveManufactureToInfrastructure(int fromIndex1, int fromIndex2, int toIndex1, int toIndex2) {
	if (GetValueByIndex(toIndex1, toIndex2) == "City" || GetValueByIndex(toIndex1, toIndex2) == "Village") {
		vector<int> resources = worldMap[fromIndex2 - 1][fromIndex1 - 1].manufacture->resourcesArr;
		vector<int> capacity = r.config["Capacities"][GetValueByIndex(toIndex1, toIndex2)];
		for (int i = 0; i < resources.size(); i++)
		{
			int amount = worldMap[toIndex2 - 1][toIndex1 - 1].infrastructure->resourcesArr[i];
			if (resources[i] + amount <= capacity[i])
				worldMap[toIndex2 - 1][toIndex1 - 1].infrastructure->resourcesArr[i] = resources[i];
		}
	}
}

void World::TakeResourcesToManufacture(int fromIndex1, int fromIndex2, int toIndex1, int toIndex2) {
	vector<int> resources = worldMap[fromIndex2 - 1][fromIndex1 - 1].infrastructure->resourcesArr;
	string manufacture = worldMap[toIndex2 - 1][toIndex1 - 1].manufacture->nameOfManufacture;
	vector<int> capacity = r.config["Capacities"][manufacture];
	for (int i = 0; i < resources.size(); i++)
	{
		int amount = worldMap[toIndex2 - 1][toIndex1 - 1].manufacture->resourcesArr[i];
		if (amount + resources[i] <= capacity[i]) {
			worldMap[toIndex2 - 1][toIndex1 - 1].manufacture->resourcesArr[i] = resources[i];
			worldMap[fromIndex2 - 1][fromIndex1 - 1].infrastructure->resourcesArr[i] = 0;
		}
		else {
			int amountToTake = capacity[i] - amount;
			worldMap[toIndex2 - 1][toIndex1 - 1].manufacture->resourcesArr[i] = capacity[i];
			worldMap[fromIndex2 - 1][fromIndex1 - 1].infrastructure->resourcesArr[i] -= amountToTake;
		}
	}
}
