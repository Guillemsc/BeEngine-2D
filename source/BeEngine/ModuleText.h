#ifndef __MODULE_TEXT_H__
#define __MODULE_TEXT_H__

#include <map>

#include "Module.h"
#include "GeometryMath.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

class TextData;

class Glyph
{
	friend class ModuleText;

public:
	Glyph();

	uint GetTextureId() const;
	float2 GetSize() const;
	float2 GetBearing() const;
	float GetAdvance() const;

private:
	uint texture_id = 0;
	float2 size = float2::zero;
	float2 bearing = float2::zero; // Offset from baseline to left/top of glyph
	float advance = 0; // Offset to advance to next glyph
};

class Font
{
	friend class ModuleText;

public:
	Font();

	TextData GetTextGlyphs(const std::string& text);

	int GetSize() const;
	float GetHeight() const;

private:
	std::string name;
	uint size = 0;
	uint height = 0;

	std::map<char, Glyph> glyphs;
};

class TextData
{
	friend class Font;

public:
	std::string GetText() const;
	int GetFontSize() const;
	std::vector<Glyph> GetGlyphs() const;
	float2 GetFullSize() const;

private:
	std::string text = "";
	int size = 0;
	std::vector<Glyph> glyphs;
	float2 full_size = float2::zero;
};

class ModuleText : public Module
{
public:
	ModuleText();
	~ModuleText();

	bool Awake();
	bool CleanUp();

	bool LoadFace(const std::string& font_filepath, FT_Face& face);
	void UnloadFace(FT_Face& face);
	void SetFaceSize(FT_Face& face, int size);
	bool LoadFaceChar(FT_Face& face, char char_to_load);

	Font* GenerateFont(FT_Face face, int size);
	void DestroyFont(Font* font);

private:
	void DestroyAllFonts();

private:
	FT_Library ft_library;

	std::vector<Font*> fonts;
};

#endif // !__MODULE_TEXT_H__