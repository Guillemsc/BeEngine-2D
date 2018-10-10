#include "Globals.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "Cursor.h"
#include "ModuleJson.h"

ModuleWindow::ModuleWindow() : Module()
{
	window = NULL;
	screen_surface = NULL;

	window_namer = new WindowNaming();

	width = 1280;
	height = 1024;
	fullscreen = false;
	resizable = true;
	borderless = false;
	full_dekstop = false;
	maximized = true;
	vsync = true;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

void ModuleWindow::OnLoadConfig(JSON_Doc * config)
{
	width = config->GetNumber("window.width", 900);
	height = config->GetNumber("window.height", 900);
	fullscreen = config->GetBool("window.fullscreen", false);
	resizable = config->GetBool("window.resizable", true);
	borderless = config->GetBool("window.borderless", false);
	full_dekstop = config->GetBool("window.fulldekstop", false);
	maximized = config->GetBool("window.maximized", false);
	SetVsync(config->GetBool("window.vsync", true));
	brightness = config->GetNumber("window.brightness", 1.0f);
}

void ModuleWindow::OnSaveConfig(JSON_Doc * config)
{
	config->SetNumber("window.width", width);
	config->SetNumber("window.height", height);
	config->SetBool("window.fullscreen", fullscreen);
	config->SetBool("window.resizable", resizable);
	config->SetBool("window.borderless", borderless);
	config->SetBool("window.fulldekstop", full_dekstop);
	config->SetBool("window.maximized", maximized);
	config->SetNumber("window.brightness", GetBrightness());
	config->SetBool("window.vsync", GetVsync());
}

// Called before render is available
bool ModuleWindow::Awake()
{
	bool ret = true;

	INTERNAL_LOG("Init SDL window & surface");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Use OpenGL 4.5
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		INTERNAL_LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		Uint32 flags =	 0;

		if (fullscreen)		
			flags += SDL_WINDOW_FULLSCREEN;

		if (resizable)		
			flags += SDL_WINDOW_RESIZABLE;

		if (borderless)		
			flags += SDL_WINDOW_BORDERLESS;

		if (full_dekstop)	
			flags += SDL_WINDOW_FULLSCREEN_DESKTOP;

		if (maximized)		
			flags += SDL_WINDOW_MAXIMIZED;

		window = GenerateWindow(window, screen_surface, App->GetAppName(), flags, float2(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED), float2(1280, 720), brightness);

		SetWindowIcon("spooky.bmp");

		if (window == nullptr)
			ret = false;
	}
	
	// Create cursor
	cursor = new Cursor();

	return ret;
}

bool ModuleWindow::Update()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	bool ret = true;

	INTERNAL_LOG("Destroying SDL window and quitting all SDL systems");

	cursor->CleanUp();
	RELEASE(cursor);

	RELEASE(window_namer);

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();

	return ret;
}

SDL_Window* ModuleWindow::GenerateWindow(SDL_Window* window, SDL_Surface* surface, const char* name, Uint32 flags, float2 pos, float2 size, float brightness)
{
	flags += SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	window = SDL_CreateWindow(App->GetAppName(), pos.x, pos.y, size.x, size.y, flags);

	if (window == NULL)
	{
		INTERNAL_LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return nullptr;
	}
	else
	{
		surface = SDL_GetWindowSurface(window);
		
		if (brightness > 1) 
			brightness = 1;

		else if (brightness < 0) 
			brightness = 0;

		SDL_SetWindowBrightness(window, brightness);
	}

	return window;
}

void ModuleWindow::SetWindowTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetWindowSize(const int _width, const int _height)
{
	if (_width > 0 && _height > 0)
	{
		width = _width;

		height = _height;
		SDL_SetWindowSize(window, width, height);

		if(!fullscreen)
			App->renderer3D->OnResize(width, height);
	}
}

void ModuleWindow::GetWindowSize(int & width, int & height)
{
	SDL_GetWindowSize(window, &width, &height);
}

const float2 ModuleWindow::GetWindowSize()
{
	int w, h = 0;

	GetWindowSize(w, h);

	return float2(w, h);
}

