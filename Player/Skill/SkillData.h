#ifndef SKILL_DATA_H_
#define SKILL_DATA_H_

#include "PlayerSkill.h"

class SkillData {
public:

	//コンストラクタ
	SkillData();
	//描画
	void Draw()const;
	//ロード
	void SkillLoad(const char* file_path);
	//保持しているスキルの使用
	void Excute(Player* player);

private:

	//スキルの設定
	void SetSkill(int num, int skill);
	//アイコンセット
	void SetIcon();
	//アイコンロード
	void IconTexture();

private:
	//スキル
	PlayerSkill m_PlayerSkill;
	//スキル数
	std::array<int, 3>m_Skills;
	//スキルのアイコン
	std::unordered_map<int, GSuint>m_SkillsIcon;
	//セレクト数
	int m_SelectCount;

};

#endif