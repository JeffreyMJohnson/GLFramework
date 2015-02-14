#include "Entity.h"
#include "Platform.h"
#include <math.h>

class Player
	:public Entity
{
public:
	const char* mAnimationDescriptionFile = "smurf_sprite.xml";
	char* mCurrentAnimationState;
	vec2 mVelocity;
	float mGravity;
	int mDirection;
	bool mOnGround;
	bool mJumping;

	Player()
	{
		/*mSize = vec2(100, 100);
		mPosition = vec2(100, 115);*/
		mVelocity = vec2(0, 0);
		mGravity = 35;
		mDirection = 1;
		mOnGround = false;
		mJumping = false;
		mCurrentAnimationState = "idle";
	}

	void Update(float timeDelta)
	{

	}

	void Update(float timeDelta, std::vector<Platform>& platformList)
	{
		bool isCollided = CheckPlatformCollision(platformList);

		if (!isCollided)
		{
			//not colliding 
			mVelocity.y -= 10;
		}
		else 
		{
			//colliding and on ground
			if (mOnGround)
			{
				mJumping = false;
				mVelocity.y = 0;
			}
			else
			{
				//colliding not on ground
				mVelocity = vec2(0, -10);
			}
			
		}		

		if (mVelocity.x > 0)
		{
			mVelocity.x -= 5;
			mCurrentAnimationState = "walk";
			//frk.SetAnimationState(mSpriteID, "walk");
		}
		else if (mVelocity.x < 0)
		{
			mVelocity.x += 5;
			mCurrentAnimationState = "walk";
			//frk.SetAnimationState(mSpriteID, "walk");
		}
		else if (MNF::Utils::Equalsf(mVelocity.x, 0.0f, .001))
		{
			mVelocity.x = 0;
			mCurrentAnimationState = "idle";
			//frk.SetAnimationState(mSpriteID, "idle");
		}

		/*frk.MoveAnimation(mSpriteID, mPosition.x += mVelocity.x * deltaTime,
			mPosition.y += mVelocity.y * deltaTime);*/
		UpdateCollider();
	}

	void Draw()
	{
		
	}

	bool IsCollided(Entity& otherEntity)
	{
		mOnGround = false;
		bool r = MNF::Collider::AABB(mColliderBoxMin, mColliderBoxMax, otherEntity.mColliderBoxMin, otherEntity.mColliderBoxMax);
		if (r)
		{
			//check if platform
			Platform* p = dynamic_cast<Platform*>(&otherEntity);
			if (p != nullptr)
			{
				//if on top of platform with bottom of player set 
				if (mColliderBoxMin.y < p->mColliderBoxMax.y)
				{
					mOnGround = true;
				}
			}

		}
		return r;
	}

	bool Equalsf(float lhs, float rhs, float delta)
	{
		return (fabs(lhs - rhs) < delta);
	}

	bool CheckPlatformCollision(std::vector<Platform>& platformList)
	{
		for (std::vector<Platform>::iterator it = platformList.begin(); it != platformList.end(); it++)
		{
			if (IsCollided((*it)))
				return true;
		}
		return false;
	}




};