void ModuleWindow::GetDisplaySize(int & width, int & height)
{
	SDL_DisplayMode dm;

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}

	width = dm.w;
	height = dm.h;
}

const float2 ModuleWindow::GetDisplaySize()
{
	int x, y;
	GetDisplaySize(x, y);

	return float2(x, y);
}

void ModuleWindow::SetFullscreen(const bool& set)
{
	fullscreen = set;

	Uint32 flags;

	if (fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	SDL_SetWindowFullscreen(window, flags);
}

const bool ModuleWindow::GetFullscreen() const
{
	return fullscreen;
}

void ModuleWindow::SetResizable(const bool& set)
{
	resizable = set;
}

const bool ModuleWindow::GetResizable() const
{
	return resizable;
}

void ModuleWindow::SetBorderless(const bool& set)
{
	if (!fullscreen && !full_dekstop)
	{
		borderless = set;
		SDL_SetWindowBordered(window, (SDL_bool)!set);
	}
}

const bool ModuleWindow::GetBorderless() const
{
	return borderless;
}

void ModuleWindow::SetFullDekstop(const bool& set)
{
	full_dekstop = set;

	if (full_dekstop)
	{
		SetFullscreen(false);

		if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
		{
			full_dekstop = false;
		}
	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
	}
}

const bool ModuleWindow::GetFullDekstop() const
{
	return full_dekstop;
}

void ModuleWindow::SetMaximized(const bool& set)
{
	maximized = set;

	if(set)
		SDL_MaximizeWindow(window);

	else
		SDL_RestoreWindow(window);
}

const bool ModuleWindow::GetMaximized()
{
	Uint32 flags = SDL_GetWindowFlags(window);

	if (flags & SDL_WINDOW_MAXIMIZED)
		maximized = true;
	else
		maximized = false;

	return maximized;
}

void ModuleWindow::SetBrightness(float& set)
{
	if (set > 1)
		set = 1;

	else if (set < 0)
		set = 0;

	brightness = set;

	SDL_SetWindowBrightness(window, set);
}

const float ModuleWindow::GetBrightness() const
{
	return SDL_GetWindowBrightness(window);
}

void ModuleWindow::SetVsync(const bool& set)
{
	vsync = set;

	SDL_GL_SetSwapInterval(vsync);
}

const bool ModuleWindow::GetVsync() const
{
	return vsync;
}

void ModuleWindow::SetWindowIcon(const char * filepath)
{
	SDL_Surface* icon = SDL_LoadBMP("spooky.bmp");
	SDL_SetWindowIcon(window, icon);
}

Cursor * ModuleWindow::GetCursor() const
{
	return cursor;
}

WindowNaming * ModuleWindow::GetWindowNamer() const
{
	return window_namer;
}

SDL_Window * ModuleWindow::GetWindow() const
{
	return window;
}

SDL_Surface * ModuleWindow::GetSurface() const
{
	return screen_surface;
}

WindowNaming::WindowNaming()
{
}

void WindowNaming::AddNamePart(std::string identifier, std::string value)
{
	bool found = false;
	for (std::vector<NamePart>::iterator it = name_parts.begin(); it != name_parts.end(); ++it)
	{
		if ((*it).identifier.compare(identifier) == 0)
		{
			found = true;
			(*it).val = value;
			break;
		}
	}

	if (!found)
	{
		NamePart part;
		part.identifier = identifier;
		part.val = value;

		name_parts.push_back(part);
	}

	UpdateWindowName();
}

void WindowNaming::UpdateNamePart(std::string identifier, std::string value)
{
	for (std::vector<NamePart>::iterator it = name_parts.begin(); it != name_parts.end(); ++it)
	{
		if ((*it).identifier.compare(identifier) == 0)
		{
			(*it).val = value;
			break;
		}
	}

	UpdateWindowName();
}

void WindowNaming::UpdateWindowName()
{
	std::string name;

	for (std::vector<NamePart>::iterator it = name_parts.begin(); it != name_parts.end(); ++it)
	{
		name += (*it).val;

		name += " ";
	}

	App->window->SetWindowTitle(name.c_str());
}
