#ifndef ENEMY_H_
#define ENEMY_H_

#include "Actor/Actor.h"
#include "Animation/AnimatedMesh.h"
#include "State/StateMachine.h"
#include "UI/Slider.h"

class Enemy : public Actor {
public:

	//コンストラクタ
	Enemy() = default;
	//デストラクタ
	~Enemy() = default;
	//更新
	void Update(float delta_time)override;
	//描画
	void Draw()const override;
	//GUI描画
	void DrawGui()const;
	//リアクション
	void React(Actor& other)override;
	//メッセージ
	void HandleMessage(const std::string& message, void* param)override;

public:

	// プレイヤーの取得	
	Actor* GetPlayer()const;

public:

	// ターゲット方向の角度を求める
	float TargetSignedAngle()const;
	//ターゲット方向の角度を求める(符号あり)
	float TargetSignedAngle(GSvector3 target)const;
	//ターゲット方向の角度を求める(符号なし)
	float TargetAngle() const;
	// ターゲットの距離を求める
	float TargetDistance()const;
	//ターゲットの距離を求める
	float TargetDistance(GSvector3 target) const;
	//移動速度取得
	float Speed()const;

public:

	//体力取得
	int GetHP()const;
	//最大HP取得
	int GetMaxHP()const;
	//スタンポイント取得
	int GetSP()const;
	//最大スタンポイント取得
	int GetMaxSP()const;

public:

	//アニメーションタイマ―取得
	bool GetAnimationTimer(float num, float num2 = 0.0f)const;
	//モーション終了か
	bool IsMotionEnd(float subtract = 1.0f)const;
	//プレイヤーがいれば
	bool IsPlayer()const;
	//チケットを持っているか
	bool IsTicket()const;
	//アタックするか
	bool IsAttack()const;
	//アタックしたか
	bool IsAttacked()const;
	//死亡したか
	bool IsDead()const;
	//スタンしたか
	bool IsStan()const;
	//攻撃インターバル中か
	bool IsInterval()const;
	//ダメージを受けたか
	bool IsDamage()const;
	//攻撃するか(ボス)
	bool CanAttack(const float can_attack_distance)const;
	//攻撃するか(ザコ敵)
	bool CanSoldierAttack(const float can_attack_distance, const float can_attack_distance2 = 0.0f)const;
	//カメラ内か
	bool IsInErea()const;

public:

	//モーション変更
	void ChangeMotion(GSuint motion, bool loop, float speed = 1.0f);
	//移動量加算
	void ChangeVelocity(GSvector3 vec);
	//空中に行く
	void Velocity_Y(float fly_velocity);
	//移動量初期化
	void ResetVelocity();
	//攻撃インターバルの設定
	void ResetInterval(float interval_time = 0.0f);
	//チケットを持つか
	void HaveTicket(bool have);
	//攻撃を始めるか
	void AttackStart(bool finish);
	//攻撃をしているか
	void AttackingCheck(bool attacking);
	//死亡したか
	void DeadCheck(bool died);
	//スタンしたか
	void StanCheck(bool stan);
	//重力使うか使わないか
	void ToUseOrNotUseGravity(bool gravity);
	//ダメージ状態のリセット
	void ResetDamageCondition();
	//攻撃インターバルの減算
	void DecrementInterval(float delta_time);
	//死亡した際に消えていく
	void ExtinctionDie(float delta_time);
	//出現時間
	void GenerateTimer(float delta_time);
	//リムカラーの設定
	void DrawHitColorTime(float delta_time);
	//ヒットストップの詳細
	void HitStop(float time, float scale);
	//リムカラー
	void DrawRimColor()const;
	//ヒット時の描画
	void DrawHitRimColor()const;
	//攻撃ができるならカラーを変える
	void AttackRimColor()const;
	//ヒットストップ処理
	void DecentHitStop();
	//消していく
	void DeleteTransparency()const;
	//出現時の半透明ディザ
	void GenerateTransparency()const;
	//ジャスト回避時のスロー
	void DecentSlowDown();
	//ダメージ描画
	void DrawDamage(const GSvector3& pos, const int damage, const float timer = 30.0f);
	//ターゲット画像描画
	void DrawTarget();
	//フィールドとの衝突判定
	void ColliderField();
	//アクターとの衝突判定
	void ColliderActor(Actor& other);
	//敵共通の更新処理
	void EnemyUpdate(float delta_time);
	//メッシュ描画
	void MeshDraw()const;

public:

	//攻撃判定の生成
	void GenerateAttackCollider(
		std::string name,			//攻撃タグ
		int damage,					//ダメージ量
		float distance = 1.5f,		//位置
		float life_span = 10.0f,	//寿命
		float radius = 1.0f,		//半径
		float height = 1.0f			//高さ
		);	

protected:
	//アニメーションメッシュ
	AnimatedMesh m_Mesh{ NULL,NULL,NULL };
	//プレイヤー
	Actor* m_Player{ nullptr };
	//HPゲージ
	Slider m_HPSlider{ GSvector2::zero(),GSvector2::one(),0,1 };
	//スタンゲージ
	Slider m_SPSlider{ GSvector2::zero(),GSvector2::one(),0,1 };
	//攻撃を受けたか
	bool m_IsDamage{ false };
	//重力をかけるか
	bool m_UseGravity{ true };
	//出現した
	bool m_IsGenerate{ false };
	//ヒットストップ
	bool m_IsHitStop{ false };
	//チケット処理
	bool m_IsTicket{ false };
	//攻撃するか
	bool m_IsAttacking{ false };
	//攻撃したか
	bool m_IsAttacked{ false };
	//死亡するか
	bool m_IsDead{ false };
	//スタンしたか
	bool m_IsStan{ false };
	//攻撃のインターバル
	float m_AttackInterval{ 0.0f };
	//遅くする倍率
	float m_SlowMagnification{ 1.0f };
	//移動速度
	float m_Speed{ 0.01f };
	//消滅時間
	float m_ExtinctionTimer{ 5.0f };
	//ヒットストップタイマー
	float m_HitStopTimer{ 0.0f };
	// フラッシュ時間
	float m_FlashTime{ 20.0f };
	// フラッシュ用のタイマ
	float m_FlashTimer{ m_FlashTime };
	//元に戻るまで
	float m_ColorTimer{ 0.0f };
	//HP
	int m_Health{ 10 };
	//最大HP
	int m_MaxHealth{ 10 };
	//SP
	int m_StanPoint{ 0 };
	//最大SP
	int m_MaxStanPoint{ 10 };
};

#endif