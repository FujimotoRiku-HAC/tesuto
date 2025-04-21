#ifndef ITEM_DATA_H_
#define ITEM_DATA_H_

#include "ItemKinds.h"

class ItemData {
public:

	//�R���X�g���N�^
	ItemData();
	//�X�V
	void Update(IWorld* world,float delta_time);
	//�`��
	void Draw()const;

private:

	//�A�C�e�����c���Ă��邩
	bool RemainingItem(int type)const;
	//�w��̃A�C�e���������Ă��邩
	bool HavingItem()const;

private:

	//�A�C�e���g�p
	void UseItem(IWorld* world);
	//�g�����A�C�e�������炷
	void DecrementItemNum(int type);
	//�A�C�R���Z�b�g
	void SetIcon();
	//�A�C�e���̌��ݒ�
	void SetHavingItemNum();
	//�A�C�R�����[�h
	void IconTexture();
	//�U���{�^������������
	void AttackDraw(float delta_time);
	//����{�^������������
	void AvoidDraw(float delta_time);
	//�A�C�e���{�^������������
	void Item1Draw(float delta_time);
	void Item2Draw(float delta_time);
	//�A�C�R���̍X�V
	void IconUpdate(float delta_time);

private:

	//���A�C�e����
	std::unordered_map<int, int>m_HavingItemNum;
	//�A�C�e�����
	ItemKinds m_ItemKinds;
	//�A�C�e���A�C�R��
	std::unordered_map<int, GSuint>m_ItemIcon;

	//�{�^������������
	bool m_IsAttack{ false };
	bool m_IsAvoid{ false };
	bool m_IsItem1{ false };
	bool m_IsItem2{ false };

	//�A�C�R���̃A���t�@�l
	float m_AttackAlpha{ 1.0f };
	float m_AvoidAlpha{ 1.0f };
	float m_Item1Alpha{ 1.0f };
	float m_Item2Alpha{ 1.0f };
	//���݂̃A�C�e���ԍ�
	int m_CurrentItem = 0;
};

#endif