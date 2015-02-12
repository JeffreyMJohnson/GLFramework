#include "Entity.h"
#include <math.h>

class Player
	:public Entity
{
public:
	const char* mAnimationDescriptionFile = "smurf_sprite.xml";
	vec2 mVelocity;
	float mGravity;
	int mDirection;
	bool mOnGround;
	bool mJumping;

	Player()
	{
		mSize = vec2(100, 100);
		mPosition = vec2(100, 115);
		mVelocity = vec2(0, 0);
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