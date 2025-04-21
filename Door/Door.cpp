#include "Door.h"

Door::Door(IWorld* world,const GSvector3& position,GSuint mesh,GSuint collider){
	//ワールド設定
	m_World = world;
	//タグ設定
	m_Tag = st_DoorTag;
	//名前設定
	m_Name = st_Door;
	//初期位置
	m_Transform.position(position);
	//メッシュ設定
	m_Mesh = mesh;
	//コライダ―設定
	m_MeshCollider = collider;
}

//更新
void Door::Update(float delta_time) {
	if (m_IsOpen) {
		//カラー値が0になったら死亡
		if (m_ColorTimer <= 0.f) {
			Die();
			m_World->GetCamera()->HandleMessage(st_Message_Die,nullptr);
			return;
		}
		//段々透明にしていく
		m_ColorTimer = std::max(m_ColorTimer - DoorColorTime * delta_time, 0.f);
	}
}

void Door::Draw() const{
	//ディザ半透明の設定取得
	float transparency = gsGetDitheredTransparency();
	// 現在の乗算カラーを取得（退避しておく）
	GScolor current_color;
	glGetFloatv(GL_CURRENT_COLOR, current_color);
	// 現在の加算カラーの取得（退避しておく）
	GScolor current_secondary_color;
	glGetFloatv(GL_CURRENT_SECONDARY_COLOR, current_secondary_color);
	//半透明ディザ
	Transparency();

	//メッシュ描画
	glPushMatrix();
	glMultMatrixf(m_Transform.localToWorldMatrix());
	gsDrawMesh(m_Mesh);
	glPopMatrix();

	//ディザを復帰
	gsSetDitheredTransparency(transparency);
	// 乗算カラーを復帰する
	glColor4fv(current_color);
	// 加算カラーを復帰する
	glSecondaryColor3fv(current_secondary_color);
}



void Door::HandleMessage(const std::string& message, void* param){
	if (message == st_Message_IsOpen) m_IsOpen = true;
}

void Door::Transparency() const{
	// 補間値(0.0～1.0)を求める
	float t = LERP(m_ColorTimer, m_ColorTime, 1.f);
	
	gsSetDitheredTransparency(gsEasingInExpo(t));
}



