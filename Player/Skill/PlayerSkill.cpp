#include "PlayerSkill.h"
#include "Actor/Player/Player.h"
#include "SkillType.h"

void PlayerSkill::UseSkill(Player* player, int skill){
	switch (skill)
	{
	case SkillType::HealthUpAndHealSkill:
		player->Status().AddMaxHealth(HealthUpValue);
		break;
	case SkillType::AttackPowerUpSkill:
		player->Status().AddAttackPower(AttackPowerUpValue);
		break;
	case SkillType::AttackPowerLergeUpSkill:
		player->Status().AddAttackPower(AttackPowerLergeUpValue);
		player->Status().SetMoveSpeed(DecreaseMoveSpeed);
		break;
	case SkillType::HealthHealSeedUpSkill:
		player->Status().AddHealHealthPower(HealthHealPowerUpValue);
		break;
	case SkillType::AvoidPerformanceUpSkill:
		player->Status().AddInvicincibleTime(InvincibleTime);
		player->Status().AddDamageIncreaseMultiplier(DamageIncreaseMultiplier);
		break;
	case SkillType::AvoidDistanceUpSkill:
		player->Status().AddAvoidDistance(AvoidDistance);
		player->Status().AddDamageIncreaseMultiplier(DamageIncreaseMultiplier);
		break;
	case SkillType::JustAvoidSlowSkill:
		player->Status().SetAvoidAuxiliary();
		player->Status().AddDamageIncreaseMultiplier(DamageIncreaseMultiplier);
		break;
	case SkillType::DivineProtectionSkill:
		player->Status().SetDivineProtection();
		break;
	case SkillType::FullChargeSkill:
		player->Status().SetFullCharge();
		break;
	case SkillType::CrisisChargeSkill:
		player->Status().SetCrisisCharge();
		break;
	case SkillType::SuperArmorSkill:
		player->Status().SetSuperArmor();
		player->Status().AddDamageIncreaseMultiplier(DamageIncreaseMultiplier);
		break;
	default:
		break;
	}
}
