/*	InputManager.cpp - Lecturer Defence Game - Computer Methods 3 - ENEL3CC - UKZN 2014
Author: R. Mawbey - 213560382
Date: 19/5/2014
Description:
Abstracts the keyboard key processing, takes an array of booleans corosponding to selected keys and updates if they are pressed or not. 
See header for keys. 

*/

#include "InputManager.h"


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}


// Update the state of keys in a boolean array
void InputManager::UpdateKeys(ALLEGRO_EVENT ev, bool keys[]){
	if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
		switch (ev.keyboard.keycode)						//Switch keyboard code returned. 
		{
			// Movement
		case ALLEGRO_KEY_W:
			keys[UP] = true;
			break;
		case ALLEGRO_KEY_A:
			keys[LEFT] = true;
			break;
		case ALLEGRO_KEY_S:
			keys[DOWN] = true;
			break;
		case ALLEGRO_KEY_D:
			keys[RIGHT] = true;
			break;

			// shoot keys
		case ALLEGRO_KEY_UP:
			keys[S_UP] = true;
			break;
		case ALLEGRO_KEY_DOWN:
			keys[S_DOWN] = true;
			break;
		case ALLEGRO_KEY_RIGHT:
			keys[S_RIGHT] = true;
			break;
		case ALLEGRO_KEY_LEFT:
			keys[S_LEFT] = true;
			break;
		case ALLEGRO_KEY_SPACE:
			keys[SPACE] = true;
			break;

			// control
		case ALLEGRO_KEY_ENTER:
			keys[ENTER] = true;
			break;
		case ALLEGRO_KEY_P:
			keys[PAUSE] = true;
			break;
		case ALLEGRO_KEY_ESCAPE:
			keys[ESC] = true;
			break;
		case ALLEGRO_KEY_BACKSPACE:
			keys[BKSPCE] = true;
			break;

		} // Keydown detect
	}
	else if (ev.type == ALLEGRO_EVENT_KEY_UP)					//If Key up event
	{
		switch (ev.keyboard.keycode)
		{
			// Movement
		case ALLEGRO_KEY_W:
			keys[UP] = false;
			break;
		case ALLEGRO_KEY_A:
			keys[LEFT] = false;
			break;
		case ALLEGRO_KEY_S:
			keys[DOWN] = false;
			break;
		case ALLEGRO_KEY_D:
			keys[RIGHT] = false;
			break;

			// shoot keys
		case ALLEGRO_KEY_UP:
			keys[S_UP] = false;
			break;
		case ALLEGRO_KEY_DOWN:
			keys[S_DOWN] = false;
			break;
		case ALLEGRO_KEY_RIGHT:
			keys[S_RIGHT] = false;
			break;
		case ALLEGRO_KEY_LEFT:
			keys[S_LEFT] = false;
			break;
		case ALLEGRO_KEY_SPACE:
			keys[SPACE] = false;
			break;

			// control
		case ALLEGRO_KEY_ENTER:
			keys[ENTER] = false;
			break;
		case ALLEGRO_KEY_P:
			keys[PAUSE] = false;
			break;
		case ALLEGRO_KEY_ESCAPE:
			keys[ESC] = false;
			break;
		case ALLEGRO_KEY_BACKSPACE:
			keys[BKSPCE] = false;
			break;

		} // keyup detect switch
	}// end if
}