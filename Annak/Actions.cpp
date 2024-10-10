#include "Actions.h"

int Actions::GetMatNumOfRows() const {
	vector< shared_ptr<Command>> start = in.start;
	int length = 0;
	for (int i = 0; i < in.start.size(); i++)
	{
		try {
			int temp = stoi(start[i]->name);
			length++;
		}
		catch (const invalid_argument& e) {
			break;
		}
	}
	return length;
}

int Actions::GetCountSetNameResourceForResource(Square& square) {
	if (square.nameCategory == "Forest")
	{
		square.nameResource = "Wood";
		return r.config["StartingResources"]["Forest"];
	}
	else
	{
		if (square.nameCategory == "Field")
		{
			square.nameResource = "Wool";
			return r.config["StartingResources"]["Field"];
		}
		else
		{
			if (square.nameCategory == "IronMine")
			{
				square.nameResource = "Iron";
				return r.config["StartingResources"]["IronMine"];
			}
			else
				if (square.nameCategory == "BlocksMine")
				{
					square.nameResource = "Blocks";
					return r.config["StartingResources"]["BlocksMine"];
				}
				else {
					return -1;
				}
		}
	}
}

void Actions::BuildWorld() {

	vector< shared_ptr<Command>> start = in.start;
	int length = GetMatNumOfRows();

	vector<vector<Square>> mat(length, vector<Square>(start[0]->arguments.size() + 1));

	for (int i = 0; i < length; i++)
	{
		mat[i][0].nameCategory = worldGame.GetCategoryByNumber(stoi(start[i]->name));
		int size = r.config["ResourceTypes"].size() - 1;

		shared_ptr<int> countResources = make_shared<int>(GetCountSetNameResourceForResource(mat[i][0]));

		if (*countResources != -1)
			mat[i][0].countResource = countResources;

		for (int j = 0; j < (start[i]->arguments.size()); j++)
		{
			mat[i][j + 1].nameCategory = worldGame.GetCategoryByNumber(stoi(start[i]->arguments[j]));

			int size = r.config["ResourceTypes"].size() - 1;
			shared_ptr<int> Countresources2 = make_shared<int>(GetCountSetNameResourceForResource(mat[i][j + 1]));
			if (*Countresources2 != -1)
				mat[i][j + 1].countResource = Countresources2;
		}
	}
	worldGame.setWorldMap(mat);
}

void Actions::BuildStart() {
	vector< shared_ptr<Command>> start = in.start;
	string itemToBuild;
	int amount;
	int index1;
	int index2;

	for (int i = GetMatNumOfRows(); i < start.size(); i++)
	{
		if (start[i]->name == "Resource")
		{
			itemToBuild = start[i]->arguments[1];
			amount = stoi(start[i]->arguments[0]);
			index1 = stoi(start[i]->arguments[2]);
			index2 = stoi(start[i]->arguments[3]);

			worldGame.SetResource(index1, index2, amount, itemToBuild);
		}
		else
			if (start[i]->name == "People")
			{
				int amount = stoi(start[i]->arguments[0]);
				int index1 = stoi(start[i]->arguments[1]);
				int index2 = stoi(start[i]->arguments[2]);

				worldGame.SetAmountOfPeopleInInfrastructureOrInSquare(index1, index2, amount);
			}
			else
				if (start[i]->name == "Build") {
					string itemToBuild = start[i]->arguments[0];
					int index1 = stoi(start[i]->arguments[1]);
					int index2 = stoi(start[i]->arguments[2]);

					pair<int, int> size = r.config["Sizes"][itemToBuild];
					worldGame.SetInfrastructure(itemToBuild, index1, index2, size, "Start");
				}
				else if (start[i]->name == "MakeEmpty") {
					int index1 = stoi(start[i]->arguments[0]);
					int index2 = stoi(start[i]->arguments[1]);
					if (worldGame.GetValueByIndex(index1, index2) == "City" || worldGame.GetValueByIndex(index1, index2) == "Village") {
						worldGame.MakeEmptyInfrastructure(index1, index2);
					}
					else
						worldGame.SetResource(index1, index2, 0);
				}
				else if (start[i]->name == "Manufacture") {
					string nameOfManufacture = start[i]->arguments[0];
					int index1 = stoi(start[i]->arguments[1]);
					int index2 = stoi(start[i]->arguments[2]);

					worldGame.SetManufactureName(index1, index2, nameOfManufacture);
				}
	}
}

