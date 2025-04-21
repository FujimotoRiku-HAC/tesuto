#ifndef GOLEM_H_
#define GOLEM_H_

#include "Actor/Enemy/Enemy.h"
#include "State/StateMachine.h"

//�G�N���X
class Golem : public Enemy {
public:

	//�R���X�g���N�^
	Golem(IWorld* world, const GSvector3& position, const GSvector3& rotation);
	//�X�V
	virtual void Update(float delta_time)override;
	//�`��
	virtual void Draw() const override;
	//GUI�`��
	virtual void DrawGui() const override;
	//�Փ˃��A�N�V����
	virtual void React(Actor& other)override;
	//���b�Z�[�W����
	virtual void HandleMessage(const std::string& message, void* param)override;

public:

	//�C�x���g�ǉ�
	void SetAnimationEvent();
	//�X�e�[�g�ύX
	void ChangeState(int state, bool restate = false);
	//���ړ�
	void ThinkMove();
	//�U���̎��
	void RandamAttack();
	//�X�^�������Z�b�g
	void ResetStanValue();

private:

	//�X�e�[�g�ǉ�
	void AddState()override;

private:
	
	StateMachine<Golem> m_StateMachine;	//�X�e�[�g�}�V��
	int m_HitCount{ 0 };				//�q�b�g������
};

#endif