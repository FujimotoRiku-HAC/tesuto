#include "Actor.h"

void Actor::Update(float){}
void Actor::LateUpdate(float){}
void Actor::Draw() const{}
void Actor::DrawTransparent() const{}
void Actor::DrawGui()const {}
void Actor::React(Actor&) {}
void Actor::HandleMessage(const std::string&message, void* param){}

void Actor::Collide(Actor& other) {
	//どちらのアクターも衝突判定が有効か
	if (m_EnableCollider && other.m_EnableCollider) {
		//衝突判定をする
		if (IsCollide(other)) {
			//衝突した場合は、お互いに衝突リアクションをする
			React(other);
			other.React(*this);
		}
	}
}
void Actor::Die() {
	m_Dead = true;
}

bool Actor::IsCollide(const Actor& other)const {
	return Collider().Intersects(other.Collider());
}

bool Actor::IsDead() const {
	return m_Dead;
}

const std::string& Actor::Name() const {
	return m_Name;
}

const std::string& Actor::Tag() const {
	return m_Tag; 
}

const GStransform& Actor::Transform() const {
	return m_Transform;
}

GStransform& Actor::Transform() {
	return m_Transform;
}

GSvector3 Actor::Velocity() const {
	return m_Velocity;
}

BoundingSphere Actor::Collider() const {
	return m_Collider.Transform(m_Transform.localToWorldMatrix());
}

void Actor::PlayEffect(GSuint id, const GSvector3& local_position, const GSvector3& local_rotation,
	const GSvector3& local_scale) {
	//指定されたTranslate, Rotation, Scaleの行列を作成する
	GSmatrix4 local_matrix = GSmatrix4::TRS(local_position, GSquaternion::euler(local_rotation), local_scale);
	//ワールド空間に変換する
	GSmatrix4 world_matrix = local_matrix * m_Transform.localToWorldMatrix();
	//エフェクトを再生する
	gsPlayEffectEx(id, &world_matrix);
}

TweenUnit& Actor::MoveTo(const GSvector2& from,const GSvector2& to, float duration) {
	// 現在の場所から指定された場所まで、Tweenで移動する
	return Tween::Vector2(from, to, duration,
		[=](GSvector2 pos) {m_Transform.position(pos); });
}

TweenUnit& Actor::MoveTo(const GSvector3& to, float duration) {
	return Tween::Vector3(m_Transform.position(), to, duration,
		[=](GSvector3 pos) {m_Transform.position(pos); });
}