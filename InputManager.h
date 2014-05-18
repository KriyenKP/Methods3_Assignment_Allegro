#pragma once

#include <allegro5\allegro.h>
#include <allegro5\keyboard.h>
#include <vector>

class InputManager
{
private:
	ALLEGRO_KEYBOARD_STATE keyState;
public:
	InputManager();
	~InputManager();

	void IsKeyPressed(ALLEGRO_EVENT ev, int key);
	void IsKeyPressed(ALLEGRO_EVENT ev);
};

