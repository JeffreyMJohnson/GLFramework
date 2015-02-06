#pragma once

#include "pugixml\pugixml.hpp"
#include <vector>
#include <string>

struct Frame
{
	int number;
	glm::vec4 UV;
};

class Animation
{
public:
	Sprite mSprite;
	std::string mName;
	std::vector<Frame> mFrames;
	int mCurrentFrame;
	const float mFrameDuration = (1.0 / 15.0);
	double mElapsedTime = 0;

	Animation()
	{
		mSprite.Initialize(100, 100, ".\\resources\\images\\smurf_sprite.png", true);
		mSprite.translation = glm::vec3(300, 300, 0);
		mSprite.UpdateTransform();

		glm::vec2 spriteSize((512.0f / 4), (512.0f / 4));
		float sheetSize = 512.0f;

		LoadData();
		mCurrentFrame = 0;

	}

	void LoadData()
	{
		using namespace pugi;
		xml_document doc;
		xml_parse_result result = doc.load_file(".\\resources\\images\\smurf_sprite.xml");

		for (xml_node frame : doc.child("atlas").child("frames").children())
		{
			Frame f;
			f.number = std::atoi(frame.attribute("id").value());
			f.UV.x = std::atof(frame.attribute("minX").value());
			f.UV.y = std::atof(frame.attribute("minY").value());
			f.UV.z = std::atof(frame.attribute("maxX").value());
			f.UV.w = std::atof(frame.attribute("maxY").value());
			mFrames.push_back(f);
		}
	}

	void Update(double deltaTime)
	{
		const float speed = .1;
		mElapsedTime += deltaTime;

		if (mElapsedTime > mFrameDuration)
		{
			mElapsedTime = 0;
			//check if last frame
			if (mCurrentFrame == mFrames.size() - 1)
			{
				mCurrentFrame = 0;
			}
			else
			{
				mCurrentFrame++;
			}
		}
		mSprite.SetUV(mFrames[mCurrentFrame].UV.x, mFrames[mCurrentFrame].UV.y, mFrames[mCurrentFrame].UV.z, mFrames[mCurrentFrame].UV.w);
		

	}

	void Draw()
	{
		mSprite.Draw();
	}

};