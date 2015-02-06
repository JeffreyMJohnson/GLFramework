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
	Sprite mSpriteSheet;
	std::string mName;
	std::vector<Frame> mFrames;
	Frame mCurrentFrame;

	Animation()
	{
		mSpriteSheet.Initialize(100, 100, ".\\resources\\images\\smurf_sprite.png", true);
		mSpriteSheet.translation = glm::vec3(300, 300, 0);
		mSpriteSheet.UpdateTransform();

		glm::vec2 spriteSize((512.0f / 4), (512.0f / 4));
		float sheetSize = 512.0f;

		LoadData();

		//Frame f1;
		//f1.number = 0;
		//f1.UV = glm::vec4(
		//	0,
		//	0,
		//	spriteSize.x / sheetSize,
		//	spriteSize.y / sheetSize);
		//mFrames.push_back(f1);

		//Frame f2;
		//f2.number = 1;
		//f2.UV = glm::vec4(
		//	spriteSize.x / sheetSize,
		//	0,
		//	(spriteSize.x * 2) / sheetSize,
		//	spriteSize.y / sheetSize);
		//mFrames.push_back(f2);

		//Frame f3;
		//f3.number = 2;
		//f3.UV = glm::vec4(
		//	(spriteSize.x * 2) / sheetSize,
		//	0,
		//	(spriteSize.x * 3) / sheetSize,
		//	spriteSize.y / sheetSize);
		//mFrames.push_back(f3);

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

	void Update()
	{
		//check if last frams
		if (mCurrentFrame.number == mFrames.size() - 1)
		{
			mCurrentFrame = mFrames[0];
		}
		else
		{
			mCurrentFrame = mFrames[(mCurrentFrame.number + 1)];
		}
		mSpriteSheet.SetUV(mCurrentFrame.UV.x, mCurrentFrame.UV.y, mCurrentFrame.UV.z, mCurrentFrame.UV.w);

	}

	void Draw()
	{
		mSpriteSheet.Draw();
	}

};