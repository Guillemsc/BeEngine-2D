#ifndef __MODULE_TEXT_H__
#define __MODULE_TEXT_H__

#include "Module.h"

#include <ft2build.h>
#include FT_FREETYPE_H  


class ModuleText : public Module
{
public:
	ModuleText();
	~ModuleText();

	bool Awake();
	bool CleanUp();

	bool LoadFace(const std::string& font_filepath, FT_Face& face);

private:
	FT_Library ft_library;

};

#endif // !__MODULE_TEXT_H__