#pragma once

struct ItemData
{
	enum Type
	{
		Weapon = 1, Armor, Potion
	};

	int key;
	string name;
	string explane;
	int price;
	int value;
	Type type;
};

class DataManager : public Singleton<DataManager>
{
	friend class Singleton;

private:
	DataManager();
	~DataManager();

public:
	void LoadData(const string& fileName);
	
	ItemData GetItem(int key) { return itemDatas[key]; }
	int GetItemCount() { return itemDatas.size(); }

private:
	unordered_map<int, ItemData> itemDatas;

};