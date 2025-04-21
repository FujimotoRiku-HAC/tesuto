#ifndef STONE_SOLDIER_OWNED_STATE_H_
#define STONE_SOLDIER_OWNED_STATE_H_

#include "State/State.h"
#include "Actor/Enemy/StoneSoldier/StoneSoldierMotionType.h"
#include "Actor/Enemy/StoneSoldier/StoneSoldier.h"

class StoneSoldierOwnedState {
public:
	enum {
		Spawn,						//�o��
		Idle,						//�A�C�h��
		ForwardWalk,				//�O����
		BackwardWalk,				//������
		RightWalk,					//�E����
		LeftWalk,					//������
		BeforePounceAttack,			//��эU���O
		PounceAttack,				//��эU��
		BeforePeripheralAttack,		//���ӍU���O
		PeripheralAttack,			//���ӍU��
		HitFront,					//�O�U�����A�N�V����
		StrongHitFront,				//�O���U�����A�N�V����
		BlowHitFront,				//�O�u���E�U�����A�N�V����
		PushUpHitFront,				//�O�v�b�V���A�b�v�U�����A�N�V����
		HitBack,					//���U�����A�N�V����
		StrongHitBack,				//��닭�U�����A�N�V����
		BlowHitBack,				//���u���E�U�����A�N�V����
		PushUpHitBack,				//���v�b�V���A�b�v�U�����A�N�V����
		CollapseAfterPushUp,			//�v�b�V���U���󂯂��ۂ̋N���オ��
		GetUp,						//�N���オ��
		BeforeDie,					//���S�O
		Die							//���S��
	};
};
//�C���^�[�o��
const float AttackInterval{ 300.0f };
//�^�[���A���O��
const float TurnAngle{ 2.5f };
//���ړ�
const float SideBackWalk{ 0.5f };
//�������̋���
const float BackDistance{ 2.0f };
//����ł��鋗���E���E
const float LeaveDistance{ 2.0f };
const float LeaveAngle{ 10.0f };
//���ړ��̋���
const float AskDistance{ 4.5f };
//�U���ł��鋗��
const float AttackDistance{ 4.5f };
//����ړ����鎞��
const float LeaveTimer{ 30.0f };
//����X�s�[�h�{��
const float LeaveSpeed{ 5.5f };
//�Ƃт�����U��
const float PounceTimerStart{ 2.0f };
const float PounceTimerEnd{ 50.0f };
const float PounceHitSpeed{ 3.2f };
const float BeforePounceTimerEnd{ 80.0f };
const float BeforePounceTimerStart{ 55.0f };
//���ӍU��
const float BeforePeripheralTimeEnd{ 110.0f };
const float BeforePeripheralTimeStart{ 65.0f };
//�u���E�U�����󂯂��Ƃ�
const float HitBlowEnd{ 40.0f };
const float HitBlowSpeed{ 7.5f };
//�v�b�V���U�����󂯂��Ƃ�
const float HitPushEnd{ 30.0f };
const float HitPushSpeed{ 5.5f };
const float HitPushY{ 0.35f };
//���U�����󂯂��Ƃ�
const float HitStrongEnd{ 30.0f };
const float HitStrongSpeed{ 4.5f };



//�{�X�̊��N���X
class StoneSoldierStateBase : public State<StoneSoldier> {
public:
	StoneSoldierStateBase(StoneSoldier* owner, IWorld* world = nullptr) :
		State{ owner }, world_{ world } {}
	void Enter()override {};
	void Execute(float delta_time)override {};
	void Draw()const override {};
	void Exit() override {};
	void OnMessage(const std::string& message, void* param = nullptr)override {};
protected:
	IWorld* world_{ nullptr };
};

//---------------------------------------------------�X�|�[�����--------------------------------------------------------------
class StoneSoldierSpawnState : public StoneSoldierStateBase {
public:
	StoneSoldierSpawnState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierSpawnState(const StoneSoldierSpawnState& other) = default;
	StoneSoldierSpawnState& operator = (const StoneSoldierSpawnState& other) = default;
};


