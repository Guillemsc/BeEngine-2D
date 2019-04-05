#include "ModuleText.h"

#pragma comment( lib, "FreeType/lib/freetype.lib" )

ModuleText::ModuleText() : Module()
{
}

ModuleText::~ModuleText()
{
}

bool ModuleText::Awake()
{
	bool ret = true;

	if (FT_Init_FreeType(&ft_library))
	{
		INTERNAL_LOG("Could not init FreeType Library");
		ret = false;
	}

	return ret;
}

bool ModuleText::CleanUp()
{
	bool ret = true;

	return ret;
}

bool ModuleText::LoadFace(const std::string & font_filepath, FT_Face & face)
{
	bool ret = true;

	if (FT_New_Face(ft_library, font_filepath.c_str(), 0, &face))
	{
		ret = false;

		INTERNAL_LOG("Failed to load font: %s", font_filepath.c_str());
	}

	return ret;
}
