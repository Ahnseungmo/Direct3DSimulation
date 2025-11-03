#include "Framework.h"

DataManager::DataManager()
{
}

DataManager::~DataManager()
{
}

void DataManager::LoadData(const string& fileName)
{
	ifstream file(fileName);
	if (!file.is_open())
	{
		MessageBox(nullptr, L"Failed to open file", L"Error", MB_OK);
		return;
	}

	string line;

	bool isFirstLine = true;

	while (getline(file, line))
	{
		if (isFirstLine)
		{
			isFirstLine = false;
			continue;
		}

		vector<string> data = Utility::SplitString(line, ",");

		ItemData itemData;
		itemData.key = stoi(data[0]);
		itemData.name = data[1];
		itemData.explane = data[2];
		itemData.price = stoi(data[3]);
		itemData.value = stoi(data[4]);
		itemData.type = (ItemData::Type)stoi(data[5]);

		itemDatas[itemData.key] = itemData;
	}
	file.close();
}
