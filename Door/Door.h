#ifndef DOOR_H_
#define DOOR_H_

#include "Field/FieldActor.h"

class Door : public FieldActor {
public:

	//コンストラクタ
	Door(IWorld* world,const GSvector3& position,GSuint mesh,GSuint collider);
	//更新
	virtual void Update(float delta_time)override;
	//描画
	virtual void Draw() const override;
	//メッセージ送信
	virtual void HandleMessage(const std::string& message, void* param)override;

private:

	//半透明ディザ
	void Transparency() const;

private:
	bool m_IsOpen = false;		//開いたか
	float m_ColorTime{ 0.01f };	//カラータイム
	float m_ColorTimer{ 1.0f };	//カラータイマー
};

#endif