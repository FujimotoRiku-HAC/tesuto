#ifndef PLAYER_OWNED_STATE_H_
#define PLAYER_OWNED_STATE_H_

#include "State/State.h"
#include "Actor/Player/Player.h"
#include "Actor/Player/PlayerAnim.h"
#include "Animation/AnimatedMesh.h"
#include "Common/InputManager.h"
#include <vector>

class PlayerOwnedState {
public:
	enum {
		Idle,								//�A�C�h��
		Walk,								//����
		FirstAttack,						//�ꌂ��
		SecondAttack,						//�񌂖�
		ThirdAttack,						//�O����
		JustAvoidanceAttack,				//�W���X�g�U��
		AvoidanceRight,						//�E���
		AvoidanceLeft,						//�����
		AvoidanceForward,					//�O���
		AvoidanceBack,						//�����
		AbilityBlowAttack,					//�u���[�U��
		AbilityBlowAttackReinforcement,		//�u���[�U���񌂖�
		AbilityPushAttack,					//�v�b�V���U��
		AbilityAvoidAttack,					//����U��
		AbilityRotatingAttack,				//��]�a��
		AbilityInpactAttack,				//�C���p�N�g�U��
		AbilitySlamAttack,					//�@�����U��
		AbilityWildDance,					//����
		AbilityWaveWildDance_1,				//�g������
		AbilityWaveWildDance_2,				//�g�������񌂖�
		AbilityWaveWildDance_3,				//�g�������O����
		AbilitySlashAttack,					//���
		HitForward,							//�O�q�b�g
		HitBackward,						//���q�b�g
		HitLeft,							//���q�b�g
		HitRight,							//�E�q�b�g
		StrongHit,							//���q�b�g
		GetUp,								//�N���オ��
		Die,								//���S
		DieLoop,							//���S���[�v
	};
};

class Player;

//�~�܂鋗��
const float StopDistance{ 1.5f };
//�u���E�U���̃��[�V�����I���𑁂߂�
const float BlowMotionEndTime{ 30.0f };
//�E�F�C�u�U���̃��[�V�����I���𑁂߂�
const float WaveMotionEndTime{ 30.0f };
//
const float AvoidSpeed{ 1.2f };
//
const float AvoidAnimMotionStart{ 5.0f };
const float AvoidAnimMotionEnd{ 20.0f };
//���[�V�������ԑ��߂�
const float AvoidMotionEnd{ 10.0f };


//�{�X�̊��N���X
class PlayerStateBase : public State<Player> {
public:
	PlayerStateBase(Player* owner, IWorld* world = nullptr) :
		State{ owner }, world_{ world } {}
	void Enter()override {};
	void Execute(float delta_time)override {};
	void Draw()const override {};
	void Exit() override {};
	void OnMessage(const std::string& message, void* param = nullptr)override {};
protected:
	IWorld* world_{ nullptr };
};

//�A�C�h�����
class PlayerIdleState : public PlayerStateBase {
public:
	PlayerIdleState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerIdleState(const PlayerIdleState& other) = default;
	PlayerIdleState& operator = (const PlayerIdleState& other) = default;
private:
	float m_Timer{ 0.0f };
};

//----------------------------------------�U���X�e�[�g---------------------------------------
// 
//�ꌂ��
class PlayerAttackFirstState : public PlayerStateBase {
public:
	PlayerAttackFirstState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Draw()const override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAttackFirstState(const PlayerAttackFirstState& other) = default;
	PlayerAttackFirstState& operator = (const PlayerAttackFirstState& other) = default;

private:

	bool Distance()const;
	Actor* m_Enemy{ nullptr };
};

//�񌂖�
class PlayerAttackSecondState : public PlayerStateBase {
public:
	PlayerAttackSecondState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Draw()const override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAttackSecondState(const PlayerAttackSecondState& other) = default;
	PlayerAttackSecondState& operator = (const PlayerAttackSecondState& other) = default;
private:
	bool Distance()const;
	Actor* m_Enemy{ nullptr };
};

//�O����
class PlayerAttackThirdState : public PlayerStateBase {
public:
	PlayerAttackThirdState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Draw()const override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAttackThirdState(const PlayerAttackThirdState& other) = default;
	PlayerAttackThirdState& operator = (const PlayerAttackThirdState& other) = default;
private:
	bool Distance()const;
	Actor* m_Enemy{ nullptr };
};

//-------------------------------------------������------------------------------------------
// 
//�O
class PlayerAvoidForwardState : public PlayerStateBase {
public:
	PlayerAvoidForwardState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAvoidForwardState(const PlayerAvoidForwardState& other) = default;
	PlayerAvoidForwardState& operator = (const PlayerAvoidForwardState& other) = default;
};

