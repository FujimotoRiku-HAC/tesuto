#include "AnimatedMesh.h"

//��ԃt���[����
const GSfloat LerpTime{ 10.0f };

const float Magnification{ 0.1f };

AnimatedMesh::AnimatedMesh(GSuint id, GSuint motion, bool loop, GSuint num_bones) :
	m_Id{ id },
	m_Motion{motion},
	m_MotionTimer{0.f},
	m_PrevTimer{0.f},
	m_MotionLoop{loop},
	m_Transform{GS_MATRIX4_IDENTITY},
	m_PrevMotion{motion},
	m_PrevMotionTimer{0.f},
	m_LerpTimer{0.f},
	m_LocalBoneMatrices{ num_bones },
	m_BoneMatrices{ num_bones },
	m_Speed{ 1.f } {
}

void AnimatedMesh::Update(float delta_time) {
	//�X�V�O�̃^�C�}�[�l������Ă���
	m_PrevTimer = m_MotionTimer;
	//�A�j���[�V�����^�C�}�̍X�V
	m_MotionTimer += m_Speed * delta_time;

	if (m_MotionLoop) {
		//���[�V�����^�C�}�����[�v������
		m_MotionTimer = std::fmod(m_MotionTimer, MotionEndTime());
	}
	else {

		m_MotionTimer = std::min(m_MotionTimer, MotionEndTime() - 1.0f);
	}

	m_LerpTimer = std::min(m_LerpTimer + (m_Speed * delta_time), LerpTime);
	//���[�V���������[�v�������ǂ���
	bool looped = m_MotionTimer < m_PrevTimer;
	for (const auto& event : m_Events) {
		//���݂̃��[�V�������C�x���g�Ώۂ̃��[�V�����łȂ���΁A�������Ȃ�
		if (event->m_Motion != m_Motion)continue;

		if (looped) {
			if (m_PrevTimer < event->m_Time || event->m_Time <= m_MotionTimer) {
				event->m_Callback();
			}
		}
		else {
			if (m_PrevTimer < event->m_Time && event->m_Time <= m_MotionTimer) {
				event->m_Callback();
			}
		}
	}
}

void AnimatedMesh::Draw() const {
	//�X�P���g���̎����v�Z�𖳌��ɂ���
	gsDisable(GS_CALC_SKELETON);
	//�X�P���g���̃o�C���h
	gsBindSkeleton(m_Id);
	//�{�[���̕ϊ��s���ݒ�
	gsSetMatrixSkeleton(m_BoneMatrices.data());

	glPushMatrix();
	//���[���h�ϊ��s���ݒ�
	glMultMatrixf(m_Transform);
	//���b�V���̕`��
	gsDrawMesh(m_Id);
	glPopMatrix();

	//�X�P���g���̎����v�Z��L��
	gsEnable(GS_CALC_SKELETON);
}

void AnimatedMesh::ChangeMotion(GSuint motion, bool loop, GSfloat speed) {
	//���݂Ɠ������[�V�����̏ꍇ�͉������Ȃ�
	if (m_Motion == motion && loop) return;
	//��Ԓ��́A�O��̃��[�V�������X�V���Ȃ��悤�ɂ���
	if (m_LerpTimer > (LerpTime * Magnification)) {
		//�O��̃��[�V�����ԍ�
		m_PrevMotion = m_Motion;
		//���[�V�������Ԃ�ۑ�
		m_PrevMotionTimer = m_MotionTimer;
		//��Ԓ��^�C�}�̏�����
		m_LerpTimer = 0.f;
	}
	//���[�V�����X�V
	m_Motion = motion;
	//���[�V�����^�C�}�̏�����
	m_MotionTimer = 0.f;
	//�X�s�[�h
	m_Speed = speed;
	//���[�v�t���O�̐ݒ�
	m_MotionLoop = loop;
}

void AnimatedMesh::Transform(const GSmatrix4& matrix) {
	m_Transform = matrix;
	//�{�[�����Ƃ̃��[�J���ϊ��s����v�Z
	gsCalculateAnimationLerp(
		m_Id, m_PrevMotion, m_PrevMotionTimer,
		m_Id, m_Motion, m_MotionTimer,
		m_LerpTimer / LerpTime,
		m_LocalBoneMatrices.data()
	);
	//�{�[���ϊ��s����v�Z
	gsBindSkeleton(m_Id);
	gsCalculateSkeleton(NULL, m_LocalBoneMatrices.data(), m_BoneMatrices.data());
}

void AnimatedMesh::MotionTime(float time) {
	m_MotionTimer = time;
}

void AnimatedMesh::MotionStop(int value) {
	m_Speed = value;
}

void AnimatedMesh::ApplyRootMotion(GStransform& transform) {
	//�ړ��ʂƉ�]�̍������擾
	GSvector3 velocity;
	GSquaternion rotation;
	gsGetAnimationKeyFrameDelta(
		m_Id, m_Motion, 0,
		m_MotionTimer, m_PrevTimer,
		&rotation, &velocity);
	//���[�J�����W�n�ŉ�]������
	m_Transform.rotate(rotation);
	//���s�ړ��ʂɂ̓X�P�[�����O���l��
	GSvector3 scale = transform.localScale();
	//���[�J�����W�ŕ��s�ړ�
	transform.translate(GSvector3::scale(velocity, scale));
}

void AnimatedMesh::AddEvent(GSuint motion, GSfloat time, std::function<void()>callback) {
	m_Events.push_back(std::make_unique<AnimationEvent>(motion, time, callback));
}

float AnimatedMesh::MotionTime() const {
	return m_MotionTimer;
}

float AnimatedMesh::MotionSpeed() const{
	return  m_Speed;
}

 float AnimatedMesh::MotionEndTime() const {
	return gsGetEndAnimationTime(m_Id, m_Motion);
}

GSmatrix4 AnimatedMesh::BoneMatrices(int bone_no) const {
	return m_BoneMatrices[bone_no] * m_Transform;
}