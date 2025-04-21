#ifndef ITEM_DATA_H_
#define ITEM_DATA_H_

#include "ItemKinds.h"

class ItemData {
public:

	//コンストラクタ
	ItemData();
	//更新
	void Update(IWorld* world,float delta_time);
	//描画
	void Draw()const;

private:

	//アイテムが残っているか
	bool RemainingItem(int type)const;
	//指定のアイテムを持っているか
	bool HavingItem()const;

private:

	//アイテム使用
	void UseItem(IWorld* world);
	//使ったアイテムを減らす
	void DecrementItemNum(int type);
	//アイコンセット
	void SetIcon();
	//アイテムの個数設定
	void SetHavingItemNum();
	//アイコンロード
	void IconTexture();
	//攻撃ボタンを押したら
	void AttackDraw(float delta_time);
	//回避ボタンを押したら
	void AvoidDraw(float delta_time);
	//アイテムボタンを押したら
	void Item1Draw(float delta_time);
	void Item2Draw(float delta_time);
	//アイコンの更新
	void IconUpdate(float delta_time);

private:

	//何アイテムか
	std::unordered_map<int, int>m_HavingItemNum;
	//アイテム種類
	ItemKinds m_ItemKinds;
	//アイテムアイコン
	std::unordered_map<int, GSuint>m_ItemIcon;

	//ボタンを押したか
	bool m_IsAttack{ false };
	bool m_IsAvoid{ false };
	bool m_IsItem1{ false };
	bool m_IsItem2{ false };

	//アイコンのアルファ値
	float m_AttackAlpha{ 1.0f };
	float m_AvoidAlpha{ 1.0f };
	float m_Item1Alpha{ 1.0f };
	float m_Item2Alpha{ 1.0f };
	//現在のアイテム番号
	int m_CurrentItem = 0;
};

#endif