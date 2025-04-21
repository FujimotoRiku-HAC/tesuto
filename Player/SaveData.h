#ifndef SAVE_DATA_H_
#define SAVE_DATA_H_

//必殺技の数
const int AbilityNum{ 2 };
//アイテムの数
const int ItemNum{ 3 };

class SaveData {
public:
	//必殺技セーブ
	void SaveAbility(const char* file_path, std::array<int, AbilityNum>abilities);
	//アイテムセーブ
	void SaveItem(const char* file_path, std::array<int, ItemNum>items);
};

#endif