//���
class PlayerAvoidBackState : public PlayerStateBase {
public:
	PlayerAvoidBackState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAvoidBackState(const PlayerAvoidBackState& other) = default;
	PlayerAvoidBackState& operator = (const PlayerAvoidBackState& other) = default;
};

//�E
class PlayerAvoidRightState : public PlayerStateBase {
public:
	PlayerAvoidRightState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAvoidRightState(const PlayerAvoidRightState& other) = default;
	PlayerAvoidRightState& operator = (const PlayerAvoidRightState& other) = default;
};

//��
class PlayerAvoidLeftState : public PlayerStateBase {
public:
	PlayerAvoidLeftState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAvoidLeftState(const PlayerAvoidLeftState& other) = default;
	PlayerAvoidLeftState& operator = (const PlayerAvoidLeftState& other) = default;
};

//--------------------------------�q�b�g���--------------------------------

//�O�q�b�g
class PlayerHitForwardState : public PlayerStateBase {
public:
	PlayerHitForwardState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerHitForwardState(const PlayerHitForwardState& other) = default;
	PlayerHitForwardState& operator = (const PlayerHitForwardState& other) = default;
};

//���q�b�g
class PlayerHitBackwardState : public PlayerStateBase {
public:
	PlayerHitBackwardState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerHitBackwardState(const PlayerHitBackwardState& other) = default;
	PlayerHitBackwardState& operator = (const PlayerHitBackwardState& other) = default;
};

//�E�q�b�g
class PlayerHitRightState : public PlayerStateBase {
public:
	PlayerHitRightState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerHitRightState(const PlayerHitRightState& other) = default;
	PlayerHitRightState& operator = (const PlayerHitRightState& other) = default;
};

//���q�b�g
class PlayerHitLeftState : public PlayerStateBase {
public:
	PlayerHitLeftState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerHitLeftState(const PlayerHitLeftState& other) = default;
	PlayerHitLeftState& operator = (const PlayerHitLeftState& other) = default;
};

//�O���U���q�b�g
class PlayerHitStrongState : public PlayerStateBase {
public:
	PlayerHitStrongState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerHitStrongState(const PlayerHitStrongState& other) = default;
	PlayerHitStrongState& operator = (const PlayerHitStrongState& other) = default;
};

//----------------------------------------------�N���オ��-----------------------------------------------------

class PlayerGetUpState : public PlayerStateBase {
public:
	PlayerGetUpState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerGetUpState(const PlayerGetUpState& other) = default;
	PlayerGetUpState& operator = (const PlayerGetUpState& other) = default;
};

//--------------------------------------------------���S�X�e�[�g---------------------------------------

class PlayerDieState : public PlayerStateBase {
public:
	PlayerDieState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerDieState(const PlayerDieState& other) = default;
	PlayerDieState& operator = (const PlayerDieState& other) = default;
};

class PlayerDieLoopState : public PlayerStateBase {
public:
	PlayerDieLoopState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerDieLoopState(const PlayerDieLoopState& other) = default;
	PlayerDieLoopState& operator = (const PlayerDieLoopState& other) = default;
};

//--------------------------------------------------------�W���X�g�U��--------------------------------------------------
class PlayerJustAvoidanceAttackState : public PlayerStateBase {
public:
	PlayerJustAvoidanceAttackState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerJustAvoidanceAttackState(const PlayerJustAvoidanceAttackState& other) = default;
	PlayerJustAvoidanceAttackState& operator = (const PlayerJustAvoidanceAttackState& other) = default;

private:
	bool Distance()const;
	float m_Speed;

	Actor* m_Enemy{ nullptr };

};

//---------------------------------------�A�r���e�B--------------------------------

//�t����΂��U��
class PlayerAbilityBlowAttackState : public PlayerStateBase {
public:
	PlayerAbilityBlowAttackState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAbilityBlowAttackState(const PlayerAbilityBlowAttackState& other) = default;
	PlayerAbilityBlowAttackState& operator = (const PlayerAbilityBlowAttackState& other) = default;
private:
	bool Distance()const;

	Actor* m_Enemy{ nullptr };
};

//�t����΂��U��(������)
class PlayerAbilityBlowAttackReinforcementState : public PlayerStateBase {
public:
	PlayerAbilityBlowAttackReinforcementState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAbilityBlowAttackReinforcementState(const PlayerAbilityBlowAttackReinforcementState& other) = default;
	PlayerAbilityBlowAttackReinforcementState& operator = (const PlayerAbilityBlowAttackReinforcementState& other) = default;
private:
	bool Distance()const;

	Actor* m_Enemy{ nullptr };
	float speed{ 1.0f };

};