pair<int, int> Actions::DoInput() {
	vector< shared_ptr<Command>> steps = in.steps;
	int num1 = 0, num2 = 0, temp = 0;
	bool valueOfSelect = false, rain = false;

	for (int i = 0; i < steps.size(); i++)
	{
		if (steps[i]->name == "Select") {
			num1 = stoi((steps)[i]->arguments[0]);
			num2 = stoi((steps)[i]->arguments[1]);
		}
		else if ((steps[i]->name == "Work")) {
			if (worldGame.GetAmountOfPeopleByIndex(num1, num2) > 0) {
				int index1 = stoi((steps)[i]->arguments[0]);
				int index2 = stoi((steps)[i]->arguments[1]);
				worldGame.Work(index1, index2);
			}
		}
		else if (steps[i]->name == "Wait") {
			for (int j = 0; j < stoi(steps[i]->arguments[0]); j++);
		}
		else if (steps[i]->name == "Rain") {
			worldGame.UpdateResorcesItsRaining(stoi((steps)[i]->arguments[0]));
		}
		else if (steps[i]->name == "Build") {
			string itemToBuild = steps[i]->arguments[0];
			int index1 = stoi(steps[i]->arguments[1]);
			int index2 = stoi(steps[i]->arguments[2]);

			pair<int, int> size = r.config["Sizes"][itemToBuild];
			worldGame.SetInfrastructure(itemToBuild, index1, index2, size, "Input");
		}
		else if (steps[i]->name == "People")
		{
			int amount = stoi(steps[i]->arguments[0]);
			num1 = stoi(steps[i]->arguments[1]);
			num2 = stoi(steps[i]->arguments[2]);

			worldGame.SetAmountOfPeopleInInfrastructureOrInSquare(num1, num2, amount);
		}
		else if (steps[i]->name == "Deposit") {
			int index1 = stoi(steps[i]->arguments[0]);
			int index2 = stoi(steps[i]->arguments[1]);
			if (worldGame.GetValueByIndex(num1, num2) == "City" ||
				worldGame.GetValueByIndex(num1, num2) == "Village") {
			}
			else if (worldGame.GetValueByIndex(num1, num2) == "Ground" && worldGame.GetAmountOfPeople(num1, num2) > 0) {
				if (worldGame.IsExistSomeManufacture(num1, num2))
					worldGame.MoveManufactureToInfrastructure(num1, num2, index1, index2);
				else
					worldGame.MovePersonToInfrastructure(num1, num2, index1, index2);
			}

		}
		else if (steps[i]->name == "TakeResources") {
			int index1 = stoi(steps[i]->arguments[0]);
			int index2 = stoi(steps[i]->arguments[1]);
			worldGame.TakeResourcesToManufacture(index1, index2, num1, num2);
		}
	}
	return { num1, num2 };
}

void Actions::DoAction() {
	BuildWorld();
	BuildStart();
	pair<int, int> pair = DoInput();

	int num1 = pair.first, num2 = pair.second;

	vector< shared_ptr<Command>> start = in.start;
	vector<string> asserts = in.asserts;

	for (int i = 0; i < asserts.size(); i++)
	{
		string command = asserts[i];
		if (command == "SelectedCategory")
			cout << "SelectedCategory " << worldGame.GetValueByIndex(num1, num2) << endl;
		else
			if (command == "SelectedResource")
			{
				vector<int> resourcesArr(4, 0);
				int index = r.getResourceTypes()[worldGame.GetNameOfResource(num1, num2)];
				resourcesArr[index] = worldGame.GetAmountOfResource(num1, num2, worldGame.GetNameOfResource(num1, num2));

				cout << "SelectedResource ";
				for (int i = 0; i < resourcesArr.size(); i++)
				{
					cout << resourcesArr[i] << " ";
				}
				cout << endl;
			}
			else if (command == "CityCount")
				cout << "CityCount " << worldGame.CountInfrastructure("City") << endl;
			else if (command == "RoadCount")
				cout << "RoadCount " << worldGame.CountInfrastructure("Road") << endl;
			else if (command == "VillageCount")
				cout << "VillageCount " << worldGame.CountInfrastructure("Village") << endl;
			else if (command == "SelectedComplete") {
				cout << "SelectedComplete false";
			}
			else if (command == "SelectedPeople") {
				cout << "SelectedPeople " << worldGame.GetAmountOfPeople(num1, num2) << endl;
			}
			else if (command == "SelectedCar")
				cout << "SelectedCar " << worldGame.IsExistManufacture(num1, num2, "Car");
			else if (command == "SelectedTruck") {
				cout << "SelectedTruck " << worldGame.IsExistManufacture(num1, num2, "Truck");
			}

	}
}
