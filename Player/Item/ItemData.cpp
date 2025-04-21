#include "ItemData.h"
#include "ItemType.h"
#include "Number/Numbertexture.h"

const int HaveHealPortion{ 3 };
const int HavePowerUpPortion{ 2 };
const float AlphaSubtraction{ 0.1f };

ItemData::ItemData(){
	IconTexture();
	SetHavingItemNum();
	SetIcon();
}

void ItemData::Update(IWorld* world, float delta_time){

	//攻撃ボタンを押したら
	if (InputManager::Instance().IsBottonDown(InputManager::InputType::Attack)) {
		m_IsAttack = true;
		m_AttackAlpha = 1.f;
	}
	//回避ボタンを押したら
	if (InputManager::Instance().IsBottonDown(InputManager::InputType::Avoid)) {
		m_IsAvoid = true;
		m_AvoidAlpha = 1.f;
	}
	//アイコンの更新
	IconUpdate(delta_time);

	//RBを押していない状態なら
	if (!InputManager::Instance().IsBottonState(InputManager::InputType::SkillSelect)) {

		if (InputManager::Instance().IsBottonDown(InputManager::InputType::Ability1)) {
			m_CurrentItem = 0;
			//アイテムを持っていれば使う
			if (HavingItem()) {
				UseItem(world);
				gsPlaySE(PlayerHeal_SE);
			}
			m_IsItem1 = true;
			m_Item1Alpha = 1.f;
		}
		if (InputManager::Instance().IsBottonDown(InputManager::InputType::Ability2)) {
			m_CurrentItem = 1;
			//アイテムを持っていれば使う
			if (HavingItem()) {
				UseItem(world);
				gsPlaySE(PlayerPowerUp_SE);
			}
			m_IsItem2 = true;
			m_Item2Alpha = 1.f;
		}
		//アイテムの更新
		m_ItemKinds.Update(world, delta_time);
	}
	else {
		//使ったらアルファ値を下げる
		if (InputManager::Instance().IsBottonDown(InputManager::InputType::Ability1)) {
			m_IsItem1 = true;
			m_Item1Alpha = 1.f;
		}
		if (InputManager::Instance().IsBottonDown(InputManager::InputType::Ability2)) {
			m_IsItem2 = true;
			m_Item2Alpha = 1.f;
		}
	}
}

void ItemData::Draw() const{
	//操作説明アイコン表示
	gsDrawSprite2D(Play_Text, &OperationExplanationPos, NULL, NULL, NULL, &OperationExplanationScale, NULL);

	GScolor color = { 1.f,1.f,1.f,1.0f };
	//攻撃・回避アイコン表示
	color.a = m_AttackAlpha;
	GSuint n = AttackIcon_Texture;
	GSvector2 icon_pos = AttackIconPos;
	for (int i = 0; i < IconNum; i++) {
		gsDrawSprite2D(n, &icon_pos, NULL, NULL, &color, &IconScale, NULL);
		icon_pos.x += IconShiftPos.x;
		icon_pos.y += IconShiftPos.y;
		n = AvoidIcon_Texture;
		color.a = m_AvoidAlpha;
	}

	//背景アイコンの表示
	GSvector2 pos = { BackIconPos.x,BackIconPos.y };
	color = { 1.f,1.f,1.f,m_Item1Alpha };
	for (int i = 0; i < IconNum; i++) {
		GSvector2 frame_pos = IconPosition - pos;
		gsDrawSprite2D(IconFrame_Texture, &frame_pos, NULL, NULL, &color, &IconScale, NULL);
		pos.x += IconShiftPos.x;
		pos.y += IconShiftPos.y;
		color.a = m_Item2Alpha;
	}

	if (InputManager::Instance().IsBottonState(InputManager::InputType::SkillSelect)) return;

	//回復の表示
	static const NumberTexture number{ NumText_Texture,16,16 };
	GSvector2 item_pos = ItemAndAbilityIconPos;
	for (int i = 0; i < IconNum; ++i) {
		gsDrawSprite2D(m_ItemIcon.at(i), &item_pos, NULL, NULL, NULL, &ItemAndAbilityIconScale, NULL);
		int num = m_HavingItemNum.at(i);
		number.Draw(item_pos, num, 1);
		item_pos.x -= IconShiftPos.x;
		item_pos.y -= IconShiftPos.y;
	}
}

