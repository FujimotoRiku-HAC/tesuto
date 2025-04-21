#ifndef SKILL_DATA_H_
#define SKILL_DATA_H_

#include "PlayerSkill.h"

class SkillData {
public:

	//�R���X�g���N�^
	SkillData();
	//�`��
	void Draw()const;
	//���[�h
	void SkillLoad(const char* file_path);
	//�ێ����Ă���X�L���̎g�p
	void Excute(Player* player);

private:

	//�X�L���̐ݒ�
	void SetSkill(int num, int skill);
	//�A�C�R���Z�b�g
	void SetIcon();
	//�A�C�R�����[�h
	void IconTexture();

private:
	//�X�L��
	PlayerSkill m_PlayerSkill;
	//�X�L����
	std::array<int, 3>m_Skills;
	//�X�L���̃A�C�R��
	std::unordered_map<int, GSuint>m_SkillsIcon;
	//�Z���N�g��
	int m_SelectCount;

};

#endif