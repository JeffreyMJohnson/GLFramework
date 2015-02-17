#include "Player.h"

Player::Player()
{
	mVelocity = glm::vec2(0, 0);
	mGravity = 35;
	mDirection = 1;
	mOnGround = false;
	mJumping = false;
	mCurrentAnimationState = "idle";
}

Player::~Player(){};

void Player::Update(float timeDelta){};

void Player::Update(float timeDelta, std::vector<Platform*>& platformList)
{

	bool isCollided = CheckPlatformCollision(platformList);



	if (!isCollided)
	{
		//not colliding 
		mVelocity.y -= 5;
	}
	else
	{
		//colliding and on ground
		if (mOnGround && !isMovingPlatform)
		{
			mJumping = false;
			mVelocity.y = 0;
		}
		else if (mOnGround && isMovingPlatform)
		{
			mJumping = false;

		}
		else
		{
			//colliding not on ground
			mVelocity = glm::vec2(0, -100);
		}

	}

	if (mVelocity.x > 0 && !mJumping)
	{
		mVelocity.x -= 5;
		mCurrentAnimationState = "walk";
		//frk.SetAnimationState(mSpriteID, "walk");
	}
	else if (mVelocity.x < 0 && !mJumping)
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

	if (mPosition.x > MNF::Globals::SCREEN_WIDTH)
	{
		mPosition.x = MNF::Globals::SCREEN_WIDTH;
		mVelocity.x = 0;

	}
	if (mColliderBoxMin.x < 0)
	{
		mPosition.x = mSize.x * .25;
		mVelocity.x = 0;
		UpdateCollider();
	}
	if (mColliderBoxMax.x > MNF::Globals::SCREEN_WIDTH)
	{
		mPosition.x = MNF::Globals::SCREEN_WIDTH - mSize.x * .25;
		mVelocity.x = 0;
		UpdateCollider();

	}
}

void Player::Draw(){};

bool Player::IsCollided(Entity& otherEntity)
{
	mOnGround = false;
	isMovingPlatform = false;
	bool r = MNF::Collider::AABB(mColliderBoxMin, mColliderBoxMax, otherEntity.mColliderBoxMin, otherEntity.mColliderBoxMax);
	if (r)
	{
		//check if platform
		Platform* p = dynamic_cast<Platform*>(&otherEntity);
		if (p != nullptr)
		{
			//if on top of platform with bottom of player set 
			if (mColliderBoxMin.y < p->mColliderBoxMax.y && mColliderBoxMax.y > p->mColliderBoxMax.y
				&& mColliderBoxMin.x > p->mColliderBoxMin.x && mColliderBoxMax.x < p->mColliderBoxMax.x)
			{
				if (dynamic_cast<MovingPlatform*>(&otherEntity))
				{
					isMovingPlatform = true;
					mPosition.y = p->mColliderBoxMax.y + mSize.y * .45;
				}
				mOnGround = true;
			}
		}

	}
	return r;
}

bool Player::CheckPlatformCollision(std::vector<Platform*>& platformList)
{
	for (std::vector<Platform*>::iterator it = platformList.begin(); it != platformList.end(); it++)
	{
		if (IsCollided(*(*it)))
		{
			return true;
		}

	}
	return false;
}

