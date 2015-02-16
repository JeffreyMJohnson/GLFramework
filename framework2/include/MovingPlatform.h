#ifndef _MOVING_PLATFORM_H_
#define _MOVING_PLATFORM_H_
#include "Platform.h"
#include "glm\gtx\compatibility.hpp"

class MovingPlatform
	: public Platform
{
public:
	vec2 mStopPositionMin = vec2(0, 0);
	vec2 mStopPositionMax = vec2(0, 0);
	float mMovementSpeed;
	float currentLERPPercentage;
	int mMoveDirection;
	
	MovingPlatform()
	{
		currentLERPPercentage = 0;
		mMoveDirection = 1;
		mMovementSpeed = .1;
	}

	void Update(float timeDelta)
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


};

#endif