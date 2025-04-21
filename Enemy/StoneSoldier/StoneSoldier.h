#ifndef STONE_SOLDIER_H_
#define STONE_SOLDIER_H_

#include "Actor/Enemy/Enemy.h"
#include "Animation/AnimatedMesh.h"
#include "StoneSoldierMotionType.h" 
#include "State/StateMachine.h"

//敵クラス
class StoneSoldier : public Enemy {

public:

	//コンストラクタ
	StoneSoldier(IWorld* world, const GSvector3& position,const GSvector3& rotation);
	//更新
	virtual void Update(float delta_time)override;
	//描画
	virtual void Draw() const override;
	//GUI描画
	virtual void DrawGui() const override;
	//衝突リアクション
	virtual void React(Actor& other)override;
	//メッセージ
	virtual void HandleMessage(const std::string& message, void* param)override;

public:

	//ステート変更
	void ChangeState(int state, bool restate = false);
	//インターバル前
	void ThinkMove();
	//ランダム攻撃
	void RandomAttack();

private:

	//ステート追加
	void AddState()override;
	//アニメーションイベント追加
	void SetAnimationEvent();
	//攻撃を受けた後のモーション変更
	void DecentHitMotion(Actor& other);

private:
	bool m_IsStrongHit{ false };				//強攻撃を受けたか
	bool m_IsBlowHit{ false };					//ブロウ攻撃を受けたか
	bool m_IsPush{ false };						//プッシュ攻撃を受けたか
	bool m_IsGetUp{ false };					//起き上がるか

	StateMachine<StoneSoldier> m_StateMachine;	//ステートマシン
};

#endif