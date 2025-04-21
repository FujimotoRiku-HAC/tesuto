#include "PlayerStatus.h"

PlayerStatus::PlayerStatus() {
	//最大体力の設定
	m_MaxHealth = DefaultHealth;
	//回復量の初期化
	m_HealthHealPower = DefaultHealPowerHealth;
	//移動速度の初期化
	m_MoveSpeed = DefaultWalkSpeed;
	//基礎攻撃力の初期化
	m_BasicAttackPower = DefaultBasicAttackPower;
	//基礎スタン量の初期化
	m_BasicStanPower = DefaultBasicStanPower;
	//無敵時間の初期化
	m_InvincibleTime = DefaultInvincibleTime;
	//回避距離の初期化
	m_AvoidDistance = DefaultAvoidDistance;
	//フルチャージの攻撃力上昇の設定
	m_FullChargePower = DefaultAttackPower;
	//火事場の攻撃上昇倍率の設定
	m_CrisisChargePower = DefaultAttackPower;
	//攻撃力アップの倍率初期設定
	m_ItemPowerUp = DefaultAttackPower;
	//初期体力の設定
	m_Health = m_MaxHealth;
}

void PlayerStatus::Update(float delta_time){
	if (m_IsFullCharge && IsRemaingValueHealth(FullChargeInvokerate)) {
		m_FullChargePower = FullChargePowerUpValue;
	}
	else {
		m_FullChargePower = DefaultAttackPower;
	}
	if (m_IsCrisisCharge && !IsRemaingValueHealth(CrisisChargeInvokeRate)) {
		m_CrisisChargePower = CrisisChargePowerUpValue;
	}
	else {
		m_CrisisChargePower = DefaultAttackPower;
	}
}

void PlayerStatus::HitDamage(int damage){
	// ダメージ軽減スキルを通してダメージを算出
	if (gsRandf(1.0f, 100.0f) <= 33.3f && m_IsDivineProtection) {
		m_Health -= damage * m_DamageIncreaseMultiplier * 0.66f;
	}
	else {
		m_Health -= damage * m_DamageIncreaseMultiplier * m_DamageGuardIncreaseMultiplier;
	}

	m_Health = CLAMP(m_Health, 0, m_MaxHealth);
}

void PlayerStatus::HealHealth(float heal_health){
	m_Health += heal_health * DefaultHealPowerHealth;
	m_Health = CLAMP(m_Health, 0, m_MaxHealth);
}

void PlayerStatus::AddMaxHealth(float magnification){
	m_MaxHealth *= magnification;
	m_Health = m_MaxHealth;
}

void PlayerStatus::AddHealHealthPower(float magnification){
	m_HealthHealPower *= magnification;
}

void PlayerStatus::AddAttackPower(float adition){
	m_BasicAttackPower += adition;
}

void PlayerStatus::AddDamageIncreaseMultiplier(float magnification){
	m_DamageIncreaseMultiplier *= magnification;
}

void PlayerStatus::AddDamageGuardIncreaseMultiplier(float magnification){
	m_DamageGuardIncreaseMultiplier *= magnification;
}

void PlayerStatus::AddInvicincibleTime(float adition){
	m_InvincibleTime += adition;
}

void PlayerStatus::AddAvoidDistance(float magnification){
	m_AvoidDistance *= magnification;
}

void PlayerStatus::SetMoveSpeed(float magnification){
	m_MoveSpeed *= magnification;
}

void PlayerStatus::SetFullCharge(){
	m_IsFullCharge = true;
}

void PlayerStatus::SetCrisisCharge(){
	m_IsCrisisCharge = true;
}

void PlayerStatus::SetSuperArmor(){
	m_IsSuperArmor = true;
}

void PlayerStatus::SetDivineProtection(){
	m_IsDivineProtection = true;
}

void PlayerStatus::SetAvoidAuxiliary(){
	m_AvoidAuxiliary = true;
}

void PlayerStatus::RateAttackPortion(float magnification){
	m_ItemPowerUp = magnification;
}

void PlayerStatus::EffectWearsAttackPortion(){
	m_ItemPowerUp = DefaultAttackPower;
}

