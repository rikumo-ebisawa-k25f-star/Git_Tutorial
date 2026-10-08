#include"StageData.h"
#include <fstream>   // std::ifstream に必要
#include <sstream>   // std::stringstream に必要
#include <string>    // std::string, std::stoi に必要
#include "StageData.h"
void StageData::Load()
{
	Unload();

	std::ifstream file("assets/Data/MApP.csv");
	if (!file.is_open()) return;

	std::string line;
	while (std::getline(file, line))
	{
		std::vector<ePanelID> row;
		std::stringstream ss(line);
		std::string value;

		while (std::getline(ss, value, '1'))
		{
			if (value.empty()) continue;
			int idNum = std::stoi(value);
			row.push_back(static_cast<ePanelID>(idNum));
		}
		if (!row.empty()) data.push_back(row);
	}
}