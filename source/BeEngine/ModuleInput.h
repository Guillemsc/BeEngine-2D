#ifndef __MODULE_INPUT_H__
#define __MODULE_INPUT_H__

#include "Module.h"
#include "Globals.h"
#include "Functions.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP,
};

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

struct KeyBinding
{
	const char* binding_name = nullptr;
	int         key;
	KEY_STATE   state = KEY_IDLE;

	bool operator == (const KeyBinding& bin)
	{
		if (key == bin.key)
			return true;
		return false;
	}
};

class ModuleInput : public Module
{
public:
	ModuleInput();
	~ModuleInput();

	void OnLoadConfig(JSON_Doc* config);
	void OnSaveConfig(JSON_Doc* config);
	bool Awake();
	bool PreUpdate();
	bool CleanUp();

	const bool GetKeyDown(int id);
	const bool GetKeyRepeat(int id);
	const bool GetKeyUp(int id);
	const bool GetKeyDown(const char* key);
	const bool GetKeyRepeat(const char* key);
	const bool GetKeyUp(const char* key);

	bool GetKeyBindingDown(const char* binding_name);
	bool GetKeyBindingRepeat(const char* binding_name);
	bool GetKeyBindingUp(const char* binding_name);
	void SetKeyBinding(const char* key, const char* binding_name);
	const char* GetKeyBinding(const char* binding_name);

	bool GetKeyboardInput(std::string& input);
	void ClearKeyboardInput();

	KEY_STATE GetMouseButton(int id) const;

	bool GetWindowEvent(EventWindow ev);

	int GetMouseX() const;
	int GetMouseY() const;
	float2 GetMouse();
	int GetMouseWheel() const;
	int GetMouseXMotion() const;
	int GetMouseYMotion() const;

	int CharToKey(const char* key);
	const char* KeyToChar(int key);

private:
	void AddKeyDown(KeyBinding k);
	void AddKeyRepeat(KeyBinding k);
	void AddKeyUp(KeyBinding k);

private:
	// Those lists are filled and cleaned every frame
	std::vector<KeyBinding> keys_down;
	std::vector<KeyBinding> keys_repeat;
	std::vector<KeyBinding> keys_up;

	// Used to reference bindings and keys and keep track of states
	KeyBinding*		   keyboard;

	bool			   windowEvents[WE_COUNT];
	KEY_STATE		   mouse_buttons[MAX_MOUSE_BUTTONS];

	std::string	       text_input;

	int				   mouse_x = 0;
	int				   mouse_y = 0;
	int				   mouse_wheel = 0;
	int				   mouse_x_motion = 0;
	int				   mouse_y_motion = 0;

public:
	bool			   right_clicking = false;
};

#endif // !__MODULE_INPUT_H__