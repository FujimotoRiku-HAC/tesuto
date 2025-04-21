#ifndef GOLEM_H_
#define GOLEM_H_

#include "Actor/Enemy/Enemy.h"
#include "State/StateMachine.h"

//敵クラス
class Golem : public Enemy {
public:

	//コンストラクタ
	Golem(IWorld* world, const GSvector3& position, const GSvector3& rotation);
	//更新
	virtual void Update(float delta_time)override;
	//描画
	virtual void Draw() const override;
	//GUI描画
	virtual void DrawGui() const override;
	//衝突リアクション
	virtual void React(Actor& other)override;
	//メッセージ処理
	virtual void HandleMessage(const std::string& message, void* param)override;

public:

	//イベント追加
	void SetAnimationEvent();
	//ステート変更
	void ChangeState(int state, bool restate = false);
	//横移動
	void ThinkMove();
	//攻撃の種類
	void RandamAttack();
	//スタンをリセット
	void ResetStanValue();

private:

	//ステート追加
	void AddState()override;

private:
	
	StateMachine<Golem> m_StateMachine;	//ステートマシン
	int m_HitCount{ 0 };				//ヒットした数
};

#endif