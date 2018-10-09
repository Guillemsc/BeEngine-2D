#ifndef __CURSOR_H__
#define __CURSOR_H__

#include "SDL\include\SDL.h"

class Cursor
{
public:
	Cursor();
	~Cursor();

	void Arrow();
	void Ibeam();
	void Wait();
	void Crosshair();
	void SizeNWSE();
	void SizeNESW();
	void SizeWE();
	void SizeNS();
	void SizeAll();
	void Hand();
	void SetShowCursor(bool set);

	void SetCurrentCursor();

	void CleanUp();

private:
	SDL_Cursor* arrow = nullptr;
	SDL_Cursor* ibeam = nullptr;
	SDL_Cursor* wait = nullptr;
	SDL_Cursor* crosshair = nullptr;
	SDL_Cursor* sizenwse = nullptr;
	SDL_Cursor* sizenesw = nullptr;
	SDL_Cursor* sizewe = nullptr;
	SDL_Cursor* sizens = nullptr;
	SDL_Cursor* sizeall = nullptr;
	SDL_Cursor* hand = nullptr;

	SDL_Cursor* cursor_to_set = nullptr;
	SDL_Cursor* current_cursor = nullptr;

	bool show = true;
};

#endif //__CURSOR_H__