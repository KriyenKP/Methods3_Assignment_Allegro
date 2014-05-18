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
		return false;
	}
}

// Check if multiple keys are pressed
bool InputManager::IsKeyPressed(ALLEGRO_EVENT ev, std::vector<int> keys){
	if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
		for (int i = 0; i < keys.size(); i++){
			if (ev.keyboard.keycode == keys[i]){
				return true;
			}
		}
		return false;
	}
}

// Check if a single key is released
bool InputManager::IsKeyReleased(ALLEGRO_EVENT ev, int key){
	if (ev.type == ALLEGRO_EVENT_KEY_UP){
		if (ev.keyboard.keycode == key){
			return true;
		}
		return false;
	}
}

// Check if multiple keys are released
bool InputManager::IsKeyReleased(ALLEGRO_EVENT ev, std::vector<int> keys){
	if (ev.type == ALLEGRO_EVENT_KEY_UP){
		for (int i = 0; i < keys.size(); i++){
			if (ev.keyboard.keycode == keys[i]){
				return true;
			}
		}
		return false;
	}
}