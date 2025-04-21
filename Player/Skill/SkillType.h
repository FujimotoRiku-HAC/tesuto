#ifndef SKILL_TYPE_H_
#define SKILL_TYPE_H_

enum SkillType {
	NoneSkill,
	HealthUpAndHealSkill,							//上限解放＋回復
	AttackPowerUpSkill,								//攻撃力1.5倍上昇
	AttackPowerLergeUpSkill,						//攻撃力2.0倍上昇
	HealthHealSeedUpSkill,							//体力回復量上昇
	AvoidPerformanceUpSkill,						//回避性能上昇
	AvoidDistanceUpSkill,							//回避距離上昇
	JustAvoidSlowSkill,								//ジャスト回避補助
	DivineProtectionSkill,							//加護スキル
	FullChargeSkill,								//フルチャージ
	CrisisChargeSkill,								//火事場
	SuperArmorSkill,								//体感強化
	TotalSkillNum
};

#endif