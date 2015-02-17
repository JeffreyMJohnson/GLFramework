#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Globals.h"
#include "Entity.h"
#include "Platform.h"
#include "MovingPlatform.h"
#include <vector>

//class MovingPlatform;

class Player
	:public Entity
{
public:
	const char* mAnimationDescriptionFile = "smurf_sprite.xml";
	char* mCurrentAnimationState;
	glm::vec2 mVelocity;
	float mGravity;
	int mDirection;
	bool mOnGround;
	bool mJumping;
	bool isMovingPlatform = false;

	Player();
	~Player();

	void Update(float timeDelta);

	void Player::Update(float timeDelta, std::vector<Platform*>& platformList);

	void Draw();

	bool IsCollided(Entity& otherEntity);

	bool CheckPlatformCollision(std::vector<Platform*>& platformList);
};

#endif