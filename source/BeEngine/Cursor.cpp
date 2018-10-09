#include "Cursor.h"

Cursor::Cursor()
{
	Arrow();
}

Cursor::~Cursor()
{
}

void Cursor::Arrow()
{
	if (arrow == nullptr)
		arrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

	cursor_to_set = arrow;
}

void Cursor::Ibeam()
{
	if (ibeam == nullptr)
		ibeam = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);

	cursor_to_set = ibeam;
}

void Cursor::Wait()
{
	if (wait == nullptr)
		wait = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);

	cursor_to_set = wait;
}

void Cursor::Crosshair()
{
	if (crosshair == nullptr)
		crosshair = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);

	cursor_to_set = crosshair;
}

void Cursor::SizeNWSE()
{
	if (sizenwse == nullptr)
		sizenwse = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);

	cursor_to_set = sizenwse;
}

void Cursor::SizeNESW()
{
	if (sizenesw == nullptr)
		sizenesw = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);

	cursor_to_set = sizenesw;
}

void Cursor::SizeWE()
{
	if (sizewe == nullptr)
		sizewe = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);

	cursor_to_set = sizewe;
}

void Cursor::SizeNS()
{
	if (sizens == nullptr)
		sizens = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);

	cursor_to_set = sizens;
}

void Cursor::SizeAll()
{
	if (sizeall == nullptr)
		sizeall = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);

	cursor_to_set = sizeall;
}

void Cursor::Hand()
{
	if (hand == nullptr)
		hand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

	cursor_to_set = hand;
}

void Cursor::SetShowCursor(bool set)
{
	show = set;
}

void Cursor::SetCurrentCursor()
{
	if (current_cursor != cursor_to_set && cursor_to_set != nullptr)
	{
		SDL_SetCursor(cursor_to_set);
		current_cursor = cursor_to_set;
	}

	if (cursor_to_set == nullptr)
	{
		Arrow();
		SDL_SetCursor(cursor_to_set);
		current_cursor = cursor_to_set;
	}

	cursor_to_set = nullptr;
	
	SDL_ShowCursor(show);
}

void Cursor::CleanUp()
{
	if (arrow != nullptr)
		SDL_FreeCursor(arrow);

	if (ibeam != nullptr)
		SDL_FreeCursor(ibeam);

	if (wait != nullptr)
		SDL_FreeCursor(wait);

	if (crosshair != nullptr)
		SDL_FreeCursor(crosshair);

	if (sizenwse != nullptr)
		SDL_FreeCursor(sizenwse);

	if (sizenesw != nullptr)
		SDL_FreeCursor(sizenesw);

	if (sizewe != nullptr)
		SDL_FreeCursor(sizewe);

	if (sizens != nullptr)
		SDL_FreeCursor(sizens);

	if (sizeall != nullptr)
		SDL_FreeCursor(sizeall);

	if (hand != nullptr)
		SDL_FreeCursor(hand);
}