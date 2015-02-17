#ifndef _MOVING_PLATFORM_H_
#define _MOVING_PLATFORM_H_


#include "Platform.h"
#include "glm\gtx\compatibility.hpp"

class MovingPlatform
	: public Platform
{
public:
	glm::vec2 mStopPositionMin = glm::vec2(0, 0);
	glm::vec2 mStopPositionMax = glm::vec2(0, 0);
	float mMovementSpeed;
	float currentLERPPercentage;
	int mMoveDirection;
	
	MovingPlatform();

	void Update(float timeDelta);
};

#endif