#ifndef PLAYER_ABILITY_H_
#define PLAYER_ABILITY_H_

#include "AbilityType.h"
#include "AbilityAnimation.h"

class PlayerAbility {
public:

	//コンストラクタ
	PlayerAbility();
	//更新
	void Update(IWorld* world, float delta_time);
	//描画
	void Draw()const;
	//ロード
	void AbilityLoad(const char* file_path);

private:

	//必殺技を使ったら
	void UseCurrentAbility(IWorld* world, int num);
	//何の必殺技を選択したか
	void SetAbility(int num, int special);
	//アイコンの設定
	void IconSet();
	//アビリティ使ったら
	void IconDraw(float delta_time);
	void IconDraw2(float delta_time);
	//ロード
	void LoadTexture();

private:

	//アビリティクラス
	AbilityAnimation m_AbilityAnim;
	//アビリティ
	std::array<int, 2>m_Abilities;
	//アビリティアイコン
	std::unordered_map<int, GSuint>m_AbilitiesIcon;

	//現在のアビリティ
	int m_CurrentAbility = 0;
	//それぞれのアルファ値
	float m_Alpha_1{ 1.0f };
	float m_Alpha_2{ 1.0f };
	//どのスキルを使用したか
	bool m_UseAbility1{ false };
	bool m_UseAbility2{ false };
};


#endif