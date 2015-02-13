#ifndef _ENTITY_H_
#define _ENTITY_H_

typedef unsigned int uint;
typedef glm::vec2 vec2;

//game object base class
class Entity
{
public:
	uint mSpriteID = 0;
	vec2 mSize = vec2(0, 0);
	vec2 mPosition = vec2(0, 0);
	vec2 mColliderBoxMin = vec2(0, 0);
	vec2 mColliderBoxMax = vec2(0, 0);


	void Init(const vec2& size, const vec2& position)
	{
		mSize = size;
		mPosition = position;
		UpdateCollider();
	}

	void UpdateCollider()
	{
		mColliderBoxMin = mPosition - (mSize * .5f);
		mColliderBoxMax = mPosition + (mSize * .5f);
	}

	bool NewIsCollided(Entity& otherEntity)
	{ 
		return MNF::Collider::AABB(mColliderBoxMin, mColliderBoxMax, otherEntity.mColliderBoxMin, otherEntity.mColliderBoxMax);
	}

	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;


};

#endif