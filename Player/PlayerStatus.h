#ifndef PLAYER_STATUS_H_
#define PLAYER_STATUS_H_

class PlayerStatus {
public:
	enum AttackType {
		NormalAttack_1,					//一攻撃
		NormalAttack_2,					//二撃目
		NormalAttack_3,					//三撃目
		CounterAttack,					//カウンター
		AbilityBlowAttack,				//ブロー攻撃
		AbilityBlowAttackReinforcement,	//ブロー攻撃2撃目
		AbilityPushUpAttack,			//プッシュ攻撃
		AbilityAvoidAttack,				//回避攻撃
		AbilityRotatingAttack,			//回転斬り
		AbilityInpactAttack,			//インパクト攻撃
		AbilityInpactAttack2,			//インパクト二撃目
		AbilitySlamAttack,				//スラム攻撃
		AbilitySlamAttack2,				//スラム攻撃二撃目
		AbilityWildDance_1,				//乱舞１
		AbilityWildDance_2,				//乱舞２
		AbilityWildDance_3,				//乱舞３
		AbilityWildDance_4,				//乱舞４
		AbilityWildDance_5,				//乱舞５
		AbilityWildDance_6,				//乱舞６
		AbilityWildDance_7,				//乱舞７
		AbilityWaveWildDance_1,			//波動乱舞
		AbilityWaveWildDance_2,			//波動乱舞２
		AbilityWaveWildDance_3,			//波動乱舞３
		AbilityFangs					//牙突
	};
public:
	PlayerStatus();
	//更新
	void Update(float delta_time);
	//ダメージ設定
	void HitDamage(int damage);
	//体力の回復
	void HealHealth(float heal_health);
	// 最大体力の追加(乗算)
	void AddMaxHealth(float magnification);
	//体力回復量の追加
	void AddHealHealthPower(float magnification);
	//攻撃力の追加(加算させる)
	void AddAttackPower(float adition);
	//攻撃力にかける倍率の追加(乗算させる)
	void AddDamageIncreaseMultiplier(float magnification);
	//ガード状態
	void AddDamageGuardIncreaseMultiplier(float magnification);
	//無敵時間の追加(加算)
	void AddInvicincibleTime(float adition);
	//回避距離の追加(乗算)
	void AddAvoidDistance(float magnification);
	//移動速度の追加(乗算)
	void SetMoveSpeed(float magnification);
	//フルチャージか
	void SetFullCharge();
	//火事場か
	void SetCrisisCharge();
	//スーパーアーマーか
	void SetSuperArmor();
	//加護スキルを使うか
	void SetDivineProtection();
	//回避補助するか
	void SetAvoidAuxiliary();
	
	//攻撃力ポーションの攻撃力倍率の設定
	void RateAttackPortion(float magnification);
	//攻撃力ポーションの効果が切れた
	void EffectWearsAttackPortion();
	//攻撃力アップを使った
	void UseAttackPowerUp(bool use);

	//
	void UseHealPortion(bool use);
	//各攻撃の攻撃倍率
	void SetAttackRate();

public:
	//死んだか
	bool IsDead()const;
	//指定した体力が残っているか
	bool IsRemaingValueHealth(float ratio)const;
	//スーパーアーマー使っているか
	bool IsSuperArmar()const;
	//回避補助を使っているか
	bool IsAvoidAuxiliary()const;
	//回復ポーションを使ったか
	bool IsHealPortion()const;
	//攻撃力アップを使ったか
	bool IsIsAttackPowerupPortion()const;
public:
	//現在の体力取得
	int Health()const;
	//最大体力の取得
	int MaxHealth()const;
	//攻撃力の取得
	int AttackPower(AttackType type)const;
	//スタン量の取得
	int StanPower(AttackType type)const;
	//移動速度
	float MoveSpeed()const;
	//回避距離取得
	float AvoidDistance()const;
	//無敵時間取得
	float InvincibleTime()const;

private:

	int   m_Health;
	int   m_MaxHealth;
	int   m_HealthHealPower;
	float m_DamageIncreaseMultiplier = 1.0f;
	float m_DamageGuardIncreaseMultiplier = 1.0f;

	float m_InvincibleTime;
	float m_AvoidDistance;
	float m_BasicAttackPower;
	float m_BasicStanPower;
	float m_MoveSpeed;
	float m_FullChargePower;
	float m_CrisisChargePower;
	float m_ItemPowerUp;
	bool  m_AvoidAuxiliary = false;
	bool  m_Burning = false;
	bool  m_IsFullCharge = false;
	bool  m_IsCrisisCharge = false;
	bool  m_IsSuperArmor = false;
	bool  m_IsDivineProtection = false;
	bool m_HealPortion = false;
	bool m_PowerUpPortion = false;
	std::unordered_map<AttackType, float> m_AttackRate;
};

#endif