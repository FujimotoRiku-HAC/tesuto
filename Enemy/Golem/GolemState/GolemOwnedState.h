#ifndef STONE_SOLDIER_OWNED_STATE_H_
#define STONE_SOLDIER_OWNED_STATE_H_

#include "State/State.h"
#include "Actor/Enemy/Golem/GolemMotionType.h"
#include "Actor/Enemy/Golem/Golem.h"

class GolemOwnedState {
public:
	enum {
		Idle,						//�A�C�h��
		RightThink,					//�E�ړ�
		LeftThink,					//���ړ�
		ForwardWalk,				//�O����
		BackWalk,					//������
		ChargeSpinAttack,			//���߉�]�a��
		JumpingAttack,				//�W�����v�U��
		WildDanceRotatingAttack,	//������]
		ChargeDanceAttack,			//���ߗ���
		PushAttack,					//�����o���U��
		TripleAttack,				//3�A�U��
		ForAttack,					//����
		Jump,						//�W�����v
		Generate,					//�o��
		StanStart,					//�X�^���X�^�[�g
		StanLoop,					//�X�^�����[�v
		StanEnd,					//�X�^���G���h
		Die							//���S

	};
};
//�^�[���A���O��
const float TurnAngle{ 2.5f };
//���ړ��X�s�[�h
const float ThinkSpeed{ 0.3f };
//���ɍs���X�s�[�h
const float BackSpeed{ 0.25f };
//�U���C���^�[�o��
const float AttackInterval{ 60.0f* 2 };
//���ړ����鋗��
const float AskDis{ 6.0f };
//�����鋗��
const float WalkDis{ 3.0f };
//�������U��
const float LongAttackDis{ 10.0f };
//�U������
const float AttackDis{ 6.0f };
//�~�܂鋗��
const float StopDis{ 1.5f };
//�߂鎞��
const float ReturnTimer{ 300.0f };
//�ʏ�U�����ɉ�鎞��
const float AttackTurnTime{ 10.0f };
//�W�����v�U�����ɉ�鎞��
const float JumpAttackTurnTime{ 110.0f };
//�������̉�鎞��
const float WildDanceAttackTurnTime{ 100.0f };


//�{�X�̊��N���X
class GolemStateBase : public State<Golem> {
public:
	GolemStateBase(Golem* owner, IWorld* world = nullptr) :
		State{ owner }, world_{ world } {}
	void Enter()override {};
	void Execute(float delta_time)override {};
	void Draw()const override {};
	void Exit() override {};
	void OnMessage(const std::string& message, void* param = nullptr)override {};
protected:
	IWorld* world_{ nullptr };
};

//---------------------------------------------�o�����----------------------------------------------------
class GolemGenerateState : public GolemStateBase {
public:
	GolemGenerateState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemGenerateState(const GolemGenerateState& other) = default;
	GolemGenerateState& operator = (const GolemGenerateState& other) = default;
};

//�A�C�h�����
class GolemIdleState : public GolemStateBase {
public:
	GolemIdleState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemIdleState(const GolemIdleState& other) = default;
	GolemIdleState& operator = (const GolemIdleState& other) = default;

private:
	bool IsLongDisAttack()const;
	bool IsAsk()const;
};

//-----------------------------------------------�v���C���[�܂ŕ���---------------------------------------------

//�O
class GolemWalkForwardState : public GolemStateBase {
public:
	GolemWalkForwardState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemWalkForwardState(const GolemWalkForwardState& other) = default;
	GolemWalkForwardState& operator = (const GolemWalkForwardState& other) = default;
};

//���
class GolemWalkBackWardState : public GolemStateBase {
public:
	GolemWalkBackWardState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemWalkBackWardState(const GolemWalkBackWardState& other) = default;
	GolemWalkBackWardState& operator = (const GolemWalkBackWardState& other) = default;
};

//�E
class GolemThinkRightMoveState : public GolemStateBase {
public:
	GolemThinkRightMoveState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemThinkRightMoveState(const GolemThinkRightMoveState& other) = default;
	GolemThinkRightMoveState& operator = (const GolemThinkRightMoveState& other) = default;
};

