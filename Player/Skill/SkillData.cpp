#include "SkillData.h"
#include "SkillType.h"

SkillData::SkillData(){
	IconTexture();
	SetIcon();
}

void SkillData::Draw() const{
	GSvector2 skill_pos{ SkillDrawPos.x,SkillDrawPos.y };
	for (int i = 0; i < static_cast<int>(m_Skills.size()); ++i) {
		gsDrawSprite2D(m_SkillsIcon.at(m_Skills.at(i)), &skill_pos, NULL, NULL, NULL, &SkillScale, 0.0f);
		skill_pos.x += SkillPosShift;
	}
}

void SkillData::SkillLoad(const char* file_path){
	std::ifstream filepath(file_path);
	std::string str;
	int count = 0;

	while (std::getline(filepath, str)) {
		//スキルの使用
		SetSkill(count, atoi(str.c_str()));
		count++;
	}
	filepath.close();
}

void SkillData::SetSkill(int num, int skill){
	m_Skills.at(num) = skill;
}

void SkillData::Excute(Player* player){
	for (int i = 0; i < static_cast<int>(m_Skills.size()); ++i) {
		//常にスキル使用
		if (m_Skills.at(i != SkillType::NoneSkill)) {
			m_PlayerSkill.UseSkill(player, m_Skills.at(i));
		}
	}
}

void SkillData::SetIcon(){
	m_SkillsIcon[SkillType::NoneSkill] = NotIconFrame_Texture;
	m_SkillsIcon[SkillType::HealthUpAndHealSkill] = HealthUpAndHeal_Skill_UI_Texture;;
	m_SkillsIcon[SkillType::AttackPowerUpSkill] = AttackPowerUp_Skill_UI_Texture;
	m_SkillsIcon[SkillType::AttackPowerLergeUpSkill] = AttackPowerLergeUp_Skill_UI_Texture;
	m_SkillsIcon[SkillType::HealthHealSeedUpSkill] = HealthHealSeedUp_Skill_UI_Texture;
	m_SkillsIcon[SkillType::AvoidPerformanceUpSkill] = AvoidPerformanceUp_Skill_UI_Texture;
	m_SkillsIcon[SkillType::AvoidDistanceUpSkill] = AvoidDistanceUp_Skill_UI_Texture;
	m_SkillsIcon[SkillType::JustAvoidSlowSkill] = JustAvoidSlow_Skill_UI_Texture;
	m_SkillsIcon[SkillType::DivineProtectionSkill] = DivineProtection_Skill_UI_Texture;
	m_SkillsIcon[SkillType::FullChargeSkill] = FullCharge_Skill_UI_Texture;
	m_SkillsIcon[SkillType::CrisisChargeSkill] = CrisisCharge_Skill_UI_Texture;
	m_SkillsIcon[SkillType::SuperArmorSkill] = SuperArmor_Skill_UI_Texture;
}

void SkillData::IconTexture(){
	gsLoadTexture(NotIconFrame_Texture,					"Assets/Texture/UI/SkillUI/skill_slot.png");
	gsLoadTexture(HealthUpAndHeal_Skill_UI_Texture,		"Assets/Texture/UI/SkillUI/heal_skill_ui.png");
	gsLoadTexture(AttackPowerUp_Skill_UI_Texture,		"Assets/Texture/UI/SkillUI/attack_power_up_skill_ui.png");
	gsLoadTexture(AttackPowerLergeUp_Skill_UI_Texture,	"Assets/Texture/UI/SkillUI/attack_power_lerge_up_skill_ui.png");
	gsLoadTexture(HealthHealSeedUp_Skill_UI_Texture,	"Assets/Texture/UI/SkillUI/heal_seed_up_skill_ui.png");
	gsLoadTexture(AvoidPerformanceUp_Skill_UI_Texture,	"Assets/Texture/UI/SkillUI/avoid_performance_up_skill_ui.png");
	gsLoadTexture(AvoidDistanceUp_Skill_UI_Texture,		"Assets/Texture/UI/SkillUI/avoid_distance_skill_ui.png");
	gsLoadTexture(JustAvoidSlow_Skill_UI_Texture,		"Assets/Texture/UI/SkillUI/just_avoid_slow_skill_ui.png");
	gsLoadTexture(DivineProtection_Skill_UI_Texture,	"Assets/Texture/UI/SkillUI/divine_protection_skill_ui.png");
	gsLoadTexture(FullCharge_Skill_UI_Texture,			"Assets/Texture/UI/SkillUI/full_charge_skill_ui.png");
	gsLoadTexture(CrisisCharge_Skill_UI_Texture,		"Assets/Texture/UI/SkillUI/crisis_scharge_skill_ui.png");
	gsLoadTexture(SuperArmor_Skill_UI_Texture,			"Assets/Texture/UI/SkillUI/super_armor_skill_ui.png");
}
