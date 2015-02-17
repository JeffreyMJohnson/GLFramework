#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#include "Entity.h"

class Platform
	:public Entity
{
public:
	const char* mTextureFile;

	Platform();

	void Update(float deltaTime);

	void Draw();
};
#endif
