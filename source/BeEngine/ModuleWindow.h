#ifndef __MODULE_WINDOW_H__
#define __MODULE_WINDOW_H__

#include "Module.h"
#include "GeometryMath.h"
#include "SDL/include/SDL.h"

#define SCREEN_SIZE 1

class Application;
class Cursor;

class WindowNaming
{
	struct NamePart
	{
		std::string identifier;
		std::string val;
	};

public:
	WindowNaming();

	void AddNamePart(std::string identifier, std::string default_val);
	void UpdateNamePart(std::string identifier, std::string value);

private:
	void UpdateWindowName();

private:
	std::vector<NamePart> name_parts;
};

class ModuleWindow : public Module
{
public:
	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	void OnLoadConfig(JSON_Doc* config);
	void OnSaveConfig(JSON_Doc* config);
	bool Awake();
	bool Update();
	bool CleanUp();

	void SetWindowTitle(const char* title);

	void SetWindowSize(int width, int height);
	void GetWindowSize(int &width, int &height);
	const float2 GetWindowSize();

	void GetDisplaySize(int &width, int &height);
	const float2 GetDisplaySize();

	void SetFullscreen(const bool& set);
	const bool GetFullscreen() const;

	void SetResizable(const bool& set);
	const bool GetResizable() const;
	void SetBorderless(const bool& set);
	const bool GetBorderless() const;
	void SetFullDekstop(const bool& set);
	const bool GetFullDekstop() const;
	void SetMaximized(const bool& set);
	const bool GetMaximized();
	void SetBrightness(float& set);
	const float GetBrightness() const;

	void SetVsync(const bool& set);
	const bool GetVsync() const;

	void SetWindowIcon(const char* filepath);

	Cursor* GetCursor() const;

	WindowNaming* GetWindowNamer() const;
	SDL_Window* GetWindow() const;
	SDL_Surface* GetSurface() const;

private:
	SDL_Window* GenerateWindow(SDL_Window* window, SDL_Surface* surface, const char* name, Uint32 flags, float2 pos = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED }, float2 size = { 500, 500 }, float brightness = 1.0f);

	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	// Window namer
	WindowNaming* window_namer = nullptr;

private:
	Cursor* cursor = nullptr;

	int     width = 0;
	int     height = 0;
	bool    fullscreen = false;
	bool    resizable = false;
	bool    borderless = false;
	bool    full_dekstop = false;
	bool    maximized = true;
	float   brightness = 0.0f;
	bool    vsync = false;
};

#endif // !__MODULE_WINDOW_H__