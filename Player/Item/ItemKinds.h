#ifndef ITEM_KINDS_H_
#define ITEM_KINDS_H_

class ItemKinds {
public:

	//アイテム使用
	void UseItem(IWorld* world, int item);
	//更新
	void Update(IWorld* world, float delta_time);

private:

	//回復ポーションの更新
	void HealPortionUpdate(IWorld* world);
	//攻撃力アップポーションの更新
	void AttackPortionUpdate(IWorld* world, float delta_time);

private:

	bool m_IsUseAttackPortion = false;	//攻撃ポーション使用したか
	bool m_IsUseHealPortion = false;	//ヒールポーションを使用したか
	float m_AttackPortionTimer = 0.0f;	//ポーションタイマー

};

#endif