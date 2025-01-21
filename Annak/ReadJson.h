#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;


class ReadJson
{
public:
	json config;

	ReadJson() {
	   ifstream f("configuration.json");
	   if (!f.is_open()) {
             throw runtime_error("Failed to open configuration file");
           }
           config = json::parse(f, nullptr, false);
           if (config.is_discarded()) {
            throw runtime_error("Failed to parse configuration file");
           }
	}

	unordered_map<string, int> getTiles() const {
		unordered_map<string, int> tiles;
		if (config.contains("Tiles")) {
			for (const auto& item : config["Tiles"].items()) {
				tiles[item.key()] = item.value().get<int>();
			}
		}
		return tiles;
	}

	unordered_map<string, int> getResourceTypes() const {
		unordered_map<string, int> resourceToIndex;
		vector<string> resourcesTypes = config["ResourceTypes"];
		for (size_t i = 0; i < resourcesTypes.size(); i++)
		{
			resourceToIndex[resourcesTypes[i]] = i;
		}
		return resourceToIndex;
	}
};

