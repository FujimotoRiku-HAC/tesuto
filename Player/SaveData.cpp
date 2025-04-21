#include "SaveData.h"

void SaveData::SaveAbility(const char* file_path, std::array<int, AbilityNum> abilities){
	std::ofstream filepath;
	filepath.open(file_path);

	for (int i = 0; i < static_cast<int>(abilities.size()); i++) {
		filepath << abilities.at(i) << std::endl;
	}

	filepath.close();
}

void SaveData::SaveItem(const char* file_path, std::array<int, ItemNum> items){
	std::ofstream filepath;
	filepath.open(file_path);

	for (int i = 0; i < static_cast<int>(items.size()); i++) {
		filepath << items.at(i) << std::endl;
	}

	filepath.close();
}
