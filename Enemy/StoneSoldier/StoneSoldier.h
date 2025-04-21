#ifndef STONE_SOLDIER_H_
#define STONE_SOLDIER_H_

#include "Actor/Enemy/Enemy.h"
#include "Animation/AnimatedMesh.h"
#include "StoneSoldierMotionType.h" 
#include "State/StateMachine.h"

//�G�N���X
class StoneSoldier : public Enemy {

public:

	//�R���X�g���N�^
	StoneSoldier(IWorld* world, const GSvector3& position,const GSvector3& rotation);
	//�X�V
	virtual void Update(float delta_time)override;
	//�`��
	virtual void Draw() const override;
	//GUI�`��
	virtual void DrawGui() const override;
	//�Փ˃��A�N�V����
	virtual void React(Actor& other)override;
	//���b�Z�[�W
	virtual void HandleMessage(const std::string& message, void* param)override;

public:

	//�X�e�[�g�ύX
	void ChangeState(int state, bool restate = false);
	//�C���^�[�o���O
	void ThinkMove();
	//�����_���U��
	void RandomAttack();

private:

	//�X�e�[�g�ǉ�
	void AddState()override;
	//�A�j���[�V�����C�x���g�ǉ�
	void SetAnimationEvent();
	//�U�����󂯂���̃��[�V�����ύX
	void DecentHitMotion(Actor& other);

private:
	bool m_IsStrongHit{ false };				//���U�����󂯂���
	bool m_IsBlowHit{ false };					//�u���E�U�����󂯂���
	bool m_IsPush{ false };						//�v�b�V���U�����󂯂���
	bool m_IsGetUp{ false };					//�N���オ�邩

	StateMachine<StoneSoldier> m_StateMachine;	//�X�e�[�g�}�V��
};

#endif