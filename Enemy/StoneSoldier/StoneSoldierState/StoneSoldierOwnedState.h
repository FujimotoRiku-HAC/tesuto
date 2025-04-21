#ifndef STONE_SOLDIER_OWNED_STATE_H_
#define STONE_SOLDIER_OWNED_STATE_H_

#include "State/State.h"
#include "Actor/Enemy/StoneSoldier/StoneSoldierMotionType.h"
#include "Actor/Enemy/StoneSoldier/StoneSoldier.h"

class StoneSoldierOwnedState {
public:
	enum {
		Spawn,						//出現
		Idle,						//アイドル
		ForwardWalk,				//前歩き
		BackwardWalk,				//後ろ歩き
		RightWalk,					//右歩き
		LeftWalk,					//左歩き
		BeforePounceAttack,			//飛び攻撃前
		PounceAttack,				//飛び攻撃
		BeforePeripheralAttack,		//周辺攻撃前
		PeripheralAttack,			//周辺攻撃
		HitFront,					//前攻撃リアクション
		StrongHitFront,				//前強攻撃リアクション
		BlowHitFront,				//前ブロウ攻撃リアクション
		PushUpHitFront,				//前プッシュアップ攻撃リアクション
		HitBack,					//後ろ攻撃リアクション
		StrongHitBack,				//後ろ強攻撃リアクション
		BlowHitBack,				//後ろブロウ攻撃リアクション
		PushUpHitBack,				//後ろプッシュアップ攻撃リアクション
		CollapseAfterPushUp,			//プッシュ攻撃受けた際の起き上がり
		GetUp,						//起き上がり
		BeforeDie,					//死亡前
		Die							//死亡時
	};
};
//インターバル
const float AttackInterval{ 300.0f };
//ターンアングル
const float TurnAngle{ 2.5f };
//横移動
const float SideBackWalk{ 0.5f };
//後ろ歩きの距離
const float BackDistance{ 2.0f };
//回避できる距離・視界
const float LeaveDistance{ 2.0f };
const float LeaveAngle{ 10.0f };
//横移動の距離
const float AskDistance{ 4.5f };
//攻撃できる距離
const float AttackDistance{ 4.5f };
//回避移動する時間
const float LeaveTimer{ 30.0f };
//回避スピード倍率
const float LeaveSpeed{ 5.5f };
//とびかかり攻撃
const float PounceTimerStart{ 2.0f };
const float PounceTimerEnd{ 50.0f };
const float PounceHitSpeed{ 3.2f };
const float BeforePounceTimerEnd{ 80.0f };
const float BeforePounceTimerStart{ 55.0f };
//周辺攻撃
const float BeforePeripheralTimeEnd{ 110.0f };
const float BeforePeripheralTimeStart{ 65.0f };
//ブロウ攻撃を受けたとき
const float HitBlowEnd{ 40.0f };
const float HitBlowSpeed{ 7.5f };
//プッシュ攻撃を受けたとき
const float HitPushEnd{ 30.0f };
const float HitPushSpeed{ 5.5f };
const float HitPushY{ 0.35f };
//強攻撃を受けたとき
const float HitStrongEnd{ 30.0f };
const float HitStrongSpeed{ 4.5f };



//ボスの基底クラス
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

//---------------------------------------------------スポーン状態--------------------------------------------------------------
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


//アイドル状態
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
	bool IsBack()const;				//後ろに下がるか
	bool IsAsk()const;				//横移動するか
	bool IsRandomAttack() const;	//攻撃するか
};

//-------------------------------------------------------------歩く--------------------------------------------------------

//前
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

//後ろ
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

//-----------------------------------------------------------攻撃ステート----------------------------------------------------

//攻撃前
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

//周りに敵がいるときの攻撃状態
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

//とびかかり攻撃状態
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

//周り攻撃状態
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

//-------------------------------------------様子をうかがう状態-----------------------------------------------

//右
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

//左
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


//-----------------------------------------ヒット状態-----------------------------------------------

//前攻撃
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

//強攻撃前状態
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

//ブロウ前攻撃
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

//プッシュアップ前攻撃
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

//後ろから攻撃を受けた
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

//後ろから強攻撃を受けた
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

//吹き飛ばされる(後ろ)
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

//プッシュ攻撃(後ろ)
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

//プッシュ攻撃を受けて倒れ込む
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

//-----------------------------------------------------起き上がり-------------------------------------------------

//起き上がる
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

//-----------------------------------------------------死亡状態-----------------------------------------------------

//死亡前モーション
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

//死亡モーション
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