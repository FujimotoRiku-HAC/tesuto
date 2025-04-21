#ifndef STONE_SOLDIER_OWNED_STATE_H_
#define STONE_SOLDIER_OWNED_STATE_H_

#include "State/State.h"
#include "Actor/Enemy/Golem/GolemMotionType.h"
#include "Actor/Enemy/Golem/Golem.h"

class GolemOwnedState {
public:
	enum {
		Idle,						//アイドル
		RightThink,					//右移動
		LeftThink,					//左移動
		ForwardWalk,				//前歩き
		BackWalk,					//後ろ歩き
		ChargeSpinAttack,			//溜め回転斬り
		JumpingAttack,				//ジャンプ攻撃
		WildDanceRotatingAttack,	//乱舞回転
		ChargeDanceAttack,			//溜め乱舞
		PushAttack,					//押し出し攻撃
		TripleAttack,				//3連攻撃
		ForAttack,					//溜め
		Jump,						//ジャンプ
		Generate,					//出現
		StanStart,					//スタンスタート
		StanLoop,					//スタンループ
		StanEnd,					//スタンエンド
		Die							//死亡

	};
};
//ターンアングル
const float TurnAngle{ 2.5f };
//横移動スピード
const float ThinkSpeed{ 0.3f };
//後ろに行くスピード
const float BackSpeed{ 0.25f };
//攻撃インターバル
const float AttackInterval{ 60.0f* 2 };
//横移動する距離
const float AskDis{ 6.0f };
//歩ける距離
const float WalkDis{ 3.0f };
//遠距離攻撃
const float LongAttackDis{ 10.0f };
//攻撃距離
const float AttackDis{ 6.0f };
//止まる距離
const float StopDis{ 1.5f };
//戻る時間
const float ReturnTimer{ 300.0f };
//通常攻撃時に回る時間
const float AttackTurnTime{ 10.0f };
//ジャンプ攻撃時に回る時間
const float JumpAttackTurnTime{ 110.0f };
//乱舞時の回る時間
const float WildDanceAttackTurnTime{ 100.0f };


//ボスの基底クラス
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

//---------------------------------------------出現状態----------------------------------------------------
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

//アイドル状態
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

//-----------------------------------------------プレイヤーまで歩く---------------------------------------------

//前
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

//後ろ
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

//右
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

//左
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
//-------------------------------------------攻撃ステート-----------------------------------------

//ため回転斬り
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

//ジャンプ攻撃
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

//回転乱舞
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

//溜め乱舞
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

//押し出し攻撃
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

//３連攻撃
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

//ため攻撃
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

//----------------------------------------------ジャンプ-----------------------------
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
//----------------------------------------スタン--------------------------------------

//スタンスタート
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

//スタンループ
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
	float m_Return;		//通常に戻る時間
};

//スタンエンド
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
//-------------------------------------------死亡-----------------------------------------

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