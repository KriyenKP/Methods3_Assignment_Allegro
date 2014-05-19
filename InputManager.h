#pragma once

#include <allegro5\allegro.h>
#include <allegro5\keyboard.h>

enum KEYS { UP, DOWN, LEFT, RIGHT, S_UP, S_DOWN, S_LEFT, S_RIGHT, SPACE, ENTER, PAUSE, ESC, BKSPCE };				//Key press shortcut, defines key constants 

class InputManager
{
private:
	ALLEGRO_KEYBOARD_STATE keyState;
public:
	InputManager(void);
	~InputManager(void);

	void UpdateKeys(ALLEGRO_EVENT ev, bool keys[]);
};