//�A�C�h�����
class StoneSoldierIdleState : public StoneSoldierStateBase {
public:
	StoneSoldierIdleState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierIdleState(const StoneSoldierIdleState& other) = default;
	StoneSoldierIdleState& operator = (const StoneSoldierIdleState& other) = default;

private:
	bool IsBack()const;				//���ɉ����邩
	bool IsAsk()const;				//���ړ����邩
	bool IsRandomAttack() const;	//�U�����邩
};

//-------------------------------------------------------------����--------------------------------------------------------

//�O
class StoneSoldierWalkForwardState : public StoneSoldierStateBase {
public:
	StoneSoldierWalkForwardState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierWalkForwardState(const StoneSoldierWalkForwardState& other) = default;
	StoneSoldierWalkForwardState& operator = (const StoneSoldierWalkForwardState& other) = default;
};

//���
class StoneSoldierWalkBackwardState : public StoneSoldierStateBase {
public:
	StoneSoldierWalkBackwardState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierWalkBackwardState(const StoneSoldierWalkBackwardState& other) = default;
	StoneSoldierWalkBackwardState& operator = (const StoneSoldierWalkBackwardState& other) = default;
};

//-----------------------------------------------------------�U���X�e�[�g----------------------------------------------------

//�U���O
class StoneSoldierBeforePounceAttackState : public StoneSoldierStateBase {
public:
	StoneSoldierBeforePounceAttackState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierBeforePounceAttackState(const StoneSoldierBeforePounceAttackState& other) = default;
	StoneSoldierBeforePounceAttackState& operator = (const StoneSoldierBeforePounceAttackState& other) = default;
};

//����ɓG������Ƃ��̍U�����
class StoneSoldierBeforePeripheralAttackState : public StoneSoldierStateBase {
public:
	StoneSoldierBeforePeripheralAttackState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierBeforePeripheralAttackState(const StoneSoldierBeforePeripheralAttackState& other) = default;
	StoneSoldierBeforePeripheralAttackState& operator = (const StoneSoldierBeforePeripheralAttackState& other) = default;
};

//�Ƃт�����U�����
class StoneSoldierPounceAttackState : public StoneSoldierStateBase {
public:
	StoneSoldierPounceAttackState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierPounceAttackState(const StoneSoldierPounceAttackState& other) = default;
	StoneSoldierPounceAttackState& operator = (const StoneSoldierPounceAttackState& other) = default;
};

//����U�����
class StoneSoldierPeripheralAttackState : public StoneSoldierStateBase {
public:
	StoneSoldierPeripheralAttackState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierPeripheralAttackState(const StoneSoldierPeripheralAttackState& other) = default;
	StoneSoldierPeripheralAttackState& operator = (const StoneSoldierPeripheralAttackState& other) = default;
};

//-------------------------------------------�l�q�������������-----------------------------------------------

//�E
class StoneSoldierThinkRightState : public StoneSoldierStateBase {
public:
	StoneSoldierThinkRightState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierThinkRightState(const StoneSoldierThinkRightState& other) = default;
	StoneSoldierThinkRightState& operator = (const StoneSoldierThinkRightState& other) = default;
};

//��
class StoneSoldierThinkLeftState : public StoneSoldierStateBase {
public:
	StoneSoldierThinkLeftState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierThinkLeftState(const StoneSoldierThinkLeftState& other) = default;
	StoneSoldierThinkLeftState& operator = (const StoneSoldierThinkLeftState& other) = default;
};


//-----------------------------------------�q�b�g���-----------------------------------------------

//�O�U��
class StoneSoldierHitFrontState : public StoneSoldierStateBase {
public:
	StoneSoldierHitFrontState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierHitFrontState(const StoneSoldierHitFrontState& other) = default;
	StoneSoldierHitFrontState& operator = (const StoneSoldierHitFrontState& other) = default;
};