//��
class GolemThinkLeftMoveState : public GolemStateBase {
public:
	GolemThinkLeftMoveState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemThinkLeftMoveState(const GolemThinkLeftMoveState& other) = default;
	GolemThinkLeftMoveState& operator = (const GolemThinkLeftMoveState& other) = default;
};
//-------------------------------------------�U���X�e�[�g-----------------------------------------

//���߉�]�a��
class GolemAttackChargeSpin : public GolemStateBase {
public:
	GolemAttackChargeSpin(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemAttackChargeSpin(const GolemAttackChargeSpin& other) = default;
	GolemAttackChargeSpin& operator = (const GolemAttackChargeSpin& other) = default;
};

//�W�����v�U��
class GolemAttackJumpingState : public GolemStateBase {
public:
	GolemAttackJumpingState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemAttackJumpingState(const GolemAttackJumpingState& other) = default;
	GolemAttackJumpingState& operator = (const GolemAttackJumpingState& other) = default;
};

//��]����
class GolemAttackWildDanceRotationState : public GolemStateBase {
public:
	GolemAttackWildDanceRotationState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemAttackWildDanceRotationState(const GolemAttackWildDanceRotationState& other) = default;
	GolemAttackWildDanceRotationState& operator = (const GolemAttackWildDanceRotationState& other) = default;
};

//���ߗ���
class GolemAttackChargedDanceState : public GolemStateBase {
public:
	GolemAttackChargedDanceState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemAttackChargedDanceState(const GolemAttackChargedDanceState& other) = default;
	GolemAttackChargedDanceState& operator = (const GolemAttackChargedDanceState& other) = default;
};

//�����o���U��
class GolemAttackPushState : public GolemStateBase {
public:
	GolemAttackPushState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemAttackPushState(const GolemAttackPushState& other) = default;
	GolemAttackPushState& operator = (const GolemAttackPushState& other) = default;
};

//�R�A�U��
class GolemAttackTriple : public GolemStateBase {
public:
	GolemAttackTriple(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemAttackTriple(const GolemAttackTriple& other) = default;
	GolemAttackTriple& operator = (const GolemAttackTriple& other) = default;
};

//���ߍU��
class GolemAttackForState : public GolemStateBase {
public:
	GolemAttackForState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemAttackForState(const GolemAttackForState& other) = default;
	GolemAttackForState& operator = (const GolemAttackForState& other) = default;
};

//----------------------------------------------�W�����v-----------------------------
class GolemJumpState : public GolemStateBase {
public:
	GolemJumpState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemJumpState(const GolemJumpState& other) = default;
	GolemJumpState& operator = (const GolemJumpState& other) = default;
};
//----------------------------------------�X�^��--------------------------------------

//�X�^���X�^�[�g
class GolemStanStartState : public GolemStateBase {
public:
	GolemStanStartState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemStanStartState(const GolemStanStartState& other) = default;
	GolemStanStartState& operator = (const GolemStanStartState& other) = default;
};

//�X�^�����[�v
class GolemStanLoopState : public GolemStateBase {
public:
	GolemStanLoopState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemStanLoopState(const GolemStanLoopState& other) = default;
	GolemStanLoopState& operator = (const GolemStanLoopState& other) = default;

private:
	float m_Return;		//�ʏ�ɖ߂鎞��
};

//�X�^���G���h
class GolemStanEndState : public GolemStateBase {
public:
	GolemStanEndState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemStanEndState(const GolemStanEndState& other) = default;
	GolemStanEndState& operator = (const GolemStanEndState& other) = default;
};
//-------------------------------------------���S-----------------------------------------

class GolemDieState : public GolemStateBase {
public:
	GolemDieState(Golem* owner, IWorld* world) :
		GolemStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	GolemDieState(const GolemDieState& other) = default;
	GolemDieState& operator = (const GolemDieState& other) = default;
};

#endif