#include "Globals.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"

// SDL_GetScancode: https://wiki.libsdl.org/SDL_Keycode
// To use PS3 Controller install this driver https://github.com/nefarius/ScpToolkit/releases/tag/v1.6.238.16010

#define MAX_KEYS 300

ModuleInput::ModuleInput() : Module()
{
	keyboard = new KeyBinding[MAX_KEYS];

	for (int i = 0; i < MAX_KEYS; ++i)
		keyboard[i].key = i;
}

// Destructor
ModuleInput::~ModuleInput()
{

}

void ModuleInput::OnLoadConfig(JSON_Doc* config)
{

}

void ModuleInput::OnSaveConfig(JSON_Doc * config)
{

}

// Called before render is available
bool ModuleInput::Awake()
{
	bool ret = true;

	INTERNAL_LOG("Init SDL input event system");

	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		INTERNAL_LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		INTERNAL_LOG("Error on SDL_Init_Video");
		ret = false;
	}

	if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
	{
		INTERNAL_LOG("Error on SDL_Init_GameController");
		ret = false;
	}

	return ret;
}

// Called every draw update
bool ModuleInput::PreUpdate()
{
	bool ret = true;

	SDL_PumpEvents();
	SDL_StartTextInput();

	text_input.clear();
	keys_down.clear();
	keys_repeat.clear();
	keys_up.clear();

	last_mouse_x = mouse_x;
	last_mouse_y = mouse_y;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i].state == KEY_IDLE)
			{
				keyboard[i].state = KEY_DOWN;
				AddKeyDown(keyboard[i]);
			}
			else
			{
				keyboard[i].state = KEY_REPEAT;
				AddKeyRepeat(keyboard[i]);
			}
		}
		else
		{
			if (keyboard[i].state == KEY_REPEAT || keyboard[i].state == KEY_DOWN)
			{
				keyboard[i].state = KEY_UP;
				AddKeyUp(keyboard[i]);
			}
			else
			{
				keyboard[i].state = KEY_IDLE;
			}
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_wheel = 0;

	for (int i = 0; i < 5; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}
	
	bool quit = false;
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		App->editor->EditorInput(e);

		switch (e.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (e.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			case SDL_WINDOWEVENT_RESIZED:
				App->window->SetWindowSize(e.window.data1, e.window.data2);
				break;
			}

			break;

		case SDL_MOUSEWHEEL:
			mouse_wheel = e.wheel.y;
			break;

		case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;
			break;

		case SDL_TEXTINPUT:
			text_input.insert(text_input.size(), e.text.text);
			break;

		case SDL_DROPFILE:
			std::string file = e.drop.file;

			// Drag drop load

			SDL_free(e.drop.file);
			break;
		}
	}
	
	return ret;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	bool ret = true;

	INTERNAL_LOG("Quitting SDL input event subsystem");

	delete[] keyboard;

	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return ret;
}

const bool ModuleInput::GetKeyDown(int id)
{
	if (!keys_down.empty())
	{
		for (std::vector<KeyBinding>::iterator it = keys_down.begin(); it != keys_down.end(); it++)
		{
			if (id == (*it).key)
				return true;
		}
	}

	return false;
}

const bool ModuleInput::GetKeyRepeat(int id)
{
	if (!keys_repeat.empty())
	{
		for (std::vector<KeyBinding>::iterator it = keys_repeat.begin(); it != keys_repeat.end(); it++)
		{
			if (id == (*it).key)
				return true;
		}
	}

	return false;
}

const bool ModuleInput::GetKeyUp(int id)
{
	if (!keys_up.empty())
	{
		for (std::vector<KeyBinding>::iterator it = keys_up.begin(); it != keys_up.end(); it++)
		{
			if (id == (*it).key)
				return true;
		}
	}

	return false;
}

const bool ModuleInput::GetKeyDown(const char * key)
{
	return GetKeyDown(CharToKey(key));
}

const bool ModuleInput::GetKeyRepeat(const char * key)
{
	return GetKeyRepeat(CharToKey(key));
}

const bool ModuleInput::GetKeyUp(const char * key)
{
	return GetKeyUp(CharToKey(key));
}

bool ModuleInput::GetKeyBindingDown(const char * binding_name)
{
	if (!keys_down.empty())
	{
		for (std::vector<KeyBinding>::iterator it = keys_down.begin(); it != keys_down.end(); it++)
		{
			if (TextCmp(binding_name, (*it).binding_name))
				return true;
		}
	}

	return false;
}

bool ModuleInput::GetKeyBindingRepeat(const char * binding_name)
{
	if (!keys_repeat.empty())
	{
		for (std::vector<KeyBinding>::iterator it = keys_repeat.begin(); it != keys_repeat.end(); it++)
		{
			if (TextCmp(binding_name, (*it).binding_name))
				return true;
		}
	}
}

bool ModuleInput::GetKeyBindingUp(const char * binding_name)
{
	if (!keys_up.empty())
	{
		for (std::vector<KeyBinding>::iterator it = keys_up.begin(); it != keys_up.end(); it++)
		{
			if (TextCmp(binding_name, (*it).binding_name))
				return true;
		}
	}

	return false;
}

void ModuleInput::SetKeyBinding(const char * key, const char * binding_name)
{
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (TextCmp(keyboard[i].binding_name, binding_name))
		{
			keyboard[i].binding_name = "";
		}

		if (i == CharToKey(key))
		{
			keyboard[i].binding_name = binding_name;
		}
	}
}

const char * ModuleInput::GetKeyBinding(const char * binding_name)
{
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (TextCmp(keyboard[i].binding_name, binding_name))
		{
			return KeyToChar(keyboard[i].key);
		}
	}

	return "";
}

bool ModuleInput::GetKeyboardInput(std::string& input)
{
	if (!text_input.empty())
	{
		input = text_input;
		return true;
	}

	return false;
}

void ModuleInput::ClearKeyboardInput()
{
	text_input.clear();
}

KEY_STATE ModuleInput::GetMouseButton(int id) const
{
	return mouse_buttons[id];
}

int ModuleInput::CharToKey(const char * key)
{
	return SDL_GetScancodeFromKey(SDL_GetKeyFromName(key));
}

const char * ModuleInput::KeyToChar(int key)
{
	return SDL_GetScancodeName((SDL_Scancode)key);
}

void ModuleInput::AddKeyDown(KeyBinding k)
{
	keys_down.push_back(k);
}

void ModuleInput::AddKeyRepeat(KeyBinding k)
{
	keys_repeat.push_back(k);
}

void ModuleInput::AddKeyUp(KeyBinding k)
{
	keys_up.push_back(k);
}

bool ModuleInput::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

int ModuleInput::GetMouseX() const
{
	return mouse_x;
}

int ModuleInput::GetMouseY() const
{
	return mouse_y;
}

float2 ModuleInput::GetMouse()
{
	return float2(mouse_x, mouse_y);
}

int ModuleInput::GetMouseWheel() const
{
	return mouse_wheel;
}

int ModuleInput::GetMouseXMotion() const
{
	return mouse_x - last_mouse_x;
}

int ModuleInput::GetMouseYMotion() const
{
	return  mouse_y - last_mouse_y;
}
