#include "glm\glm.hpp"
#include "Animation.h"

class Player
	:public Animation
{
public:
	const char* aniationDescriptionFile = "smurf_sprite.xml";
	glm::vec2 size = glm::vec2(150, 200);
	glm::vec2 position;
	float moveSpeed = 10;


};