//���U���O���
class StoneSoldierHitFrontStrongState : public StoneSoldierStateBase {
public:
	StoneSoldierHitFrontStrongState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierHitFrontStrongState(const StoneSoldierHitFrontStrongState& other) = default;
	StoneSoldierHitFrontStrongState& operator = (const StoneSoldierHitFrontStrongState& other) = default;
};

//�u���E�O�U��
class StoneSoldierHitFrontBlowState : public StoneSoldierStateBase {
public:
	StoneSoldierHitFrontBlowState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierHitFrontBlowState(const StoneSoldierHitFrontBlowState& other) = default;
	StoneSoldierHitFrontBlowState& operator = (const StoneSoldierHitFrontBlowState& other) = default;
};

//�v�b�V���A�b�v�O�U��
class StoneSoldierHitFrontPushUpState : public StoneSoldierStateBase {
public:
	StoneSoldierHitFrontPushUpState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierHitFrontPushUpState(const StoneSoldierHitFrontPushUpState& other) = default;
	StoneSoldierHitFrontPushUpState& operator = (const StoneSoldierHitFrontPushUpState& other) = default;
};

//��납��U�����󂯂�
class StoneSoldierHitBackState : public StoneSoldierStateBase {
public:
	StoneSoldierHitBackState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierHitBackState(const StoneSoldierHitBackState& other) = default;
	StoneSoldierHitBackState& operator = (const StoneSoldierHitBackState& other) = default;

};

//��납�狭�U�����󂯂�
class StoneSoldierHitBackStrongState : public StoneSoldierStateBase {
public:
	StoneSoldierHitBackStrongState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierHitBackStrongState(const StoneSoldierHitBackStrongState& other) = default;
	StoneSoldierHitBackStrongState& operator = (const StoneSoldierHitBackStrongState& other) = default;
};

//������΂����(���)
class StoneSoldierHitBackBlowState : public StoneSoldierStateBase {
public:
	StoneSoldierHitBackBlowState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierHitBackBlowState(const StoneSoldierHitBackBlowState& other) = default;
	StoneSoldierHitBackBlowState& operator = (const StoneSoldierHitBackBlowState& other) = default;
};

//�v�b�V���U��(���)
class StoneSoldierHitBackPushUpState : public StoneSoldierStateBase {
public:
	StoneSoldierHitBackPushUpState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierHitBackPushUpState(const StoneSoldierHitBackPushUpState& other) = default;
	StoneSoldierHitBackPushUpState& operator = (const StoneSoldierHitBackPushUpState& other) = default;
};

//�v�b�V���U�����󂯂ē|�ꍞ��
class StoneSoldierCollapseAfterPushUpState : public StoneSoldierStateBase {
public:
	StoneSoldierCollapseAfterPushUpState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierCollapseAfterPushUpState(const StoneSoldierCollapseAfterPushUpState& other) = default;
	StoneSoldierCollapseAfterPushUpState& operator = (const StoneSoldierCollapseAfterPushUpState& other) = default;
};

//-----------------------------------------------------�N���オ��-------------------------------------------------

//�N���オ��
class StoneSoldierGetUpState : public StoneSoldierStateBase {
public:
	StoneSoldierGetUpState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierGetUpState(const StoneSoldierGetUpState& other) = default;
	StoneSoldierGetUpState& operator = (const StoneSoldierGetUpState& other) = default;
};

//-----------------------------------------------------���S���-----------------------------------------------------

//���S�O���[�V����
class StoneSoldierBeforeDieState : public StoneSoldierStateBase {
public:
	StoneSoldierBeforeDieState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierBeforeDieState(const StoneSoldierBeforeDieState& other) = default;
	StoneSoldierBeforeDieState& operator = (const StoneSoldierBeforeDieState& other) = default;
};

//���S���[�V����
class StoneSoldierDieState : public StoneSoldierStateBase {
public:
	StoneSoldierDieState(StoneSoldier* owner, IWorld* world) :
		StoneSoldierStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	StoneSoldierDieState(const StoneSoldierDieState& other) = default;
	StoneSoldierDieState& operator = (const StoneSoldierDieState& other) = default;
};

#endif