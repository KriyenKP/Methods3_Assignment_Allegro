#pragma once

#include <allegro5\allegro.h>
#include <allegro5\keyboard.h>
#include <vector>

//#include "init.h" // used for the constants that make life easier :) 

class InputManager
{
private:
	ALLEGRO_KEYBOARD_STATE keyState;
public:
	InputManager(void);
	~InputManager(void);


	bool IsKeyPressed(ALLEGRO_EVENT ev, int key);

	bool IsKeyReleased(ALLEGRO_EVENT ev, int key);

	void UpdateKeys(ALLEGRO_EVENT ev, bool keys[]);
};

