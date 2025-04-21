#include "Door.h"

Door::Door(IWorld* world,const GSvector3& position,GSuint mesh,GSuint collider){
	//���[���h�ݒ�
	m_World = world;
	//�^�O�ݒ�
	m_Tag = st_DoorTag;
	//���O�ݒ�
	m_Name = st_Door;
	//�����ʒu
	m_Transform.position(position);
	//���b�V���ݒ�
	m_Mesh = mesh;
	//�R���C�_�\�ݒ�
	m_MeshCollider = collider;
}

//�X�V
void Door::Update(float delta_time) {
	if (m_IsOpen) {
		//�J���[�l��0�ɂȂ����玀�S
		if (m_ColorTimer <= 0.f) {
			Die();
			m_World->GetCamera()->HandleMessage(st_Message_Die,nullptr);
			return;
		}
		//�i�X�����ɂ��Ă���
		m_ColorTimer = std::max(m_ColorTimer - DoorColorTime * delta_time, 0.f);
	}
}

void Door::Draw() const{
	//�f�B�U�������̐ݒ�擾
	float transparency = gsGetDitheredTransparency();
	// ���݂̏�Z�J���[���擾�i�ޔ����Ă����j
	GScolor current_color;
	glGetFloatv(GL_CURRENT_COLOR, current_color);
	// ���݂̉��Z�J���[�̎擾�i�ޔ����Ă����j
	GScolor current_secondary_color;
	glGetFloatv(GL_CURRENT_SECONDARY_COLOR, current_secondary_color);
	//�������f�B�U
	Transparency();

	//���b�V���`��
	glPushMatrix();
	glMultMatrixf(m_Transform.localToWorldMatrix());
	gsDrawMesh(m_Mesh);
	glPopMatrix();

	//�f�B�U�𕜋A
	gsSetDitheredTransparency(transparency);
	// ��Z�J���[�𕜋A����
	glColor4fv(current_color);
	// ���Z�J���[�𕜋A����
	glSecondaryColor3fv(current_secondary_color);
}



void Door::HandleMessage(const std::string& message, void* param){
	if (message == st_Message_IsOpen) m_IsOpen = true;
}

void Door::Transparency() const{
	// ��Ԓl(0.0�`1.0)�����߂�
	float t = LERP(m_ColorTimer, m_ColorTime, 1.f);
	
	gsSetDitheredTransparency(gsEasingInExpo(t));
}



