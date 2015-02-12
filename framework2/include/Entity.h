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

	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;


};

#endif