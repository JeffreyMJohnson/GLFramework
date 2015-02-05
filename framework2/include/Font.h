#include "glm\glm.hpp"
#include "pugixml\pugixml.hpp"
#include <map>
#include <iostream>
#include <string>

#include "FontChar.h"


typedef unsigned int uint;

class Font
{
public:
	uint lineHeight;
	std::string name;
	std::string imageFileName;
	std::string path;
	uint charCount;
	uint spriteSheetWidth, spriteSheetHeight;
	uint fontSpriteSheetTextureID;
	std::map<uint, FontChar> Chars;

	Font()
	{
		lineHeight = 0;
		name = "";
		charCount = 0;
	}

	bool Init(const char* fontFilePath, const char* fontFileName)
	{
		//imageFileName = fontImagFile;
		//fontSpriteSheetTextureID = loadTexture(fontImagFile);

		path = std::string(fontFilePath);

		if (!LoadFont(fontFileName))
		{
			return false;
		}
		return true;

	}

private:
	bool LoadFont(const char* fontFileName)
	{
		std::string pathName(path);
		pathName += fontFileName;
		using namespace pugi;
		xml_document doc;
		//xml_parse_result result = doc.load_file(".\\resources\\fonts\\arial.fnt");
		xml_parse_result result = doc.load_file(pathName.c_str());
		if (!result)
		{
			std::cout << "Error loading font file, verify path (do the slashes need escaping?)\n";
			std::cout << result.description() << "\n";
			return false;
		}

		xml_node common = doc.child("font").child("common");

		//load the charset common attributes
		lineHeight = std::atoi(common.attribute("lineHeight").value());

		//base = std::atoi(common.attribute("base").value());
		spriteSheetWidth = std::atoi(common.attribute("scaleW").value());
		spriteSheetHeight = std::atoi(common.attribute("scaleH").value());

		name = doc.child("font").child("info").attribute("face").value();
		//imageFileName = doc.child("font").child("pages").child("page").attribute("file").value();

		//load each char
		for (xml_node Char : doc.child("font").child("chars").children("char"))
		{
			FontChar ch;
			ch.ID = std::atoi(Char.attribute("id").value());
			ch.fontSheetLocation = glm::vec2(std::atoi(Char.attribute("x").value()), std::atoi(Char.attribute("y").value()));
			ch.width = std::atoi(Char.attribute("width").value());
			ch.height = std::atoi(Char.attribute("height").value());
			ch.xOffset = std::atof(Char.attribute("xoffset").value());
			ch.yOffset = std::atof(Char.attribute("yoffset").value());
			ch.xAdvance = std::atof(Char.attribute("xadvance").value());

			ch.UV = CalculateUV(ch);
			Chars[ch.ID] = ch;
		}

		//get filename
		imageFileName = doc.child("font").child("pages").first_child().attribute("file").value();
		return true;
	}

	glm::vec4 CalculateUV(FontChar& ch)
	{
		//top left origin
		float left = ch.fontSheetLocation.x;
		float top = spriteSheetHeight - ch.fontSheetLocation.y;//invert the y origin
		float right = left + ch.width;
		float bottom = spriteSheetHeight - (ch.fontSheetLocation.y + ch.height);//invert y origin

		//normalize
		glm::vec4 result(
			left / spriteSheetWidth,
			top / spriteSheetHeight,
			right / spriteSheetWidth,
			bottom / spriteSheetHeight);
		return result;
	}

	unsigned int loadTexture(const char* a_pFilename)
	{
		unsigned int uiTextureID = 0;
		////check file exists
		//if (a_pFilename != nullptr)
		//{
		//	//read in image data from file
		//	unsigned char* pImageData = SOIL_load_image(a_pFilename, &a_iWidth, &a_iHeight, &a_iBPP, SOIL_LOAD_AUTO);

		//	//check for successful read
		//	if (pImageData)
		//	{
		//		//create opengl texture handle
		//		uiTextureID = SOIL_create_OGL_texture(pImageData, a_iWidth, a_iHeight, a_iBPP,
		//			SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		//		//clear what was read in from file now that it is stored in the handle
		//		SOIL_free_image_data(pImageData);
		//	}

		uiTextureID = SOIL_load_OGL_texture(a_pFilename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);

		//check for errors
		if (uiTextureID == 0)
		{
			std::cerr << "SOIL loading error: " << SOIL_last_result() << std::endl;
		}
		return uiTextureID;
		//}
	}
};

