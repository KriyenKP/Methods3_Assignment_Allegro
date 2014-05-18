#include "InputManager.h"


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

// Check if a single key is pressed
bool InputManager::IsKeyPressed(ALLEGRO_EVENT ev, int key){
	if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
		if (ev.keyboard.keycode == key){
			return true;
		}
	}
	return false;
}


// Check if a single key is released
bool InputManager::IsKeyReleased(ALLEGRO_EVENT ev, int key){
	if (ev.type == ALLEGRO_EVENT_KEY_UP){
		if (ev.keyboard.keycode == key){
			return true;
		}
	}
	return false;
}
