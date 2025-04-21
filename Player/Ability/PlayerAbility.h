#ifndef PLAYER_ABILITY_H_
#define PLAYER_ABILITY_H_

#include "AbilityType.h"
#include "AbilityAnimation.h"

class PlayerAbility {
public:

	//�R���X�g���N�^
	PlayerAbility();
	//�X�V
	void Update(IWorld* world, float delta_time);
	//�`��
	void Draw()const;
	//���[�h
	void AbilityLoad(const char* file_path);

private:

	//�K�E�Z���g������
	void UseCurrentAbility(IWorld* world, int num);
	//���̕K�E�Z��I��������
	void SetAbility(int num, int special);
	//�A�C�R���̐ݒ�
	void IconSet();
	//�A�r���e�B�g������
	void IconDraw(float delta_time);
	void IconDraw2(float delta_time);
	//���[�h
	void LoadTexture();

private:

	//�A�r���e�B�N���X
	AbilityAnimation m_AbilityAnim;
	//�A�r���e�B
	std::array<int, 2>m_Abilities;
	//�A�r���e�B�A�C�R��
	std::unordered_map<int, GSuint>m_AbilitiesIcon;

	//���݂̃A�r���e�B
	int m_CurrentAbility = 0;
	//���ꂼ��̃A���t�@�l
	float m_Alpha_1{ 1.0f };
	float m_Alpha_2{ 1.0f };
	//�ǂ̃X�L�����g�p������
	bool m_UseAbility1{ false };
	bool m_UseAbility2{ false };
};


#endif