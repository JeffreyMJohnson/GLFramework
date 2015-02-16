#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#include "Entity.h"

class Platform
	:public Entity
{
public:
	const char* mTextureFile;

	Platform()
	{
		mColliderOffset.x = .6;
		UpdateCollider();
	}

	void Update(float deltaTime)
	{

	}

	void Draw()
	{

	}
};
#endif
