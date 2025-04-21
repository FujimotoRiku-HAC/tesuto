#ifndef ITEM_KINDS_H_
#define ITEM_KINDS_H_

class ItemKinds {
public:

	//�A�C�e���g�p
	void UseItem(IWorld* world, int item);
	//�X�V
	void Update(IWorld* world, float delta_time);

private:

	//�񕜃|�[�V�����̍X�V
	void HealPortionUpdate(IWorld* world);
	//�U���̓A�b�v�|�[�V�����̍X�V
	void AttackPortionUpdate(IWorld* world, float delta_time);

private:

	bool m_IsUseAttackPortion = false;	//�U���|�[�V�����g�p������
	bool m_IsUseHealPortion = false;	//�q�[���|�[�V�������g�p������
	float m_AttackPortionTimer = 0.0f;	//�|�[�V�����^�C�}�[

};

#endif