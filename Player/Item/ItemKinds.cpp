#include "ItemKinds.h"
#include "ItemType.h"
#include "Actor/player/Player.h"

void ItemKinds::UseItem(IWorld* world, int item){
	auto player = static_cast<Player*>(world->FindActor(st_Player));

	switch (item){
	case HealthHealPortion:			//ヒールポーション
		player->Status().HealHealth(ItemHealthValue);
		player->Status().UseHealPortion(true);
		m_IsUseHealPortion = true;
		break;
	case AttackPowerUpPortion:		//攻撃力アップポーション
		player->Status().RateAttackPortion(ItemAttackPowerUpValue);
		player->Status().UseAttackPowerUp(true);
		m_AttackPortionTimer = ItemAttackPowerUpTime;
		m_IsUseAttackPortion = true;
		break;
	default:
		break;
	}
}

void ItemKinds::Update(IWorld* world, float delta_time){
	AttackPortionUpdate(world,delta_time);
	HealPortionUpdate(world);
}

void ItemKinds::HealPortionUpdate(IWorld* world){
	if (!m_IsUseHealPortion)return;
	//プレイヤー検索
	auto player = static_cast<Player*>(world->FindActor(st_Player));
	m_IsUseHealPortion = false;

}

void ItemKinds::AttackPortionUpdate(IWorld* world, float delta_time){
	if (!m_IsUseAttackPortion)return;

	m_AttackPortionTimer -= delta_time;
	//効果が切れたら
	if (m_AttackPortionTimer <= 0.f) {
		//プレイヤー検索
		auto player = static_cast<Player*>(world->FindActor(st_Player));

		m_IsUseAttackPortion = false;
		//攻撃力を元に戻す
		player->Status().EffectWearsAttackPortion();
		player->Status().UseAttackPowerUp(false);
	}
}
