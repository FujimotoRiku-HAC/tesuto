#include "PlayerAbility.h"
#include "AbilityType.h"
#include "Actor/Player/PlayerAnim.h"

PlayerAbility::PlayerAbility() {
	IconSet();
	LoadTexture();
}

void PlayerAbility::Update(IWorld* world,float delta_time){
	//RBを押したら
	if (InputManager::Instance().IsBottonState(InputManager::InputType::SkillSelect)) {
		//アビリティ１使用
		if (InputManager::Instance().IsBottonDown(InputManager::InputType::Ability1)) {
			UseCurrentAbility(world, m_Abilities.at(0));
			m_Alpha_1 = 0.f;
			m_UseAbility1 = true;
		}
		//アビリティ２使用
		if (InputManager::Instance().IsBottonDown(InputManager::InputType::Ability2)) {
			UseCurrentAbility(world, m_Abilities.at(1));
			m_Alpha_2 = 0.f;
			m_UseAbility2 = true;
		}
	}
	//アイコン更新
	IconDraw(delta_time);
	IconDraw2(delta_time);
	//アビリティアニメーション更新
	m_AbilityAnim.Update(delta_time);
}

void PlayerAbility::Draw() const{
	GScolor color = { 1.f,1.f,1.f,m_Alpha_1 };

	if (InputManager::Instance().IsBottonState(InputManager::InputType::SkillSelect)) {

		GSvector2 ability_pos = ItemAndAbilityIconPos;

		for (int i = 0; i < IconNum; ++i) {
			gsDrawSprite2D(m_AbilitiesIcon.at(m_Abilities.at(i)), &ability_pos, NULL, NULL, &color, &ItemAndAbilityIconScale, NULL);
			color.a = m_Alpha_2;
			ability_pos.x -= IconShiftPos.x;
			ability_pos.y -= IconShiftPos.y;
		}
	}
}

void PlayerAbility::AbilityLoad(const char* file_path){
	std::ifstream filepath(file_path);
	std::string str;
	int count = 0;

	while (std::getline(filepath, str)) {
		SetAbility(count, atoi(str.c_str()));
		count++;
	}
	filepath.close();
}

void PlayerAbility::SetAbility(int num, int special){
	m_Abilities.at(num) = special;
}

void PlayerAbility::IconSet(){
	m_AbilitiesIcon[Ability::NoneAbility] = NotIcon_Texture;
	m_AbilitiesIcon[Ability::BlowAttackAbility] = BlowAttack_Ability_UI_Taxture;
	m_AbilitiesIcon[Ability::PushAttackAbility] = PushAttack_Ability_UI_Taxture;
	m_AbilitiesIcon[Ability::AvoidAttackAbility] = AvoidAttack_Ability_UI_Taxture;
	m_AbilitiesIcon[Ability::RotatingAttackAbility] = RotatingAttack_Ability_UI_Taxture;
	m_AbilitiesIcon[Ability::InpactGroundAttackAbility] = InpactGroundAttack_Ability_UI_Taxture;
	m_AbilitiesIcon[Ability::SlamAttackAbility] = SlamAttack_Ability_UI_Taxture;
	m_AbilitiesIcon[Ability::WildDance] = WildDance_UI_Taxture;
	m_AbilitiesIcon[Ability::WaveWildDanceAbility] = WaveWildDance_Ability_UI_Taxture;
	m_AbilitiesIcon[Ability::FangsAbility] = Fangs_Ability_UI_Taxture;
}

void PlayerAbility::IconDraw(float delta_time){
	//アビリティコマンドが押されていなかったら
	if (!m_UseAbility1)return; 
	//アビリティが使えるようになったら不透明解除
	if (!m_AbilityAnim.GetAbility(m_Abilities.at(0))) {
		m_Alpha_1 = 1.f;
		m_UseAbility1 = false;
		return;
	}
	//アルファ値減算
	m_Alpha_1 -= m_Alpha_1 * delta_time;
	m_Alpha_1 = CLAMP(m_Alpha_1, 0.f, m_AbilityAnim.GetTimer(m_Abilities.at(0)));
}

void PlayerAbility::IconDraw2(float delta_time){
	//アビリティコマンドが押されていなかったら
	if (!m_UseAbility2) return;
	//アビリティが使えるようになったら不透明解除
	if (!m_AbilityAnim.GetAbility(m_Abilities.at(1))) {
		m_Alpha_2 = 1.f;
		m_UseAbility2 = false;
		return;
	}
	//アルファ値減算
	m_Alpha_2 -= m_Alpha_2 * delta_time;
	m_Alpha_2 = CLAMP(m_Alpha_2, 0.f, m_AbilityAnim.GetTimer(m_Abilities.at(1)));
}

void PlayerAbility::LoadTexture(){
	gsLoadTexture(BlowAttack_Ability_UI_Taxture,			"Assets/Texture/UI/AbilityUI/blow_attack_ui.png");
	gsLoadTexture(PushAttack_Ability_UI_Taxture,			"Assets/Texture/UI/AbilityUI/push_attack_ui.png");
	gsLoadTexture(AvoidAttack_Ability_UI_Taxture,			"Assets/Texture/UI/AbilityUI/avoid_attack_ui.png");
	gsLoadTexture(RotatingAttack_Ability_UI_Taxture,		"Assets/Texture/UI/AbilityUI/rotating_attack_ui.png");
	gsLoadTexture(InpactGroundAttack_Ability_UI_Taxture,	"Assets/Texture/UI/AbilityUI/inpact_attack_ui.png");
	gsLoadTexture(SlamAttack_Ability_UI_Taxture,			"Assets/Texture/UI/AbilityUI/slam_attack_ui.png");
	gsLoadTexture(WildDance_UI_Taxture,						"Assets/Texture/UI/AbilityUI/wild_dance_ui.png");
	gsLoadTexture(WaveWildDance_Ability_UI_Taxture,			"Assets/Texture/UI/AbilityUI/wave_wild_dance_ui.png");
	gsLoadTexture(Fangs_Ability_UI_Taxture,					"Assets/Texture/UI/AbilityUI/fangs_attack_ui.png");
}

void PlayerAbility::UseCurrentAbility(IWorld* world,int num){
	m_AbilityAnim.UseAbility(world, num);
}
