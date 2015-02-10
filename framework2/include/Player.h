#include "glm\glm.hpp"
#include "Animation.h"
#include <math.h>

class Player
	:public Animation
{
public:
	const char* mAnimationDescriptionFile = "smurf_sprite.xml";
	unsigned int mSpriteID;
	glm::vec2 mSize;
	glm::vec2 mPosition;
	glm::vec2 mVelocity;
	float mGravity;
	int mDirection;
	bool mOnGround;
	bool mJumping;

	Player()
	{
		mSize = glm::vec2(100, 100);
		mPosition = glm::vec2(100, 115);
		mVelocity = glm::vec2(0, 0);
		mGravity = 35;
		mDirection = 1;
		mOnGround = false;
		mJumping = false;
	}

	void Update(float timeDelta)
	{

	}

	void Draw()
	{
		
	}

	bool IsCollided(Sprite& a_sprite)
	{
		if (mPosition.y - (mSize.y * .5) < a_sprite.scale.y * .5)
		{
			return true;
		}
		return false;
	}

	bool Equalsf(float lhs, float rhs, float delta)
	{
		return (fabs(lhs - rhs) < delta);
	}






};