bool ItemData::HavingItem() const {
	if (m_CurrentItem == 0)return RemainingItem(0);
	else if (m_CurrentItem == 1)return RemainingItem(1);
	return false;
}

bool ItemData::RemainingItem(int type) const {
	return m_HavingItemNum.at(type) > 0;
}

void ItemData::UseItem(IWorld* world){
	DecrementItemNum(m_CurrentItem);
	m_ItemKinds.UseItem(world, m_CurrentItem);
}

void ItemData::DecrementItemNum(int type){
	m_HavingItemNum[type] = m_HavingItemNum[type] - 1;
}

void ItemData::SetIcon(){
	m_ItemIcon[ItemType::HealthHealPortion] = Heal_Portion_Texture;
	m_ItemIcon[ItemType::AttackPowerUpPortion] = Attack_PowerUp_Portion_Texture;
}

void ItemData::SetHavingItemNum(){
	m_HavingItemNum[ItemType::HealthHealPortion] = HaveHealPortion;
	m_HavingItemNum[ItemType::AttackPowerUpPortion] = HavePowerUpPortion;
}

void ItemData::IconTexture(){
	gsLoadTexture(Heal_Portion_Texture,				"Assets/Texture/UI/PlayScene/heal_ui.png");
	gsLoadTexture(Attack_PowerUp_Portion_Texture,	"Assets/Texture/UI/PlayScene/attack_power_up_ui.png");
	gsLoadTexture(NumText_Texture,					"Assets/Texture/UI/PlayScene/num_text.png");
}

void ItemData::AttackDraw(float delta_time){
	//攻撃コマンドが押されていなかったらリターン
	if (!m_IsAttack) return;
	//不透明にして元に戻す
	if (m_AttackAlpha <= 0.f) {
		m_IsAttack = false;
		m_AttackAlpha = 1.f;
		return;
	}
	m_AttackAlpha -= AlphaSubtraction * delta_time;
	m_AttackAlpha = std::max(m_AttackAlpha, 0.f);
}

void ItemData::AvoidDraw(float delta_time){
	//回避コマンドが押されていなかったらリターン
	if (!m_IsAvoid) return;
	//不透明にして元に戻す
	if (m_AvoidAlpha <= 0.f) {
		m_IsAvoid = false;
		m_AvoidAlpha = 1.f;
		return;
	}
	m_AvoidAlpha -= AlphaSubtraction * delta_time;
	m_AvoidAlpha = std::max(m_AvoidAlpha, 0.0f);
}

void ItemData::Item1Draw(float delta_time){
	//回避コマンドが押されていなかったらリターン
	if (!m_IsItem1) return;
	//不透明にして元に戻す
	if (m_Item1Alpha <= 0.f) {
		m_IsItem1 = false;
		m_Item1Alpha = 1.f;
		return;
	}
	m_Item1Alpha -= AlphaSubtraction * delta_time;
	m_Item1Alpha = std::max(m_Item1Alpha, 0.0f);
}

void ItemData::Item2Draw(float delta_time)
{
	//回避コマンドが押されていなかったらリターン
	if (!m_IsItem2) return;
	//不透明にして元に戻す
	if (m_Item2Alpha <= 0.f) {
		m_IsItem2 = false;
		m_Item2Alpha = 1.f;
		return;
	}
	m_Item2Alpha -= AlphaSubtraction * delta_time;
	m_Item2Alpha = std::max(m_Item2Alpha, 0.0f);
}

void ItemData::IconUpdate(float delta_time){
	AvoidDraw(delta_time);
	AttackDraw(delta_time);
	Item1Draw(delta_time);
	Item2Draw(delta_time);
}
