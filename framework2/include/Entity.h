#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "glm\glm.hpp"
#include "Globals.h"

//game object base class
class Entity
{
public:
	unsigned int mSpriteID = 0;
	glm::vec2 mSize = glm::vec2(0, 0);
	glm::vec2 mPosition = glm::vec2(0, 0);
	glm::vec2 mColliderBoxMin = glm::vec2(0, 0);
	glm::vec2 mColliderBoxMax = glm::vec2(0, 0);
	glm::vec2 mColliderOffset = glm::vec2(.50f, .5f);


	void Init(const glm::vec2& size, const glm::vec2& position);

	void UpdateCollider();

	bool IsCollided(Entity& otherEntity);

	virtual void Update(float deltaTime){};
	virtual void Draw(){};


};

#endif