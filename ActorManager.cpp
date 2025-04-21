#include "ActorManager.h"
#include "Actor.h"

ActorManager::~ActorManager() {
	Clear();
}

void ActorManager::Add(Actor* actor) {
	m_Actors.push_back(actor);
}

void ActorManager::Update(float delta_time) {
	std::ranges::for_each(m_Actors,
		[delta_time](Actor* actor) {actor->Update(delta_time); });
}

void ActorManager::LateUpdate(float delta_time) {
	std::ranges::for_each(m_Actors,
		[delta_time](Actor* actor) { actor->LateUpdate(delta_time); });
}

void ActorManager::Draw() const {
	std::ranges::for_each(m_Actors,
		[](Actor* actor) { actor->Draw(); });
}

void ActorManager::DrawTransparent() const {
	std::ranges::for_each(m_Actors,
		[](Actor* actor) { actor->DrawTransparent(); });
}

void ActorManager::DrawGui() const {
	std::ranges::for_each(m_Actors,
		[](Actor* actor) { actor->DrawGui(); });
}

void ActorManager::Collide() {
	for (auto i = m_Actors.begin(); i != m_Actors.end(); ++i) {
		std::for_each(std::next(i), m_Actors.end(), [i](Actor* actor) {
			(*i)->Collide(*actor); });
	}
}

void ActorManager::Remove() {
	m_Actors.remove_if(
		[](Actor* actor)->bool {
			if (actor->IsDead()) {
				delete actor;
				return true;
			}
			return false;
		}
	);
}

void ActorManager::OnMessage(const std::string& message, void* param) {
	std::ranges::for_each(m_Actors,
		[&message, param](Actor* actor) {actor->HandleMessage(message, param); });
}

void ActorManager::Clear() {
	std::ranges::for_each(m_Actors,
		[](Actor* actor) {delete actor; });
	m_Actors.clear();
}

Actor* ActorManager::Find(const std::string& name)const {
	auto i = std::ranges::find_if(m_Actors,
		[&name](Actor* actor) {return actor->Name() == name; });
	return (i != m_Actors.end()) ? *i : nullptr;
}

std::vector<Actor*>ActorManager::FindWithTag(const std::string& tag) const {
	auto v = m_Actors | std::views::filter([&tag](Actor* actor) {return actor->Tag() == tag; });
	return std::vector<Actor*>(v.begin(), v.end());
}

int ActorManager::Count() const {
	return (int)m_Actors.size();
}

int ActorManager::CountWithTag(const std::string& tag) const {
	return std::ranges::count_if(m_Actors,
		[&tag](Actor* actor){ return actor->Tag() == tag; });
}

