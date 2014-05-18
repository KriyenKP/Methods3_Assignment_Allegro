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

		// old code... 

		//	fired = false;										//Bullet Fired False. 
		//	switch (ev.keyboard.keycode)						//Switch keyboard code returned. 
		//	{
		//	case ALLEGRO_KEY_UP:				
		//		keys[UP] = true;
		//		if (direction == 1) select = player_img[2];			//set character sprite to ____
		//		else select = player_img[3];
		//		break;
		//	case ALLEGRO_KEY_W:										//button press actions for sprite dir
		//		keys[UP] = true;
		//		if (direction == 1) select = player_img[2];
		//		else select = player_img[3];
		//		break;
		//	case ALLEGRO_KEY_DOWN:
		//		keys[DOWN] = true;
		//		if (direction == 1) select = player_img[2];
		//		else select = player_img[3];
		//		break;
		//	case ALLEGRO_KEY_S:
		//		keys[DOWN] = true;
		//		if (direction == 1) select = player_img[2];
		//		else select = player_img[3];
		//		break;
		//	case ALLEGRO_KEY_RIGHT:
		//		keys[RIGHT] = true;
		//		select = player_img[2];
		//		direction = 1;
		//		break;
		//	case ALLEGRO_KEY_N:
		//		if (egg < 1 && state == SETTINGS)
		//		{
		//			egg++;
		//		}
		//		break;
		//	case ALLEGRO_KEY_M:
		//		if (egg < 2 && state == SETTINGS)
		//		{
		//			egg++;
		//		}
		//		break;
		//	case ALLEGRO_KEY_K:
		//		egg = 0;
		//		break;
		//	case ALLEGRO_KEY_D:
		//		keys[RIGHT] = true;
		//		select = player_img[2];
		//		direction = 1;
		//		break;
		//	case ALLEGRO_KEY_LEFT:
		//		keys[LEFT] = true;
		//		select = player_img[3];
		//		direction = 0;
		//		break;
		//	case ALLEGRO_KEY_A:
		//		keys[LEFT] = true;
		//		select = player_img[3];
		//		direction = 0;
		//		break;
		//	case ALLEGRO_KEY_ENTER:
		//		break;
		//	case ALLEGRO_KEY_SPACE:
		//		keys[SPACE] = true;
		//		if (state == TITLE)
		//			ChangeState(state, MENU);					//Splash->Menu on Spacebar press
		//		else if (state == MENU)
		//			ChangeState(state, PLAYING);				//Menu-> Game if Spacebar press
		//		else if (state == HELP)
		//			ChangeState(state, MENU);					//Help-> Menu if spacebar press
		//		else if (state == PLAYING)						//Spacebar fires bullets in-game
		//		{	
		//			FireBullet(bullets, NUM_BULLETS, player);
		//			//al_play_sample(sample[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); <--What did this do? 
		//		}
		//		else if (state == LOST)
		//			ChangeState(state, PLAYING);				
		//		break;
		//	}
		//}

		//else if (ev.type == ALLEGRO_EVENT_KEY_UP)					//If Key up event
		//{
		//	switch (ev.keyboard.keycode)
		//	{
		//	case ALLEGRO_KEY_UP:									//Keypress sprite and dir updates
		//		keys[UP] = false;
		//		if (direction == 1) select = player_img[0];
		//		else select = player_img[1];
		//		break;
		//	case ALLEGRO_KEY_W:
		//		keys[UP] = false;
		//		if (direction == 1) select = player_img[0];
		//		else select = player_img[1];
		//		break;
		//	case ALLEGRO_KEY_DOWN:
		//		keys[DOWN] = false;
		//		if (direction == 1) select = player_img[0];
		//		else select = player_img[1];
		//		break;
		//	case ALLEGRO_KEY_S:
		//		keys[DOWN] = false;
		//		if (direction == 1) select = player_img[0];
		//		else select = player_img[1];
		//		break;
		//	case ALLEGRO_KEY_RIGHT:
		//		keys[RIGHT] = false;
		//		select = player_img[0];
		//		break;
		//	case ALLEGRO_KEY_D:
		//		keys[RIGHT] = false;
		//		select = player_img[0];
		//		break;
		//	case ALLEGRO_KEY_LEFT:
		//		keys[LEFT] = false;
		//		select = player_img[1];
		//		break;
		//	case ALLEGRO_KEY_A:
		//		select = player_img[1];
		//		break;
		//	case ALLEGRO_KEY_P:
		//		if (state == PLAYING)					//Check state		-- THIS FUNCTION NEEDS REPAIRS	
		//		{
		//			if (timeM == true)
		//			{
		//				al_stop_timer(timer);									//Stop timer for pause menu
		//				timeM = false;
		//				al_draw_bitmap(scrns[1], scrn_W / 2 - 250, 100, 0);		//Show Pause menu
		//				al_flip_display();										//Bring backbuffer forward (bring all set contents to the current frame)
		//			}
		//			else
		//			{
		//				al_start_timer(timer);									//Continue timer
		//				timeM = true;
		//			}
		//		}
		//		break;
		//	case ALLEGRO_KEY_BACKSPACE:
		//		if (timeM == false)										//if game on pause, BKSP to menu
		//		{
		//			timeM = true;
		//			al_start_timer(timer);
		//		}
		//			ChangeState(state, MENU);
		//		break;
		//	case ALLEGRO_KEY_ENTER:
		//		if (state == WIN)
		//			boss_sel = lecturers[rand() % 6];				//Random Boss character selected
		//			ChangeState(state, PLAYING);
		//		break;
		//	case ALLEGRO_KEY_ESCAPE:								
		//		if (state==TITLE || state == MENU || state == HELP)
		//			done = true;
		//		else 
		//			ChangeState(state, MENU);
		//		break;
		//	case ALLEGRO_KEY_SPACE:
		//		keys[SPACE] = false;
		//		break;
		//	case ALLEGRO_KEY_K:
		//			egg = 0;
		//		break;
		//	case ALLEGRO_KEY_M:
		//		//	egg = 0;
		//		break;
		//	case ALLEGRO_KEY_N:
		//		//	egg = 0;
		//		break;
		//	}


} // end update keys