//�󒆂ɔ�΂���Z
class PlayerAbilityPushAttackState : public PlayerStateBase {
public:
	PlayerAbilityPushAttackState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAbilityPushAttackState(const PlayerAbilityPushAttackState& other) = default;
	PlayerAbilityPushAttackState& operator = (const PlayerAbilityPushAttackState& other) = default;
private:
	bool Distance()const;

	Actor* m_Enemy{ nullptr };
};

//����U��
class PlayerAbilityAvoidAttackState : public PlayerStateBase {
public:
	PlayerAbilityAvoidAttackState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAbilityAvoidAttackState(const PlayerAbilityAvoidAttackState& other) = default;
	PlayerAbilityAvoidAttackState& operator = (const PlayerAbilityAvoidAttackState& other) = default;

private:

	Actor* m_Enemy{ nullptr };

};

//��]�a��
class PlayerAbilityRotatingAttackState : public PlayerStateBase {
public:
	PlayerAbilityRotatingAttackState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAbilityRotatingAttackState(const PlayerAbilityRotatingAttackState& other) = default;
	PlayerAbilityRotatingAttackState& operator = (const PlayerAbilityRotatingAttackState& other) = default;

private:

	Actor* m_Enemy{ nullptr };

};

//�Ռ��g������ɏo��
class PlayerAbilityInpactGroundAttackState : public PlayerStateBase {
public:
	PlayerAbilityInpactGroundAttackState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAbilityInpactGroundAttackState(const PlayerAbilityInpactGroundAttackState& other) = default;
	PlayerAbilityInpactGroundAttackState& operator = (const PlayerAbilityInpactGroundAttackState& other) = default;

private:
	Actor* m_Enemy{ nullptr };
};

//�@�����U��
class PlayerAbilitySlamAttackState : public PlayerStateBase {
public:
	PlayerAbilitySlamAttackState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAbilitySlamAttackState(const PlayerAbilitySlamAttackState& other) = default;
	PlayerAbilitySlamAttackState& operator = (const PlayerAbilitySlamAttackState& other) = default;

private:
	Actor* m_Enemy{ nullptr };
};

//�Ռ��g�����P
class PlayerAbilityWaveWildDanceAttackState_1 : public PlayerStateBase {
public:
	PlayerAbilityWaveWildDanceAttackState_1(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAbilityWaveWildDanceAttackState_1(const PlayerAbilityWaveWildDanceAttackState_1& other) = default;
	PlayerAbilityWaveWildDanceAttackState_1& operator = (const PlayerAbilityWaveWildDanceAttackState_1& other) = default;
private:
	Actor* m_Enemy{ nullptr };
};

//�Ռ��g�����Q
class PlayerAbilityWaveWildDanceAttackState_2 : public PlayerStateBase {
public:
	PlayerAbilityWaveWildDanceAttackState_2(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAbilityWaveWildDanceAttackState_2(const PlayerAbilityWaveWildDanceAttackState_2& other) = default;
	PlayerAbilityWaveWildDanceAttackState_2& operator = (const PlayerAbilityWaveWildDanceAttackState_2& other) = default;
private:
	Actor* m_Enemy{ nullptr };
};

//�Ռ��g�����R
class PlayerAbilityWaveWildDanceAttackState_3 : public PlayerStateBase {
public:
	PlayerAbilityWaveWildDanceAttackState_3(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAbilityWaveWildDanceAttackState_3(const PlayerAbilityWaveWildDanceAttackState_3& other) = default;
	PlayerAbilityWaveWildDanceAttackState_3& operator = (const PlayerAbilityWaveWildDanceAttackState_3& other) = default;
private:
	Actor* m_Enemy{ nullptr };


};

//�����U��
class PlayerAbilityWildDanceAttackState : public PlayerStateBase {
public:
	PlayerAbilityWildDanceAttackState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAbilityWildDanceAttackState(const PlayerAbilityWildDanceAttackState& other) = default;
	PlayerAbilityWildDanceAttackState& operator = (const PlayerAbilityWildDanceAttackState& other) = default;

private:
	Actor* m_Enemy{ nullptr };

};

//���
class PlayerAbilitySlashAttackState : public PlayerStateBase {
public:
	PlayerAbilitySlashAttackState(Player* owner, IWorld* world) :
		PlayerStateBase{ owner,world } {}

	void Enter() override;

	void Execute(float delta_time) override;

	void Exit() override;

	void OnMessage(const std::string& message, void* param);

	PlayerAbilitySlashAttackState(const PlayerAbilitySlashAttackState& other) = default;
	PlayerAbilitySlashAttackState& operator = (const PlayerAbilitySlashAttackState& other) = default;

private:
	Actor* m_Enemy{ nullptr };
};

#endif