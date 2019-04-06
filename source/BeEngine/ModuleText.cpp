#include "ModuleText.h"
#include "ModuleRenderer.h"
#include "App.h"

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

	DestroyAllFonts();

	FT_Done_FreeType(ft_library);

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

void ModuleText::UnloadFace(FT_Face & face)
{
	FT_Done_Face(face);
}

void ModuleText::SetFaceSize(FT_Face & face, int size)
{
	FT_Set_Pixel_Sizes(face, 0, size);
}

bool ModuleText::LoadFaceChar(FT_Face & face, char char_to_load)
{
	bool ret = true;

	if (FT_Load_Char(face, char_to_load, FT_LOAD_RENDER))
	{
		ret = false;

		INTERNAL_LOG("Failed to load font char: %c", char_to_load);
	}

	return ret;
}

Font* ModuleText::GenerateFont(FT_Face face, int size)
{
	uint amount_glyphs = 128;

	Font* ret = new Font();

	ret->name = face->family_name;
	ret->size = size;

	SetFaceSize(face, size);

	ret->ascender = face->ascender / 32;

	for (int i = 0; i < amount_glyphs; ++i)
	{
		if (LoadFaceChar(face, i))
		{
			uint texture_id = App->renderer->LoadTextureToVRAM(face->glyph->bitmap.width, 
				face->glyph->bitmap.rows, face->glyph->bitmap.buffer, GL_RED);

			if (texture_id > 0)
			{
				Glyph curr_glyph;
				curr_glyph.texture_id = texture_id;
				curr_glyph.size = float2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
				curr_glyph.bearing = float2(face->glyph->bitmap_left, face->glyph->bitmap_top);
				curr_glyph.advance = face->glyph->advance.x;

				ret->glyphs[i] = curr_glyph;
			}
		}
	}

	fonts.push_back(ret);

	return ret;
}

void ModuleText::DestroyFont(Font* font)
{
	if (font != nullptr)
	{
		for (std::vector<Font*>::iterator it = fonts.begin(); it != fonts.end(); ++it)
		{
			if (font == (*it))
			{
				fonts.erase(it);
				break;
			}
		}

		for (std::map<char, Glyph>::iterator it = font->glyphs.begin(); it != font->glyphs.end(); ++it)
		{
			if ((*it).second.texture_id > 0)
			{
				App->renderer->DeleteTexture((*it).second.texture_id);
			}
		}

		RELEASE(font);
	}
}

void ModuleText::DestroyAllFonts()
{
	for (std::vector<Font*>::iterator it = fonts.begin(); it != fonts.end(); ++it)
	{
		for (std::map<char, Glyph>::iterator f = (*it)->glyphs.begin(); f != (*it)->glyphs.end(); ++f)
		{
			if ((*f).second.texture_id > 0)
			{
				App->renderer->DeleteTexture((*f).second.texture_id);
			}
		}

		RELEASE((*it));
	}

	fonts.clear();
}

Glyph::Glyph()
{
}

uint Glyph::GetTextureId() const
{
	return texture_id;
}

float2 Glyph::GetSize() const
{
	return size;
}

float2 Glyph::GetBearing() const
{
	return bearing;
}

uint Glyph::GetAdvance() const
{
	return advance;
}

Font::Font()
{
}

std::vector<Glyph> Font::GetTextGlyphs(const std::string& text)
{
	std::vector<Glyph> ret;

	for (int i = 0; i < text.size(); ++i)
	{
		char curr_char = text[i];

		ret.push_back(glyphs[curr_char]);
 	}

	return ret;
}

int Font::GetSize() const
{
	return size;
}

float Font::GetAscender() const
{
	return ascender;
}
