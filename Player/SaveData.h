#ifndef SAVE_DATA_H_
#define SAVE_DATA_H_

//�K�E�Z�̐�
const int AbilityNum{ 2 };
//�A�C�e���̐�
const int ItemNum{ 3 };

class SaveData {
public:
	//�K�E�Z�Z�[�u
	void SaveAbility(const char* file_path, std::array<int, AbilityNum>abilities);
	//�A�C�e���Z�[�u
	void SaveItem(const char* file_path, std::array<int, ItemNum>items);
};

#endif