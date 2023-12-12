#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL_scancode.h"

#define NUM_MOUSE_BUTTONS 5

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

	// Check key states
	const unsigned GetKey(int id) const 
	{ 
		return keyboard[id]; 
	}

	bool GetMouseButton(int id) const { return mouse_buttons[id-1]; }
	int GetMouseDeltaX() const { return mouse_motion_x; }
	int GetMouseDeltaY() const { return mouse_motion_y; }
	int GetMouseScroll() const { return mouse_scroll; }

private:
	const Uint8 *keyboard = NULL;
	bool mouse_buttons[NUM_MOUSE_BUTTONS] = { false };
	int mouse_motion_x, mouse_motion_y;
	int mouse_scroll;
};