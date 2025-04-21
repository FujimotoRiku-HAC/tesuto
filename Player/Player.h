#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor/Actor.h"
#include "Animation/AnimatedMesh.h"
#include "State/StateMachine.h"
#include "Actor/Player/PlayerAnim.h" 
#include "Actor/Player/Ability/PlayerAbility.h"
#include "Actor/Player/Item/ItemData.h"
#include "Actor/Player/PlayerStatus.h"
#include "UI/Slider.h"
#include "Skill/SkillData.h"

//プレーヤクラス
class Player : public Actor {
public:

	//コンストラクタ
	Player(IWorld* world, const GSvector3& position);
	//デストラクタ
	~Player();
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

	//ステータスクラスの取得
	PlayerStatus& Status()const;
	//入力に応じて移動量を変える
	GSvector3 InputVelocity(GSvector2 input);
	//敵の数を求める
	Actor* FindTargetEnemy()const;
	//入力に応じて回転させる
	void Interpolation(GSvector3 velocity, float delta_time);
	//回避方向の決定
	void DirectionOfAvoidance(GSvector2 input);
	//アニメーションイベント設定
	void SetAnimationEvent();
	// 移動量の変更
	void ChangeVelocity(const GSvector3& vec);
	// 移動量の初期化
	void ResetVelocity();
	//モーション変更
	void ChangeMotion(GSuint motion, bool loop, float speed = 1.0f);
	//ステート変更
	void ChangeState(int state, bool restate = false);
	//攻撃に遷移する
	void SetNextAttackStateFrag(bool next);
	//ジャスト攻撃
	void AvoidanceFrag(bool state);
	//ジャスト攻撃
	void JustAttackFrag(bool state);
	// アイテムで使うエフェクトのセット
	void SetEffect(GSuint effect, bool continuation = false);
	// エフェクトの停止
	void StopEffect();

public:

	//スピード設定
	float PlayerSpeed()const;
	//ターゲットの距離
	float TargetDistance(Actor* other)const;

public:

	//モーションが終了しているか
	bool IsMotionEnd(float time = 1.f)const;
	//次のステートに遷移するか
	bool CanAttackNextState()const;
	//セレクトシーンか
	bool IsSelect()const;
	//ボス戦
	bool BossFight()const;
	//アビリティ中に攻撃が当たったら
	bool IsAbilityReact()const;
	//ジャスト攻撃したか
	bool IsJustAttack()const;
	//ジャスト攻撃したか
	bool IsAvoidance()const;
	//モーション時間
	bool IsMotionTime(float num, float num2 = 0.f)const;

private:

	//現在のステート
	void CurrentState(int s);
	//ヒットストップするか
	void DecentHitStop();
	//敵全体の動きを遅くする
	void HitStop(float time, float scale);
	//フィールドとの衝突処理
	void CollideField();
	//アクターとの衝突処理
	void CollideActor(Actor& other);
	//ダメージモーションの決定
	void DecentHitMotion(Actor& other);
	//ジャスト回避時の更新倍率
	void SlowMagnification(float scale);
	//ジャスト回避実行
	void JustAvoid();
	//ステート追加
	void AddState()override;
	//リムカラー
	void RimColor(GScolor color,float exponent)const;
	//武器描画
	void WeponDraw()const;

private:

	//ターゲットの角度差を求める
	float TargetSignedAngle()const;
	//符号なし
	float TargetAngle()const;

private:

	//同じ状態か
	bool IsInState(int state)const;
	//ノックバックか
	bool IsKnockBack(Actor& other)const;

private:
		//攻撃判定の生成
		void GenerateAttackCollider(
			std::string name,					//攻撃タグ
			PlayerStatus::AttackType damage,	//ダメージ量
			float distance = 1.0f,				//位置
			float rightdis = 0.0f,				//横に出す位置
			float radius = 1.0f,				//半径
			float height = 1.0f,				//高さ
			float life_span = 10.0f);			//寿命
		//衝撃波攻撃の生成
		void GenerateWaveCollider(
			std::string name,
			PlayerStatus::AttackType damage,
			float distance = 1.0f,
			float radius= 0.2f,
			float height = 1.0f);

private:

	//アニメーションメッシュ
	AnimatedMesh m_Mesh{ NULL,NULL,NULL };
	//ステートマシンインスタンス
	StateMachine<Player> m_StateMachine;
	//必殺技
	PlayerAbility m_Ability;
	//スキル
	SkillData m_Skill;
	//アイテム
	ItemData m_Item;
	//ステータス
	PlayerStatus m_Status;
	//アニメーション取得
	PlayerAnim m_Anim;
	//HPバー取得
	Slider* m_HPSlider;
	//エネミー取得
	Actor* m_Enemy{ nullptr };
	// モーション
	GSuint m_Motion;
	// エフェクトハンドル
	GSint m_EffectHandle;
	//無敵時間
	float m_InvincibleTime{ 0.0f };
	//回避距離
	float m_AvoidDistance;
	//ヒットストップタイマー
	float m_HitStopTimer{ 0.0f };
	//スローにする倍率
	float m_SlowMagnification{ 1.0f };
	// フラッシュ時間
	float m_FlashTime{ 60.0f };
	// フラッシュ用のタイマ
	float m_FlashTimer{ m_FlashTime };
	//受けたダメージ
	int m_DamageValue{ 0 };
	//敵の番号
	int m_TargetEnemyNum{ 0 };
	//次の攻撃
	bool m_NextAttackStateFrag{ false };
	//ヒットストップするか
	bool m_IsHitStop{ false };
	//無敵か
	bool m_IsInvincible{ false };
	//回避したか
	bool m_IsAvoidance{ false };
	//回避スキルを使っているか
	bool m_IsHitAvoidCollider{ false };
	//回避スロー状態にする
	bool m_IsJustAvoid{ false };
	//パリィスロー状態にする
	bool m_IsJustAttack{ false };
	//セレクトシーンか
	bool m_IsSelect{ false };
	//攻撃を受けたか
	bool m_IsReact{ false };


};

#endif