void PlayerStatus::UseAttackPowerUp(bool use){
	m_PowerUpPortion = use;
}

void PlayerStatus::UseHealPortion(bool use){
	m_HealPortion = use;
}

void PlayerStatus::SetAttackRate(){
	m_AttackRate.clear();

	m_AttackRate[AttackType::NormalAttack_1] = NormalAttackMultiplier1;
	m_AttackRate[AttackType::NormalAttack_2] = NormalAttackMultiplier2;
	m_AttackRate[AttackType::NormalAttack_3] = NormalAttackMultiplier3;
	m_AttackRate[AttackType::CounterAttack] = CounterAttackMultiplier;
	m_AttackRate[AttackType::AbilityBlowAttack] = BlowAttackMultiplier;
	m_AttackRate[AttackType::AbilityBlowAttackReinforcement] = BlowReinforcementAttackMultiplier1;
	m_AttackRate[AttackType::AbilityPushUpAttack] = PushUpAttackMultiplier;
	m_AttackRate[AttackType::AbilityAvoidAttack] = AvoidAttackMultiplier;
	m_AttackRate[AttackType::AbilityRotatingAttack] = RotatingNormalAttackMultiplier;
	m_AttackRate[AttackType::AbilityInpactAttack] = InpactAttackMultiplier;
	m_AttackRate[AttackType::AbilityInpactAttack2] = InpactAttackMultiplier2;
	m_AttackRate[AttackType::AbilitySlamAttack] = SlamAttackMultiplier;
	m_AttackRate[AttackType::AbilitySlamAttack2] = SlamAttackMultiplier2;
	m_AttackRate[AttackType::AbilityWildDance_1] = WildDanceMultiplier;
	m_AttackRate[AttackType::AbilityWildDance_2] = WildDanceMultiplier2;
	m_AttackRate[AttackType::AbilityWildDance_3] = WildDanceMultiplier3;
	m_AttackRate[AttackType::AbilityWildDance_4] = WildDanceMultiplier4;
	m_AttackRate[AttackType::AbilityWildDance_5] = WildDanceMultiplier5;
	m_AttackRate[AttackType::AbilityWildDance_6] = WildDanceMultiplier6;
	m_AttackRate[AttackType::AbilityWildDance_7] = WildDanceMultiplier7;
	m_AttackRate[AttackType::AbilityWaveWildDance_1] = WaveWildDanceMultiplier;
	m_AttackRate[AttackType::AbilityWaveWildDance_2] = WaveWildDanceMultiplier2;
	m_AttackRate[AttackType::AbilityWaveWildDance_3] = WaveWildDanceMultiplier3;
	m_AttackRate[AttackType::AbilityFangs] = FangsMultiplier;


}

bool PlayerStatus::IsDead() const{
	return m_Health <= 0;
}

bool PlayerStatus::IsRemaingValueHealth(float ratio) const{
	return m_Health >= m_MaxHealth * ratio;
}

bool PlayerStatus::IsSuperArmar() const{
	return m_IsSuperArmor;
}

bool PlayerStatus::IsAvoidAuxiliary() const{
	return m_AvoidAuxiliary;
}

bool PlayerStatus::IsHealPortion() const{
	return m_HealPortion;
}

bool PlayerStatus::IsIsAttackPowerupPortion() const{
	return m_PowerUpPortion;
}

int PlayerStatus::Health() const{
	return m_Health;
}

int PlayerStatus::MaxHealth() const{
	return m_MaxHealth;
}

int PlayerStatus::AttackPower(AttackType type) const{
	return m_BasicAttackPower * m_FullChargePower * m_CrisisChargePower * m_ItemPowerUp * m_AttackRate.at(type);
}

int PlayerStatus::StanPower(AttackType type) const{
	return m_BasicStanPower * m_FullChargePower * m_CrisisChargePower * m_AttackRate.at(type);
}

float PlayerStatus::MoveSpeed() const{
	return m_MoveSpeed;
}

float PlayerStatus::AvoidDistance() const{
	return m_AvoidDistance;
}

float PlayerStatus::InvincibleTime() const{
	return m_InvincibleTime;
}

