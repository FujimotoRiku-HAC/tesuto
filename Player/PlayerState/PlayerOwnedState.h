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
		Idle,								//アイドル
		Walk,								//歩き
		FirstAttack,						//一撃目
		SecondAttack,						//二撃目
		ThirdAttack,						//三撃目
		JustAvoidanceAttack,				//ジャスト攻撃
		AvoidanceRight,						//右回避
		AvoidanceLeft,						//左回避
		AvoidanceForward,					//前回避
		AvoidanceBack,						//後ろ回避
		AbilityBlowAttack,					//ブロー攻撃
		AbilityBlowAttackReinforcement,		//ブロー攻撃二撃目
		AbilityPushAttack,					//プッシュ攻撃
		AbilityAvoidAttack,					//回避攻撃
		AbilityRotatingAttack,				//回転斬り
		AbilityInpactAttack,				//インパクト攻撃
		AbilitySlamAttack,					//叩きつけ攻撃
		AbilityWildDance,					//乱舞
		AbilityWaveWildDance_1,				//波動乱舞
		AbilityWaveWildDance_2,				//波動乱舞二撃目
		AbilityWaveWildDance_3,				//波動乱舞三撃目
		AbilitySlashAttack,					//牙突
		HitForward,							//前ヒット
		HitBackward,						//後ろヒット
		HitLeft,							//左ヒット
		HitRight,							//右ヒット
		StrongHit,							//強ヒット
		GetUp,								//起き上がり
		Die,								//死亡
		DieLoop,							//死亡ループ
	};
};

class Player;

//止まる距離
const float StopDistance{ 1.5f };
//ブロウ攻撃のモーション終了を早める
const float BlowMotionEndTime{ 30.0f };
//ウェイブ攻撃のモーション終了を早める
const float WaveMotionEndTime{ 30.0f };
//
const float AvoidSpeed{ 1.2f };
//
const float AvoidAnimMotionStart{ 5.0f };
const float AvoidAnimMotionEnd{ 20.0f };
//モーション時間早める
const float AvoidMotionEnd{ 10.0f };


//ボスの基底クラス
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

//アイドル状態
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

//----------------------------------------攻撃ステート---------------------------------------
// 
//一撃目
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

//二撃目
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

//三撃目
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

//-------------------------------------------回避状態------------------------------------------
// 
//前
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

//後ろ
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

//右
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

//左
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

//--------------------------------ヒット状態--------------------------------

//前ヒット
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

//後ろヒット
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

//右ヒット
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

//左ヒット
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

//前強攻撃ヒット
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

//----------------------------------------------起き上がる-----------------------------------------------------

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

//--------------------------------------------------死亡ステート---------------------------------------

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

//--------------------------------------------------------ジャスト攻撃--------------------------------------------------
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

//---------------------------------------アビリティ--------------------------------

//付き飛ばす攻撃
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

//付き飛ばす攻撃(強化中)
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

//空中に飛ばせる技
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

//回避攻撃
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

//回転斬り
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

//衝撃波を周りに出す
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

//叩きつけ攻撃
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

//衝撃波乱舞１
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

//衝撃波乱舞２
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

//衝撃波乱舞３
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

//乱舞攻撃
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

//牙突
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