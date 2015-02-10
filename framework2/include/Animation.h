#pragma once

#include "pugixml\pugixml.hpp"
#include <vector>
#include <string>
#include <map>

struct Frame
{
	int number;
	glm::vec4 UV;
};

class Animation
{
public:
	Sprite mSprite;
	std::map<std::string, std::vector<Frame>> mFrameData;
	float mDirection;
	float mCurrentFrame;
	std::string mAnimationState;
	std::string mSpriteSheetFile;
	std::string mFilePath;
	const float mFrameDuration = (1.0 / 15.0);
	double mElapsedTime = 0;

	Animation()
	{
		mCurrentFrame = 0;
		mDirection = 1.0f;
		//mAnimationState = "walk";
		mAnimationState = "idle";
	}

	void Initialize(const float width, const float height, const char* spriteSheetDataFile)
	{
		LoadData(spriteSheetDataFile);
		mSprite.Initialize(width, height, (mFilePath + mSpriteSheetFile).c_str(), true);
	}

	void LoadData(const char* spriteSheetDataFile)
	{
		mFilePath = GetPath(spriteSheetDataFile);
		using namespace pugi;
		xml_document doc;
		xml_parse_result result = doc.load_file(spriteSheetDataFile);
		mSpriteSheetFile = doc.child("atlas").child("common").attribute("sheet").value();

		for (xml_node frames : doc.child("atlas").children("frames"))
		{
			std::vector <Frame> framesList;
			for (xml_node frame : frames.children())
			{
				Frame f;
				f.number = std::atoi(frame.attribute("id").value());
				f.UV.x = std::atof(frame.attribute("minX").value());
				f.UV.y = std::atof(frame.attribute("minY").value());
				f.UV.z = std::atof(frame.attribute("maxX").value());
				f.UV.w = std::atof(frame.attribute("maxY").value());
				framesList.push_back(f);
			}
			mFrameData[frames.attribute("name").value()] = framesList;
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
			if (mCurrentFrame == mFrameData[mAnimationState].size() - 1)
			{
				mCurrentFrame = 0;
			}
			else
			{
				mCurrentFrame++;
			}
		}
		mSprite.SetSpriteUV(mFrameData[mAnimationState][mCurrentFrame].UV.x, mFrameData[mAnimationState][mCurrentFrame].UV.y, mFrameData[mAnimationState][mCurrentFrame].UV.z, mFrameData[mAnimationState][mCurrentFrame].UV.w);
	}

	void SwitchDirection()
	{
		mDirection *= -1;
		mSprite.scale.x *= -1;
		mSprite.UpdateTransform();
	}

	void Draw()
	{
		mSprite.Draw();
	}

	std::string GetPath(const char* spriteSheetDataFile)
	{
		std::string s(spriteSheetDataFile);
		uint pos = s.find_last_of("\\");
		if (pos == std::string::npos)
			return "";
		return s.substr(0, pos + 1);
	}

};