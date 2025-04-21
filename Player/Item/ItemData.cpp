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

	//�U���{�^������������
	if (InputManager::Instance().IsBottonDown(InputManager::InputType::Attack)) {
		m_IsAttack = true;
		m_AttackAlpha = 1.f;
	}
	//����{�^������������
	if (InputManager::Instance().IsBottonDown(InputManager::InputType::Avoid)) {
		m_IsAvoid = true;
		m_AvoidAlpha = 1.f;
	}
	//�A�C�R���̍X�V
	IconUpdate(delta_time);

	//RB�������Ă��Ȃ���ԂȂ�
	if (!InputManager::Instance().IsBottonState(InputManager::InputType::SkillSelect)) {

		if (InputManager::Instance().IsBottonDown(InputManager::InputType::Ability1)) {
			m_CurrentItem = 0;
			//�A�C�e���������Ă���Ύg��
			if (HavingItem()) {
				UseItem(world);
				gsPlaySE(PlayerHeal_SE);
			}
			m_IsItem1 = true;
			m_Item1Alpha = 1.f;
		}
		if (InputManager::Instance().IsBottonDown(InputManager::InputType::Ability2)) {
			m_CurrentItem = 1;
			//�A�C�e���������Ă���Ύg��
			if (HavingItem()) {
				UseItem(world);
				gsPlaySE(PlayerPowerUp_SE);
			}
			m_IsItem2 = true;
			m_Item2Alpha = 1.f;
		}
		//�A�C�e���̍X�V
		m_ItemKinds.Update(world, delta_time);
	}
	else {
		//�g������A���t�@�l��������
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
	//��������A�C�R���\��
	gsDrawSprite2D(Play_Text, &OperationExplanationPos, NULL, NULL, NULL, &OperationExplanationScale, NULL);

	GScolor color = { 1.f,1.f,1.f,1.0f };
	//�U���E����A�C�R���\��
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

	//�w�i�A�C�R���̕\��
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

	//�񕜂̕\��
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
	//�U���R�}���h��������Ă��Ȃ������烊�^�[��
	if (!m_IsAttack) return;
	//�s�����ɂ��Č��ɖ߂�
	if (m_AttackAlpha <= 0.f) {
		m_IsAttack = false;
		m_AttackAlpha = 1.f;
		return;
	}
	m_AttackAlpha -= AlphaSubtraction * delta_time;
	m_AttackAlpha = std::max(m_AttackAlpha, 0.f);
}

void ItemData::AvoidDraw(float delta_time){
	//����R�}���h��������Ă��Ȃ������烊�^�[��
	if (!m_IsAvoid) return;
	//�s�����ɂ��Č��ɖ߂�
	if (m_AvoidAlpha <= 0.f) {
		m_IsAvoid = false;
		m_AvoidAlpha = 1.f;
		return;
	}
	m_AvoidAlpha -= AlphaSubtraction * delta_time;
	m_AvoidAlpha = std::max(m_AvoidAlpha, 0.0f);
}

void ItemData::Item1Draw(float delta_time){
	//����R�}���h��������Ă��Ȃ������烊�^�[��
	if (!m_IsItem1) return;
	//�s�����ɂ��Č��ɖ߂�
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
	//����R�}���h��������Ă��Ȃ������烊�^�[��
	if (!m_IsItem2) return;
	//�s�����ɂ��Č��ɖ߂�
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
