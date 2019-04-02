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

struct Key
{
	int         key;
	KEY_STATE   state = KEY_IDLE;

	bool operator == (const Key& bin)
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

	void ManageInput();

	const bool GetKeyDown(int id);
	const bool GetKeyRepeat(int id);
	const bool GetKeyUp(int id);
	const bool GetKeyDown(const char* key);
	const bool GetKeyRepeat(const char* key);
	const bool GetKeyUp(const char* key);

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
	void AddKeyDown(const Key& k);
	void AddKeyRepeat(const Key& k);
	void AddKeyUp(const Key& k);

private:
	// Those lists are filled and cleaned every frame
	std::vector<Key> keys_down;
	std::vector<Key> keys_repeat;
	std::vector<Key> keys_up;

	// Used to reference bindings and keys and keep track of states
	Key*		       keyboard;

	bool			   windowEvents[WE_COUNT];
	KEY_STATE		   mouse_buttons[MAX_MOUSE_BUTTONS];

	std::string	       text_input;

	int				   mouse_x = 0;
	int				   mouse_y = 0;
	int				   last_mouse_x = 0;
	int				   last_mouse_y = 0;

	int				   mouse_wheel = 0;
};

#endif // !__MODULE_INPUT_H__