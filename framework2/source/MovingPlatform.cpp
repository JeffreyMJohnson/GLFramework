#include "MovingPlatform.h"

MovingPlatform::MovingPlatform()
{
	currentLERPPercentage = 0;
	mMoveDirection = 1;
	mMovementSpeed = .1;
}

void MovingPlatform::Update(float timeDelta)
{
	if (currentLERPPercentage < 0 && mMoveDirection == -1)
	{
		mMoveDirection = 1;

	}
	if (currentLERPPercentage > 1 && mMoveDirection == 1)
	{
		mMoveDirection = -1;
	}
	currentLERPPercentage += timeDelta * mMoveDirection * mMovementSpeed;
	mPosition = glm::lerp(mStopPositionMin, mStopPositionMax, currentLERPPercentage);
	UpdateCollider();
}