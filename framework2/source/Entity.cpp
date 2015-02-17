#include "Entity.h"

void Entity::Init(const glm::vec2& size, const glm::vec2& position)
{
	mSize = size;
	mPosition = position;
	UpdateCollider();
}

void Entity::UpdateCollider()
{
	mColliderBoxMin = mPosition - (mSize * mColliderOffset);
	mColliderBoxMax = mPosition + (mSize * mColliderOffset);
}

bool Entity::IsCollided(Entity& otherEntity)
{
	return MNF::Collider::AABB(mColliderBoxMin, mColliderBoxMax, otherEntity.mColliderBoxMin, otherEntity.mColliderBoxMax);
}