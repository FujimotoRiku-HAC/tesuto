#include "AnimatedMesh.h"

//補間フレーム数
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
	//更新前のタイマー値を取っておく
	m_PrevTimer = m_MotionTimer;
	//アニメーションタイマの更新
	m_MotionTimer += m_Speed * delta_time;

	if (m_MotionLoop) {
		//モーションタイマをループさせる
		m_MotionTimer = std::fmod(m_MotionTimer, MotionEndTime());
	}
	else {

		m_MotionTimer = std::min(m_MotionTimer, MotionEndTime() - 1.0f);
	}

	m_LerpTimer = std::min(m_LerpTimer + (m_Speed * delta_time), LerpTime);
	//モーションがループしたかどうか
	bool looped = m_MotionTimer < m_PrevTimer;
	for (const auto& event : m_Events) {
		//現在のモーションがイベント対象のモーションでなければ、何もしない
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
	//スケルトンの自動計算を無効にする
	gsDisable(GS_CALC_SKELETON);
	//スケルトンのバインド
	gsBindSkeleton(m_Id);
	//ボーンの変換行列を設定
	gsSetMatrixSkeleton(m_BoneMatrices.data());

	glPushMatrix();
	//ワールド変換行列を設定
	glMultMatrixf(m_Transform);
	//メッシュの描画
	gsDrawMesh(m_Id);
	glPopMatrix();

	//スケルトンの自動計算を有効
	gsEnable(GS_CALC_SKELETON);
}

void AnimatedMesh::ChangeMotion(GSuint motion, bool loop, GSfloat speed) {
	//現在と同じモーションの場合は何もしない
	if (m_Motion == motion && loop) return;
	//補間中は、前回のモーションを更新しないようにする
	if (m_LerpTimer > (LerpTime * Magnification)) {
		//前回のモーション番号
		m_PrevMotion = m_Motion;
		//モーション時間を保存
		m_PrevMotionTimer = m_MotionTimer;
		//補間中タイマの初期化
		m_LerpTimer = 0.f;
	}
	//モーション更新
	m_Motion = motion;
	//モーションタイマの初期化
	m_MotionTimer = 0.f;
	//スピード
	m_Speed = speed;
	//ループフラグの設定
	m_MotionLoop = loop;
}

void AnimatedMesh::Transform(const GSmatrix4& matrix) {
	m_Transform = matrix;
	//ボーンごとのローカル変換行列を計算
	gsCalculateAnimationLerp(
		m_Id, m_PrevMotion, m_PrevMotionTimer,
		m_Id, m_Motion, m_MotionTimer,
		m_LerpTimer / LerpTime,
		m_LocalBoneMatrices.data()
	);
	//ボーン変換行列を計算
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
	//移動量と回転の差分を取得
	GSvector3 velocity;
	GSquaternion rotation;
	gsGetAnimationKeyFrameDelta(
		m_Id, m_Motion, 0,
		m_MotionTimer, m_PrevTimer,
		&rotation, &velocity);
	//ローカル座標系で回転させる
	m_Transform.rotate(rotation);
	//平行移動量にはスケーリングを考慮
	GSvector3 scale = transform.localScale();
	//ローカル座標で平行移動
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