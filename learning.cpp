#include <math.h>								//Nothing yet - put in for mouse cursor check if we use it
#include <allegro5/allegro.h>					//Allegro
#include <allegro5/allegro_image.h>				//Allegro bitmaps
#include <allegro5/allegro_native_dialog.h>		//Message Dialog
#include <allegro5/allegro_font.h>				//Needed for fonts 
#include <allegro5/allegro_ttf.h>				//Needed for fonts
#include <lib/objects.h>						//Structures for Enemies/Characters/Projectiles
#include <lib/init.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>				//Audio yet to be used	
#include <cstdio>								//Input/output - Used for displaying mouse pos atm
#include <sstream>
#include <allegro5/allegro_primitives.h>		//Used for drawing Shapes
using namespace std;

//REMEMBER TO EDIT Linker -> System -> SubSystem -> WINDOW to hide console!

//asset Init
Character player;	
Bullet bullets[NUM_BULLETS];
Projectile comets[NUM_COMETS];
Projectile powerUp[NUM_POWER];
Boss bossy[NUM_BOSS];
Explosion explosions[NUM_EXPLOSIONS];
//End asset init

bool keys[10] = { false, false, false, false, false, false, false, false, false, false }; // Keystate array, stores the state of keys we are intersted in. True when key is pressed

//Asset Functions
void InitCharacter(Character &player);
void DrawCharacter(Character &player, ALLEGRO_BITMAP *select, int cur, int fW, int fH);

void MoveCharacterLeft(Character &player);
void MoveCharacterUp(Character &player);
void MoveCharacterDown(Character &player);
void MoveCharacterRight(Character &player);

void InitBullet(Character &player, Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size, ALLEGRO_BITMAP *bit);
void FireBullet(Bullet bullet[], int size, Character &player);
void UpdateBullet(Bullet bullet[], int size, int dir);
void CollideBullet(Bullet bullet[], int bSize, Projectile thrown[], int cSize, Character &ship, Explosion explosions[], int eSize, ALLEGRO_SAMPLE *sample);
void CollideBullet(Bullet bullet[], int bSize, Projectile thrown[], int cSize);
void CollideBullet(Bullet bullet[], int bSize, Boss bossy[], int cSize, Character &player, Explosion explosions[], int eSize, ALLEGRO_SAMPLE * sample);

void InitProjectile(Projectile thrown[], int size, bool s);
void DrawProjectile(Projectile thrown[], int size);
void DrawProjectile(Projectile thrown[], int size, ALLEGRO_BITMAP *bit, int cur, int fW, int fH);
void StartProjectile(Projectile thrown[], int size);
void UpdateProjectile(Projectile thrown[], int size, int bouncer);
void CollideProjectile(Projectile thrown[], int cSize, Character &player, int type);

void InitBoss(Boss bossy[], int size);
void DrawBoss(Boss bossy[], int size);
void DrawBoss(Boss bossy[], int size, ALLEGRO_BITMAP *bit, int cur, int fW, int fH);
void StartBoss(Boss bossy[], int size);
void UpdateBoss(Boss bossy[], int size);
void CollideBoss(Boss bossy[], int cSize, Character &player);

void InitExplosions(Explosion explosions[], int size, ALLEGRO_BITMAP *image);
void DrawExplosions(Explosion explosions[], int size);
void StartExplosions(Explosion explosions[], int size, int x, int y);
void UpdateExplosions(Explosion explosions[], int size);

//End asset functions

void ChangeState(int &state, int newState);   //Change State function

int main(void)
{	
	int state = -1;						//default state

	bool done	= false,				//Game over
		 fired	= false,				//Power is fired
		 redraw = true,					//Redraw frame
	     timeM	= true;					//Pause Timer  -- NEEDS TO BE FIXED


	int curLect = 0,		//current lecture identifier
		curMap = 0,			//current map identifier
		curAtk = 0;			//current attack identifier

	//Initialisers
	ALLEGRO_DISPLAY			*display		= NULL;					//Screen display
	ALLEGRO_EVENT_QUEUE		*event_queue	= NULL;					//Event Queue
	ALLEGRO_TIMER			*timer			= NULL;					//System timer
	ALLEGRO_FONT			*fonts[5]		= { NULL, NULL, NULL, NULL, NULL };	//fonts array
	ALLEGRO_STATE			*state1			= NULL;					//State
	ALLEGRO_SAMPLE			*sample[5]		= {NULL, NULL, NULL,NULL,NULL};
	ALLEGRO_BITMAP			*bgImage		= NULL,					//Title Page splash
							*player_img[5]  = {NULL,NULL, NULL, NULL, NULL},	
							*select			= NULL,					//Current Selected position of character
							*icon1			= NULL,					//Current icon -- NOT SET YET
							*numLives[3]	= { NULL, NULL, NULL},		//Lives array
							*atk[5]			= { NULL, NULL, NULL, NULL, NULL },		//Attack array
							*lockatk[5]		= { NULL, NULL, NULL, NULL, NULL },		//locked weapons
							*atksel			= NULL,
							*lecturers[6]	= {NULL, NULL, NULL, NULL, NULL, NULL},  //Lecturer array
							*minilect[6]	= { NULL, NULL, NULL, NULL, NULL, NULL },	//Lecturer Thumbnail
							*enemsel		= NULL,							//Currently selected lecturer
							*power[2]		= {NULL,NULL},
							*boss_sel		= NULL,
							*exp			= NULL,							//Explosion image
							*maps[6]		= {NULL,NULL,NULL,NULL,NULL},	//Maps array
							*mapsmini[6]	= {NULL,NULL,NULL,NULL,NULL},	//Map Thumbnails
							*mapsel			= NULL,				//Currently selected map
							*scrns[5]		= { NULL, NULL, NULL, NULL, NULL },		//Box images array (pause, gameover, etc)
							*btns[5]		= { NULL, NULL, NULL, NULL, NULL },		//Buttons Array
							*lockedmap[6]	= { NULL, NULL, NULL, NULL, NULL };		//images if map locked

	ALLEGRO_CONFIG			*savegame		= al_load_config_file("config.ini");	//inits the save game file
//	const char *unven1 = al_get_config_value(savegame, "venueunlock 1", "unlocked");	<-- test to check if config readable
//	printf("The first value for venue 1 is %s", unven1);
	bool savefile = TRUE;
	savefile = al_save_config_file("config.ini", savegame);

	if (!al_init())											//initialize and check Allegro
	{
		al_show_native_message_box(display, "Error!", "Warning!", "Failed to initialise Allegro! \n Closing Application!", NULL, ALLEGRO_MESSAGEBOX_WARN);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	display = al_create_display(scrn_W, scrn_H);			//create our display object
	if (!display)											//Check display
	{
		al_show_native_message_box(display, "Error!", "Warning!", "Failed to initialise display! \n Closing Application!", NULL, ALLEGRO_MESSAGEBOX_WARN);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);							//Create Timer
	if (!timer)													//Check timer creation
	{				
		al_show_native_message_box(display, "Error!", "Warning!", "Failed to initialise timer! \n Closing Application!", NULL, ALLEGRO_MESSAGEBOX_WARN);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	event_queue = al_create_event_queue();						//Create event queue
	if (!event_queue)											//Check event queue creation
	{											
		al_show_native_message_box(display, "Error!", "Warning!", "Failed to initialise event queue! \n Closing Application!", NULL, ALLEGRO_MESSAGEBOX_WARN);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	if (savegame == NULL || savefile == FALSE)
	{
		al_show_native_message_box(display, "Error!", "Savegame File Initialise/Save Failed!", "\n Check directory for config.ini\nClosing Application!", NULL, ALLEGRO_MESSAGEBOX_WARN);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}



	//Init all Addons
	al_init_primitives_addon();								//load primitive (drawing shapes, etc) - Needs #include <allegro5/allegro_primitives.h>
	al_init_font_addon();									//load font addon
	al_install_audio();										// load sound addon - Needs #include <allegro5/allegro_audio.h>
	al_init_acodec_addon();
	al_init_ttf_addon();									//load truetype font addon	
	al_init_image_addon();									//load image processing addon
// Should we not have error checking here? Ref http://wiki.allegro.cc/index.php?title=Basic_Keyboard_Example
	al_install_keyboard();									//install keyboard
	al_install_mouse();										//install mouse
	//end addon innit
	
	if (!al_reserve_samples(5)){
		al_show_native_message_box(display, "Error!", "Warning!", "Failed to initialise Sound samples! \n Closing Application!", NULL, ALLEGRO_MESSAGEBOX_WARN);
		return -1;
	}
	
	//cursor = al_load_bitmap("./images/target.png");
	//custom_cursor = al_create_mouse_cursor(cursor, 0, 0);
	fonts[2] = al_load_ttf_font("arial.ttf", 36, 0);				//
	fonts[1] = al_load_ttf_font("arial.ttf", 20, 0);				//Load custom font
	fonts[0] = al_load_ttf_font("arial.ttf", 18, 0);				//

	//Init images

	//Character Images
	player_img[0] = al_load_bitmap("./images/KriR.png");		
	player_img[1] = al_load_bitmap("./images/KriL.png");
	player_img[2] = al_load_bitmap("./images/KriWalkR.png");
	player_img[3] = al_load_bitmap("./images/KriWalkL.png");	

	select		= player_img[0];
	
	//Attack images (unlocked)
	atk[0]	= al_load_bitmap("./images/calc.png");
	atk[1]	= al_load_bitmap("./images/pencil.png");
	atk[2]	= al_load_bitmap("./images/c.png");
	atk[3]  = al_load_bitmap("./images/light.png");
	atksel = atk[0];

	//Attack images (locked)
	lockatk[0] = NULL;
	lockatk[1] = al_load_bitmap("./images/pencillock.png");
	lockatk[2] = al_load_bitmap("./images/clock.png");

	numLives[0]		= al_load_bitmap("./images/1.png");			//Number of Lives
	numLives[1]		= al_load_bitmap("./images/2.png");
	numLives[2]		= al_load_bitmap("./images/3.png");

	exp			= al_load_bitmap("./images/boom1.png");			//Explosions

	//Lecturer Images
	lecturers[0]	= al_load_bitmap("./images/poole.png");			//John Poole
	lecturers[1]	= al_load_bitmap("./images/saha.png");			//Akshay Saha
	lecturers[2]	= al_load_bitmap("./images/taps.png");			//Jules Tapamo
	lecturers[3]	= al_load_bitmap("./images/afullo.png");		//Afullo
	lecturers[4]	= al_load_bitmap("./images/tom.png");			//Walingo
	lecturers[5]	= al_load_bitmap("./images/viran.png");			//Viranjay

	enemsel	 = lecturers[0];										//Default selected enemy/
	boss_sel = lecturers[rand()%6];											//Default selected enemy/lecturer

	power[0] = al_load_bitmap("./images/dp.png");
	power[1] = al_load_bitmap("./images/power.png");
	
	//Lecturer Thumbnails
	minilect[0]			= al_load_bitmap("./images/poole1.png");	
	minilect[1]			= al_load_bitmap("./images/saha1.png");
	minilect[2]			= al_load_bitmap("./images/taps1.png");
	minilect[3]			= al_load_bitmap("./images/afullo1.png");
	minilect[4]			= al_load_bitmap("./images/tom1.png");
	minilect[5]			= al_load_bitmap("./images/viran1.png");
	
	//Map Images (unlocked)	
	maps[0]		= al_load_bitmap("./images/howard.png");		//Howard Building
	maps[1]		= al_load_bitmap("./images/tbdavis.png");		//TB Davis
	maps[2]		= al_load_bitmap("./images/park.png");			//The park
	maps[3]		= al_load_bitmap("./images/science.png");	//Science
	maps[4]		= al_load_bitmap("./images/cafe.png");		//cafe
	maps[5]		= al_load_bitmap("./images/amphi.png");	//amphitheatre

	//Map Images (locked)	
	lockedmap[0] = al_load_bitmap("./images/howard.png");		//Howard Building--always unlocked
	lockedmap[1] = al_load_bitmap("./images/tbdavisSlock.png");		//TB Davis
	lockedmap[2] = al_load_bitmap("./images/parkslock.png");			//The park
	lockedmap[3] = al_load_bitmap("./images/scienceslock.png");	//Science
	lockedmap[4] = al_load_bitmap("./images/cafeslock.png");		//cafe
	lockedmap[5] = al_load_bitmap("./images/amphislock.png");	//amphitheatre

	bgImage			= maps[0];								//Default selected map --always unlocked

	//Map thumbnail
	mapsmini[0]		= al_load_bitmap("./images/howards.png");		//Howard Building
	mapsmini[1]		= al_load_bitmap("./images/tbdaviss.png");		//TB Davis 
	mapsmini[2]		= al_load_bitmap("./images/parks.png");			//The park
	mapsmini[3]		= al_load_bitmap("./images/sciences.png");	//science
	mapsmini[4]		= al_load_bitmap("./images/cafes.png");			//cafe
	mapsmini[5]		= al_load_bitmap("./images/amphis.png");		//Amphitheatre

	//Button Images
	btns[0]			= al_load_bitmap("./images/startbtn.png");		//start
	btns[1]			= al_load_bitmap("./images/sttngbtn.png");		//settings
	btns[2]			= al_load_bitmap("./images/stpbtn.png");		//stop
	btns[3]			= al_load_bitmap("./images/back.png");			//back
	btns[4]			= al_load_bitmap("./images/help.png");			//png

	//Menu Images
	scrns[0]		= al_load_bitmap("./images/BG1.png");		//Title Background
	scrns[1]		= al_load_bitmap("./images/pause.png");
	scrns[2]		= al_load_bitmap("./images/gameover.png");
	scrns[3]		= al_load_bitmap("./images/config.png");
	scrns[4]		= al_load_bitmap("./images/ukzn_msc.png");
			
	sample[0]		= al_load_sample("./sounds/Pew_Pew.wav");
	sample[1]		= al_load_sample("./sounds/Evil_Laugh.wav");
	sample[2]		= al_load_sample("./sounds/victory_fanfare.wav");
	sample[3]		= al_load_sample("./sounds/boom.wav");

	icon1			= al_load_bitmap("./images/icon.png");

	int direction = 1;						//Default direction identifier init

	//Event queue - register listeners
	al_register_event_source(event_queue, al_get_timer_event_source(timer)); 
	al_register_event_source(event_queue, al_get_keyboard_event_source());				// get keyboard presses
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//end event queue

	//Colours
	black = al_map_rgb(0,0,0);
	white = al_map_rgb(255, 255, 255);
	red = al_map_rgb(255, 0, 0);
	green = al_map_rgb(0, 255, 0);
	blue = al_map_rgb(0, 0, 255);
	//End Colours

	//Asset variables
	ChangeState(state, TITLE);					//game state function
	srand(time(NULL));
	//end Asset variables

	//End initialisers
	al_set_display_icon(display, icon1);  
	al_set_window_title(display, "UKZN - LECTURE DEFENCE - HOWARD EDITION");   //set window title 
	al_start_timer(timer);											//Start event timer (program clock)
	al_clear_to_color(black);										//Clear and set Background black
	al_set_target_bitmap(al_get_backbuffer(display));				//Backbuffer--next frame to write to display
	al_flip_display();												//Allows manual switch between current disp & backbuffer

	while (!done)
	{
		ALLEGRO_EVENT ev;										//Allegro event init
		al_wait_for_event(event_queue, &ev);					//wait for and accept events 

		// game logic should not be in here? should be in timer ? I've moved it there anyway... 
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)					//If Key down event. I.E. A keyboard key has been pressed, we must process and update key array
		{
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
			} // keyup detect
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
		}
		
	
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)    // deal with mouse
		{
			crs_x = ev.mouse.x;									//get x co-ord of mouse
			crs_y = ev.mouse.y;									//get y co-ord of mouse

			//fprintf(stderr, "\nposition = x %f  y %f", crs_x,crs_y);

			if ((ev.mouse.x >= 0) && (ev.mouse.y >= 2))
			{
				al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_PRECISION);
			}
			else
			{
				al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
			}

		}

		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
		//	fprintf(stderr, "\nHERE !position = x %f  y %f", crs_x - (player.x), crs_y-(player.y));  //Prints mouse postion to console - used to identify position for clicks

			crs_x = ev.mouse.x;
			crs_y = ev.mouse.y;
			fired = true;
			if (state == MENU)
			{
				if (crs_x >= 400 && crs_x <=677 && crs_y >= 180 && crs_y <= 256)  ChangeState(state, PLAYING); //Start
				if (crs_x >= 400 && crs_x <= 677 && crs_y >= 310 && crs_y <= 388) ChangeState(state, SETTINGS);	//Settings
				if (crs_x >= 400 && crs_x <= 677 && crs_y >= 438 && crs_y <= 517) done = true; //Quit
				if (crs_x >= 967 && crs_x <= 1023 && crs_y >= 615 && crs_y <= 685) ChangeState(state, HELP);
			}
			if (state == SETTINGS)
			{
				// Position of Lecturers
				if (crs_x >= 100 && crs_x <= 224 && crs_y >= 50 && crs_y <= 180)
				{
					enemsel = lecturers[0]; //Poole
					curLect = 0;
				}
				if (crs_x >= 240 && crs_x <= 354 && crs_y >= 50 && crs_y <= 180)
				{
					enemsel = lecturers[1]; //Saha
				    curLect = 1;
				}
				if (crs_x >= 380 && crs_x <= 504 && crs_y >= 50 && crs_y <= 180)
				{
					enemsel = lecturers[2]; //Tapamo
					curLect = 2;
				}
				if (crs_x >= 520 && crs_x <= 644 && crs_y >= 50 && crs_y <= 180) 
				{
					enemsel = lecturers[3]; //Afullo
					curLect = 3;
				}
				if (crs_x >= 660 && crs_x <= 784 && crs_y >= 50 && crs_y <= 180)
				{
					enemsel = lecturers[4];//Walingo
					curLect = 4; 
				}
				if (crs_x >= 800 && crs_x <= 924 && crs_y >= 50 && crs_y <= 180)
				{
					enemsel = lecturers[5]; //Viranjay
					curLect = 5;
				}
				// End Position of Lecturers

				// Position of Maps
				if (crs_x >= 100 && crs_x <= 224 && crs_y >= 250 && crs_y <= 380)
				{
					bgImage = maps[0]; // howard--always unlocked
					curMap = 0;
				}
				
				if (crs_x >= 240 && crs_x <= 354 && crs_y >= 250 && crs_y <= 380)
				{
					if ((strcmp(al_get_config_value(savegame, "venueunlock 2", "unlocked"), "0") == 0))
					{
						curMap = 6;			//send default state
					
					}
					else {
						bgImage = maps[1]; // tbdavis unlocked
						curMap = 1;
					}
						
				}
				if (crs_x >= 380 && crs_x <= 504 && crs_y >= 250 && crs_y <= 380)
				{
					if (strcmp(al_get_config_value(savegame, "venueunlock 3", "unlocked"), "0") == 0)
					{
						curMap = 6;			//send default state

					}
					else {
						bgImage = maps[2]; // Park unlocked
						curMap = 2;
					}
				}
				if (crs_x >= 520 && crs_x <= 644 && crs_y >= 250 && crs_y <= 380) 
				{
					if (strcmp(al_get_config_value(savegame, "venueunlock 4", "unlocked"), "0") == 0)
					{
						curMap = 6;			//send default state

					}
					else {
						bgImage = maps[3]; // Science unlocked
						curMap = 3;
					}
				}
				if (crs_x >= 660 && crs_x <= 784 && crs_y >= 250 && crs_y <= 380)
				{
					if (strcmp(al_get_config_value(savegame, "venueunlock 5", "unlocked"), "0") == 0)
					{
						curMap = 6;			//send default state

					}
					else {
						bgImage = maps[4]; // Science unlocked
						curMap = 4;
					}
				}
				if (crs_x >= 800 && crs_x <= 924 && crs_y >= 250 && crs_y <= 380)
				{
					if (strcmp(al_get_config_value(savegame, "venueunlock 6", "unlocked"), "0") == 0)
					{
						curMap = 6;			//send default state
					}
					else {
						bgImage = maps[5]; // Science unlocked
						curMap = 5;
					}
				}
				// End Position of Maps

				// Position of Powers

				if (crs_x >= 330 && crs_x <= 430 && crs_y >= 470 && crs_y <= 560)
				{
					atksel = atk[0]; //Calculator
					curAtk = 0;
				}

				if (crs_x >= 500 && crs_x <= 615 && crs_y >= 470 && crs_y <= 560)
				{
					if (strcmp(al_get_config_value(savegame, "weaponunlocks", "unlocked2"), "0") == 0)
					{
						curAtk = 3; //Pencil locked, revert to calc
					}
					else { atksel = atk[1]; curAtk = 1; } //Pencil unlocked
				}

				if (crs_x >= 670 && crs_x <= 800 && crs_y >= 470 && crs_y <= 560)
				{
					if (strcmp(al_get_config_value(savegame, "weaponunlocks", "unlocked3"), "0") == 0)
					{
						curAtk = 3; //C++ locked, revert to calc
					}
					else { atksel = atk[2]; curAtk = 2; } //C++ unlocked
				}
				// End Position of Powers
			}
			FireBullet(bullets, NUM_BULLETS, player);   //Fire Bullets
		}
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		if (ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT)
		{
			if (state == PLAYING)
			{
				if (timeM == true)
				{
					al_stop_timer(timer);										//freezes game timer
					al_draw_bitmap(scrns[1], scrn_W / 2 - 250, 100, 0);			//Show pause menu
					al_flip_display();
				}
			}
		}
		if (ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN)
		{	
			if (state == PLAYING)
			{
				al_stop_timer(timer);									//freezes game timer
				timeM = false;											
				al_draw_bitmap(scrns[1], scrn_W / 2 - 250, 100, 0);		//show pause menu
				al_flip_display();
			}
		}
		
		if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
		{
			al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
		}


		// THIS IS WHERE THE MAGIC HAPPENS. 
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			//Start Animation for all images
			if (++frameCount >= frameDelay) 
			{
				if (++curFrame >= maxFrame)
				{
					curFrame = 0;
				}
				frameCount = 0;
			}
			//End animation

			// TITLE
			if (state == TITLE)
			{
				// We're just chillin in here, waiting for eternity for you to press SPACE... 
				if (keys[SPACE]){
					ChangeState(state, MENU);					//Splash->Menu on Spacebar press
				}

			} //end title 

			// MENU
			else if (state == MENU)
			{
				if (keys[SPACE]){ // pressing space starts the game
					ChangeState(state, PLAYING);				//Menu-> Game if Spacebar press
				}


				al_clear_to_color(black);
				al_draw_bitmap(btns[0], scrn_W / 2 - 130, scrn_H / 2 - 180, 0);			//Start
				al_draw_bitmap(btns[1], scrn_W / 2 - 130, scrn_H / 2 - 50, 0);		//Settings
				al_draw_bitmap(btns[2], scrn_W / 2 - 130, scrn_H / 2 + 80, 0);		//Quit
				al_draw_bitmap(btns[4], 965, 610, 0);								//Help
				boss_sel = lecturers[rand() % 6];
				bosslevel = false;
				player.score = 0;
			} 	//end menu

			// PLAYING (the game! that you just lost ;)
			else if (state == PLAYING)
			{
				if (keys[SPACE]){ // space shoot bullies :D 
					FireBullet(bullets, NUM_BULLETS, player);
					//al_play_sample(sample[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); <--What did this do? 
				}

				int random = rand() % 100;
				StartProjectile(comets, NUM_COMETS);
				UpdateProjectile(comets, NUM_COMETS, 0);

				if (player.score % 100)
				{
					StartProjectile(powerUp, NUM_POWER);
					UpdateProjectile(powerUp, NUM_POWER, 5);
				}
				if (bosslevel == true)
				{
				StartBoss(bossy, NUM_BOSS);
				UpdateBoss(bossy, NUM_BOSS);
				}

				UpdateExplosions(explosions, NUM_EXPLOSIONS);
				UpdateBullet(bullets, NUM_BULLETS, direction);
				CollideBullet(bullets, NUM_BULLETS, comets, NUM_COMETS, player, explosions, NUM_EXPLOSIONS, sample[3]);
				CollideBullet(bullets, NUM_BULLETS, bossy, NUM_BOSS, player, explosions, NUM_EXPLOSIONS, sample[2]);
				CollideProjectile(comets, NUM_COMETS, player,0);
				CollideProjectile(powerUp, NUM_POWER, player, 1);
				CollideBoss(bossy, NUM_BOSS, player);

				if (player.lives <= 0) ChangeState(state, LOST);
			} // end playing


			// LOST... and this does? 
			else if (state == LOST)
			{
				if (keys[SPACE]){ // press space to play again
					ChangeState(state, PLAYING);
				}
			} // end lost

			redraw = true;
			if (keys[UP])    MoveCharacterUp(player);
			if (keys[DOWN])  MoveCharacterDown(player);
			if (keys[LEFT])  MoveCharacterLeft(player);
			if (keys[RIGHT]) MoveCharacterRight(player);
		} 

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) 
		{
			redraw = false;

			if (state == TITLE)
			{//title
				al_draw_bitmap(scrns[0], 0, 0, 0);  //Title Screen
				al_draw_textf(fonts[0],white, scrn_W/2+20, scrn_H-60, ALLEGRO_ALIGN_CENTRE, "PRESS SPACEBAR TO START");
			//end title
			}



			//HELP
			else if (state == HELP)
			{
				if (keys[SPACE]){
					ChangeState(state, MENU);					//Help-> Menu if spacebar press
				}
				al_draw_bitmap(scrns[3], 0, 0, 0);  //Title Screen
				//al_draw_bitmap(btns[3], 800, 610, 0);								    //Back		<<fix this maybe?
				//al_draw_scaled_bitmap(btns[3], 800, 610,150,150,100,100, 10,10, 0);	//Back
				al_draw_textf(fonts[0], white, scrn_W - 300, scrn_H - 50, 0, "PRESS BACKSPACE TO RETURN");
			} // end help
			else if (state == WIN)
			{//win
				al_draw_bitmap(scrns[4], 0, 0, 0);  //Win!
				al_draw_textf(fonts[0], black, scrn_W / 2 - 110, scrn_H - 30, ALLEGRO_ALIGN_CENTRE, "ENTER FOR NEW GAME");
				al_draw_textf(fonts[0], white, 110, 0, ALLEGRO_ALIGN_CENTRE, "ENTER FOR NEW GAME");
				al_draw_textf(fonts[0], black, scrn_W / 2 + 130, scrn_H - 30, ALLEGRO_ALIGN_CENTRE, "BKSPACE FOR MENU");
				al_draw_textf(fonts[0], white, scrn_W - 100, 0, ALLEGRO_ALIGN_CENTRE, "BKSPACE FOR MENU");
				boss_sel = lecturers[rand() % 6];
				player.score = 0;
				level = 0;
			//end win
			}
			else if (state == SETTINGS)
			{
				//settings
				al_clear_to_color(black);
				al_draw_textf(fonts[1], white,scrn_W/2-100, 20, 0, "CHOOSE YOUR LECTURER : ");

				al_draw_bitmap(minilect[0], 100, 50, 0);	//Poole
				al_draw_bitmap(minilect[1], 240, 50, 0);	//Saha
				al_draw_bitmap(minilect[2], 380, 50, 0);	//Tapamo
				al_draw_bitmap(minilect[3], 520, 50, 0);	//Afullo
				al_draw_bitmap(minilect[4], 660, 50, 0);	//Walingo
				al_draw_bitmap(minilect[5], 800, 50, 0);	//Viranjay

				switch (curLect)
				{
				case 0:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 180, 0, "CURRENT LECTURER : Dr John Poole");
					break;
				case 1:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 180, 0, "CURRENT LECTURER : Dr Akshay Kumar");
					break;
				case 2:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 180, 0, "CURRENT LECTURER : Professor Jules Tapamo");
					break;
				case 3:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 180, 0, "CURRENT LECTURER : Professor Thomas Afullo");
					break;
				case 4:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 180, 0, "CURRENT LECTURER : Dr Tom Walingo");
					break;
				case 5:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 180, 0, "CURRENT LECTURER : Dr Viranjay Srivastava");
					break;
				}
				

				al_draw_textf(fonts[1], white, scrn_W / 2 - 80, 225, 0, "CHOOSE YOUR VENUE : ");
				//draw bitmap based on venue locked/unlocked
				al_draw_bitmap(mapsmini[0], 100, 250, 0);	//Howard
				
				if (strcmp(al_get_config_value(savegame, "venueunlock 2", "unlocked"), "0") == 0)
				{		al_draw_bitmap(lockedmap[1], 240, 250, 0);	//TBDavis
				}		else{	al_draw_bitmap(mapsmini[1], 240, 250, 0);}
				
				if (strcmp(al_get_config_value(savegame, "venueunlock 3", "unlocked"), "0") == 0)
				{		al_draw_bitmap(lockedmap[2], 380, 250, 0);
				}		else{ al_draw_bitmap(mapsmini[2], 380, 250, 0);}//Park

				if (strcmp(al_get_config_value(savegame, "venueunlock 4", "unlocked"), "0") == 0)
				{		al_draw_bitmap(lockedmap[3], 520, 250, 0);
				}		else{ al_draw_bitmap(mapsmini[3], 520, 250, 0); }//Science

				if (strcmp(al_get_config_value(savegame, "venueunlock 5", "unlocked"), "0") == 0)
				{		al_draw_bitmap(lockedmap[4], 660, 250, 0);
				}		else{ al_draw_bitmap(mapsmini[4], 660, 250, 0); }//Cafe

				if (strcmp(al_get_config_value(savegame, "venueunlock 6", "unlocked"), "0") == 0)
				{	al_draw_bitmap(lockedmap[5], 800, 250, 0);
				}		else{ al_draw_bitmap(mapsmini[5], 800, 250, 0); }//Cafe


				switch (curMap)
				{
					//need savegame unlockables
				case 0:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150,380, 0, "CURRENT VENUE : Howard Building");
					break;
				case 1:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 380, 0, "CURRENT VENUE : T.B Davis");
					break;					
				case 2:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 380, 0, "CURRENT VENUE : The Park");
					break;
				case 3:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150,380, 0, "CURRENT VENUE : Science Block");
					break;
				case 4:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 380, 0, "CURRENT VENUE : The Cafe");
					break;
				case 5:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 380, 0, "CURRENT VENUE : The Amphitheatre");
					break;
				case 6:
					al_draw_textf(fonts[1], red, scrn_W / 2 - 190, 380, 0, "This is a locked venue. Keep playing to unlock it.");
					//curMap = 0;
					bgImage = maps[0]; // Default to Howard
					break;
				}

				al_draw_textf(fonts[1], white, scrn_W / 2 - 80, 425, 0, "CHOOSE YOUR POWER : ");

				al_draw_bitmap(atk[0], scrn_W / 2 - 200, 475, 0);	//Calculator
				
				if (strcmp(al_get_config_value(savegame, "weaponunlocks", "unlocked2"), "0") == 0)
				{
					al_draw_bitmap(lockatk[1], scrn_W / 2 - 20, 475, 0);	//Pencil locked
				}
				else{ al_draw_bitmap(atk[1], scrn_W / 2 - 20, 475, 0); };	//Pencil

				if (strcmp(al_get_config_value(savegame, "weaponunlocks", "unlocked3"), "0") == 0)
				{
					al_draw_bitmap(lockatk[2], scrn_W / 2 + 160, 475, 0);	//C++ locked
				}
				else{al_draw_bitmap(atk[2], scrn_W / 2 + 160, 475, 0);};	//C++
				

				switch (curAtk)
				{
				case 0:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 575, 0, "CURRENT POWER : Calculator");
					break;
				case 1:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 575, 0, "CURRENT POWER : Pencil");
					break;
				case 2:
					al_draw_textf(fonts[1], white, scrn_W / 2 - 150, 575, 0, "CURRENT POWER : C++ Programming");
					break;
				case 3:
					al_draw_textf(fonts[1], red, scrn_W / 2 - 150, 575, 0, "This weapon is locked. Keep playing to unlock!");
					atksel = atk[0]; //revert to calc
				}

				al_draw_textf(fonts[0], white, scrn_W - 300, scrn_H-50, 0, "PRESS BACKSPACE TO RETURN");

				if (egg >= 2)
				{
					player_img[0] = al_load_bitmap("./images/pikaR.png");
					player_img[1] = al_load_bitmap("./images/pikaL.png");
					player_img[2] = al_load_bitmap("./images/pikaWalkR.png");
					player_img[3] = al_load_bitmap("./images/pikaWalkL.png");

					atksel = atk[3];

				}
				else
				{
					player_img[0] = al_load_bitmap("./images/kriR.png");
					player_img[1] = al_load_bitmap("./images/kriL.png");
					player_img[2] = al_load_bitmap("./images/kriWalkR.png");
					player_img[3] = al_load_bitmap("./images/kriWalkL.png");
				}
				

				//end setting
			}
			else if (state == PLAYING)
			{
				//fprintf(stderr, "\negg = %d", egg);

				//playing
				DrawCharacter(player, select, curFrame, frameW, frameH);
				DrawBullet(bullets, NUM_BULLETS, atksel);
				DrawProjectile(comets, NUM_COMETS, enemsel, curFrame, frameW, frameH);
				DrawProjectile(powerUp, NUM_POWER, power[0], curFrame, frameW, frameH);
				DrawBoss(bossy, NUM_BOSS, boss_sel, curFrame, frameW, frameH);
				DrawExplosions(explosions, NUM_EXPLOSIONS); 

				al_draw_textf(fonts[0], black, scrn_W / 2 - 100, 5, 0, "Score : %d * Level : %d", player.score, level+1);
				if (bosslevel) al_draw_textf(fonts[2], red, scrn_W / 2 -50, 20, 0, "BOSS!");
				if (player.score != 0) // <<<<< CHANGE SCORE FOR BOSS ARRIVAL
				{
					if (bossCheck + 50 == player.score || bossCheck + 51 == player.score || bossCheck + 52 == player.score)
					{
						bosslevel = true;
						if (playone == 1)
						{
							al_play_sample(sample[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							playone++;
						}
					}
				}
				if (win == true)
				{
					level++;
					bossCheck = player.score;
					if (level >= 4)
					{
						ChangeState(state, WIN);
						al_rest(1.5);
						egg = 0;
					}
					boss_sel = lecturers[rand() % 6];											//Default selected enemy/lecturer

					al_draw_textf(fonts[0], white, scrn_W / 2 + 20, scrn_H / 2, ALLEGRO_ALIGN_CENTRE, "PASSED THE YEAR!!");
					
					win = false;
					
				}
				int x = al_get_bitmap_width(numLives[player.lives-1]);
				int y = al_get_bitmap_width(numLives[player.lives-1]);

				al_draw_scaled_bitmap(numLives[player.lives-1], 5, 5,x, y, 5, 5, 150, 150, 0);

				//end playing 
			}
			else if (state == LOST)
			{//lost
				al_draw_bitmap(scrns[2], scrn_W / 2 - 250, 100, 0);                      // Game over Screen
				al_draw_textf(fonts[2], black, scrn_W/2+70, 340, 0, "%i", player.score);
				int ps = player.score;
				char score[10];
				sprintf_s(score, "%i", ps);
				al_set_config_value(savegame,"highscore", "playsc",score);	//saves player score
				al_save_config_file("config.ini", savegame);

				//check for unlocks
				if (ps > 10)
				{
					al_set_config_value(savegame, "venueunlock 2", "unlocked", "1"); //unlock TB Davis
					if (al_get_config_value(savegame, "highscore", "playsc")<score); //only shows if not unlocked before
					{
						al_draw_textf(fonts[0], green, scrn_W / 2 - 100, 380, 0, "NEW UNLOCKS: TB Davis!!");
					}
					
				}
				if (ps > 20)
				{
					al_set_config_value(savegame, "venueunlock 3", "unlocked", "1");//unlock Park
					if (al_get_config_value(savegame, "highscore", "playsc")<score) //only shows if not unlocked before
					{
						al_draw_textf(fonts[0], green, scrn_W / 2 - 100, 400, 0, "NEW UNLOCKS: Park!"); }
				}
				if (ps > 30)
				{
					al_set_config_value(savegame, "venueunlock 4", "unlocked", "1");//unlock Science
					al_set_config_value(savegame, "weaponunlocks", "unlocked2", "1");//unlock Pencil
					if (al_get_config_value(savegame, "highscore", "playsc")<score) //only shows if not unlocked before
					{
						al_draw_textf(fonts[0], green, scrn_W / 2 - 100, 420, 0, "NEW UNLOCKS: Science & Pencil!"); }
				}
				if (ps > 40)
				{
					al_set_config_value(savegame, "venueunlock 5", "unlocked", "1");//Unlock Cafe
					
					if (al_get_config_value(savegame, "highscore", "playsc")<score) //only shows if not unlocked before
					{
						al_draw_textf(fonts[0], green, scrn_W / 2 - 100, 440, 0, "NEW UNLOCKS: Cafe!"); }
				}

				if (ps > 50)
				{
					al_set_config_value(savegame, "venueunlock 6", "unlocked", "1");//unlock Amphitheatre
					al_set_config_value(savegame, "weaponunlocks", "unlocked3", "1");//unlock C++
					
					if (al_get_config_value(savegame, "highscore", "playsc") < score) //only shows if not unlocked before
					{
						al_draw_textf(fonts[0], green, scrn_W / 2 - 100, 460, 0, "NEW UNLOCKS: Amphitheatre & C++"); }
				}
				al_save_config_file("config.ini", savegame);

				egg = 0;

			//end lost
			}
			al_flip_display();
			al_clear_to_color(black);
			al_draw_scaled_bitmap(bgImage, 0, 0, al_get_bitmap_width(bgImage), al_get_bitmap_height(bgImage),0,0,scrn_W,scrn_H, 0);
		}

	}

	//Destruction of assets (prevents assert fails)
	al_destroy_bitmap(player_img[3]);
	al_destroy_bitmap(player_img[2]);
	al_destroy_bitmap(player_img[1]);
	al_destroy_bitmap(player_img[0]);
	al_destroy_bitmap(atksel);

	//this part might be buggy
	for (int i = 1; i < 4; i++)
	{
//		al_destroy_bitmap(atk[i]);
		al_destroy_bitmap(lockatk[i]);
	}
	

	for (int i = 0; i < 5; i++)
	{
		al_destroy_font(fonts[i]);
		al_destroy_bitmap(maps[i]);
		al_destroy_bitmap(mapsmini[i]);
		al_destroy_bitmap(lockedmap[i]);
		al_destroy_bitmap(btns[i]);
		al_destroy_bitmap(scrns[i]);
		al_destroy_sample(sample[i]);

	}
	for (int i = 0; i < 6; i++)
	{
		al_destroy_bitmap(lecturers[i]);
		al_destroy_bitmap(minilect[i]);
	}
	
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_timer(timer);

	al_save_config_file("config.ini", savegame);	//writes default unlocks back if config file removed during gameplay
	
	//al_destroy_bitmap(select);
	//end Destruction
	return 0;
}


void InitCharacter(Character &player)
{
	player.spritex = scrn_W / 2;
	player.spritey = scrn_H / 2;
	player.ID = PLAYER;
	player.lives = 3;
	player.speed = 5;
	player.boundx = 65;
	player.boundy = 160;
}
void DrawCharacter(Character &player, ALLEGRO_BITMAP *select, int cur, int fW, int fH)
{	
	//can we remove the below?
	//al_draw_filled_rectangle(player.spritex + 75, player.spritey + 25, player.spritex + 75 + player.boundx , player.spritey + 25 + player.boundy, green);  << test purposes - check collision area
	//al_draw_bitmap_region(select, cur * fW, 0, fW, fH, player.spritex, player.spritey, 0);
	if (poweredNum < 60 && poweredNum > 0)
	{
		al_draw_scaled_bitmap(al_load_bitmap("./images/power.png"), cur * fW, 0, fW, fH, player.spritex, player.spritey, shrinkx, shrinky, 0);
		poweredNum++;
	}
	al_draw_scaled_bitmap(select, cur * fW, 0, fW, fH, player.spritex, player.spritey, shrinkx, shrinky, 0); //character
}
void MoveCharacterUp(Character &player)
{
	player.spritey -= player.speed;
	if (player.spritey < 0) player.spritey = 0;
	player.dir = 0;
/*
	if (shrinkx >= 200 && shrinky >= 200)
	{
		shrinky -= 5;
		shrinkx -= 5;
		player.boundy -= 2;
	}
*/
}
void MoveCharacterRight(Character &player)
{
	player.spritex += player.speed;
	if (player.spritex > scrn_W - 80)
		player.spritex = scrn_W - 80;
	player.dir = 1;
}
void MoveCharacterDown(Character &player)
{
	player.spritey += player.speed;
	//player.y = player.spritey + 100;
	if (player.spritey > scrn_H-200)
		player.spritey = scrn_H-200;
	player.dir = 2;
/*	if (shrinkx <= 350 && shrinky <= 350)
	{
		shrinky += 5;
		shrinkx += 5;
		player.boundy += 2;
	}
*/
}
void MoveCharacterLeft(Character &player)
{
	player.spritex -= player.speed;
	if (player.spritex < 0)
		player.spritex = 0;
	player.dir = 3;

}

void InitBullet(Character &player, Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		bullet[i].ID = BULLET;
		bullet[i].speed = 10;
		bullet[i].live = false;
		bullet[i].dir = player.dir;
	}
}
void DrawBullet(Bullet bullet[], int size, ALLEGRO_BITMAP *bit)
{
	for (int i = 0; i < size; i++)
	{
		if (bullet[i].live)al_draw_bitmap(bit, bullet[i].x, bullet[i].y,0 );
	}
}
int FindDeadBulletIndex(Bullet bullet[], int size) 
{
	for (int i = 0; i < size; ++i)
	if (!bullet[i].live)
		return i;

	// didn't find an index; maybe delete old bullet or similar?
	return -1;
}
void FireBullet(Bullet bullet[], int size, Character &player)
{

	int index = FindDeadBulletIndex(bullet, size);

	if (index < 0)
		return; // no "open" bullet positions available

	bullet[index].live = true;
	bullet[index].dir = player.dir;

	// set bullet position to character's position ...
	bullet[index].x = player.spritex + 25;
	bullet[index].y = player.spritey + 25;

	// ... or adjust position based on direction, if you want:
	if (player.dir == NORTH) 
	{
		bullet[index].x = player.spritex +50;
		bullet[index].y = player.spritey;
	}
	else if (player.dir == EAST)
	{
		
		bullet[index].x = player.spritex + 20 + player.boundx;
		bullet[index].y = player.spritey + 50;
	}
	else if (player.dir == SOUTH)
	{
		
		bullet[index].x = player.spritex + 50;
		bullet[index].y = player.spritey + 25;
	}
	else if (player.dir == WEST)
	{
		
		bullet[index].x = player.spritex - 20 ;
		bullet[index].y = player.spritey + 50;
	}

}
void UpdateBullet(Bullet bullet[], int size, int dir)
{
	
	for (int i = -1; i < size; i++)

		switch (bullet[i].dir)
	{
		case 0: //up
			//for (int i = 0; i < size; i++)
			{
				if (bullet[i].live)
				{
					bullet[i].y -= bullet[i].speed;
					if (bullet[i].y < 0)
						bullet[i].live = false;
				}
			}
			break;
		case 1: //right
			//for (int i = 0; i < size; i++)
			{
				if (bullet[i].live)
				{
					bullet[i].x += bullet[i].speed;
					if (bullet[i].x > scrn_W)
						bullet[i].live = false;
				}
			}
			break;
		case 2: //down
			//for (int i = 0; i < size; i++)
			{
				if (bullet[i].live)
				{
					bullet[i].y += bullet[i].speed;
					if (bullet[i].y > scrn_H)
						bullet[i].live = false;
				}
			}
			break;
		case 3: //left
			//for (int i = 0; i < size; i++)
			{
				if (bullet[i].live)
				{
					bullet[i].x -= bullet[i].speed;
					if (bullet[i].x < 0)
						bullet[i].live = false;
				}
			}
			break;
	}
	
}

void CollideBullet(Bullet bullet[], int bSize, Projectile thrown[], int cSize, Character &player , Explosion explosions[], int eSize, ALLEGRO_SAMPLE *sample)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int j = 0; j < cSize; j++)
			{
				if (thrown[j].live)
				{
					if (bullet[i].x > (thrown[j].x - thrown[j].boundx) 
					 && bullet[i].x < (thrown[j].x + thrown[j].boundx) 
					 && bullet[i].y > (thrown[j].y - thrown[j].boundy) 
					 && bullet[i].y < (thrown[j].y + thrown[j].boundy))
					{
						bullet[i].live = false;
						thrown[j].live = false;
						al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						//player.score += thrown[j].speed/3;
						for (int k = 0; k < thrown[j].speed / 3;++k)
							player.score++;			
						StartExplosions(explosions, eSize, bullet[i].x + 70, bullet[i].y+30);
					}
				}
			}
		}
	}
}
void CollideBullet(Bullet bullet[], int bSize, Boss bossy[], int cSize, Character &player, Explosion explosions[], int eSize, ALLEGRO_SAMPLE * sample)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int j = 0; j < cSize; j++)
			{
				if (bossy[j].live)
				{
					if (bullet[i].x >(bossy[j].x - bossy[j].boundx+100) 
					 && bullet[i].x < (bossy[j].x + bossy[j].boundx+60) 
					 && bullet[i].y >(bossy[j].y - bossy[j].boundy)
				     && bullet[i].y < (bossy[j].y + bossy[j].boundy+250))
					{
						bullet[i].live = false;
						bossy[j].lives--;
						player.score++;
						if (bossy[j].lives <= 0)
						{
					     bossy[j].live = false;
						 bosslevel = false;
						 win = 1;
						 bossy[j].lives = 30;
						 al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						}
						StartExplosions(explosions, eSize, bullet[i].x + 70, bullet[i].y + 30);
					}
				}
			}
		}
	}
}

void InitProjectile(Projectile thrown[], int size, bool s)
{
	int spd = 3;
	for (int i = 0; i < size; i++)
	{
		if (s) spd = (rand() % 3 + 1) * 3;
		thrown[i].ID = PROJECTILE;
		thrown[i].live = false;
		thrown[i].speed = spd;
		thrown[i].boundx = 110;
		thrown[i].boundy = 120;
		//thrown[i].start_x = thrown[i].x + 25;
		//thrown[i].start_y = thrown[i].y + 10;
	}
}
void DrawProjectile(Projectile thrown[], int size, ALLEGRO_BITMAP *bit, int cur, int fW, int fH)
{
	for (int i = 0; i < size; i++)
	{
		if (thrown[i].live)
		{
			//al_draw_filled_rectangle((thrown[i].start_x), thrown[i].start_y, (thrown[i].x + thrown[i].boundx), thrown[i].y + thrown[i].boundy, green);// << test purposes - check collision area
			al_draw_bitmap_region(bit, cur * fW, 0, fW, fH, thrown[i].x, thrown[i].y, 0);
		}
	}
	
	
}
void StartProjectile(Projectile thrown[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!thrown[i].live)
		{
			if (rand() % 500 == 0)
			{
				thrown[i].live = true;
				thrown[i].x = scrn_W;
				
				retry:
				int y = 30 + rand() % (scrn_H - 20);
				if (y < 475)
				{ 
					thrown[i].y = y;
					thrown[i].start_y = y;
				}
				else
				{
					goto retry;
				}
				//thrown[i].start_x = thrown[i].x + 25;
				//thrown[i].start_y = thrown[i].y + 10;
				break;
			}
		}
	}
}
void UpdateProjectile(Projectile thrown[], int size, int bouncer)
{
	for (int i = 0; i < size; i++)
	{
		if (thrown[i].live)
		{
			thrown[i].x -= thrown[i].speed;
			
			//thrown[i].start_x = thrown[i].x + 25;
			//thrown[i].start_y = thrown[i].y + 10;

			if (bouncer == 5)
			{
				if (thrown[i].y >= (thrown[i].start_y + 100)) sign = 1;
				if (thrown[i].y < 0) thrown[i].y = scrn_H - 30;
				if (thrown[i].y <= (thrown[i].start_y - 100) || thrown[i].y > scrn_H) sign = -1;
				thrown[i].y -= sign*thrown[i].speed;
			}
			
			if (thrown[i].x < 0)
				thrown[i].live = false;
		}
	}
}
void CollideProjectile(Projectile thrown[], int cSize, Character &player, int type)
{

	for (int i = 0; i < cSize; i++)
	{
		if (thrown[i].live)
		{
			if ((thrown[i].x + 20 ) < (player.spritex + 20 + player.boundx) &&			//the + 20 sets it to be on the sprite instead of the outer graphic box
				(thrown[i].x + thrown[i].boundx) > (player.spritex + 75) &&
				(thrown[i].y + 10 ) < (player.spritey + 20 + player.boundy) &&			//the + 10 sets it to be on the sprites instead of the outer graphic box
				(thrown[i].y + thrown[i].boundy) > (player.spritey + 25))
			{
				//al_draw_filled_rectangle((thrown[i].x - thrown[i].boundx),(thrown[i].y - thrown[i].boundy), (thrown[i].x + thrown[i].boundx), thrown[i].y + thrown[i].boundy, green);// << test purposes - check collision area
				if (type == 0)
				{
					player.lives--;
					thrown[i].live = false;
				}
				else if (type == 1)
				{
					if (player.lives < 3)
					{
						player.lives++;
					}
					player.score += 2;
					poweredNum = 1;
					thrown[i].live = false;
				}

			}
			else if (thrown[i].x < 0)
			{
				thrown[i].live = false;
				//player.lives--;
			}
		}
	}
}

void InitBoss(Boss bossy[], int size)
{
	for (int i = 0; i < size; i++)
	{
		int spd = 1;
		bossy[i].ID = BOSS;
		bossy[i].live = false;
		bossy[i].speed = spd;
		bossy[i].boundx = 50;
		bossy[i].boundy = 50;
		bossy[i].lives = 30;
	}
}
void DrawBoss(Boss bossy[], int size, ALLEGRO_BITMAP *bit, int cur, int fW, int fH)
{
	for (int i = 0; i < size; i++)
	{
		if (bossy[i].live)
		{
			//al_draw_filled_rectangle((bossy[i].x+50), bossy[i].y+30, (bossy[i].x + 250 + bossy[i].boundx), (bossy[i].y + 250 +bossy[i].boundy), green);// << test purposes - check collision area
			al_draw_scaled_bitmap(bit, cur * fW, 0, fW, fH, bossy[i].x, bossy[i].y, 325, 325, 0);
			al_draw_filled_rectangle(bossy[i].x + 30, bossy[i].y - 5, bossy[i].x + 300 + 40, bossy[i].y + 15, black); //life bar
			al_draw_filled_rectangle(bossy[i].x + 35, bossy[i].y, bossy[i].x + (bossy[i].lives * 10) + 35, bossy[i].y + 10, red);		//life bar
		}
	}


}
void StartBoss(Boss bossy[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!bossy[i].live)
		{
				bossy[i].live = true;
				bossy[i].x = scrn_W;
				bossy[i].y = scrn_H / 2 -150;
		}
	}
}
void UpdateBoss(Boss bossy[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (bossy[i].live)
		{
			if (bossy[i].x > scrn_W / 2 - 50)
			{
				bossy[i].x -= bossy[i].speed;
			}
			else
			{
				if (bossy[i].y > scrn_H - 286)sign = 1;
				if (bossy[i].y <= 0) sign = -1;
				bossy[i].y -= sign*bossy[i].speed;
			}
		}
	}
}
void CollideBoss(Boss bossy[], int cSize, Character &player)
{
	for (int i = 0; i < cSize; i++)
	{
		if (bossy[i].live)
		{
			if (bossy[i].x - bossy[i].boundx < player.spritex + player.boundx &&
				bossy[i].x + bossy[i].boundx > player.spritex - player.boundx &&
				bossy[i].y - bossy[i].boundy < player.spritey + player.boundy &&
				bossy[i].y + bossy[i].boundy > player.spritey - player.boundy)
			{
				player.lives--;
				//bossy[i].live = false;
			}
			else if (bossy[i].x < 0)
			{
				//bossy[i].live = false;
				player.lives--;
			}
			
		}
	}
}

void InitExplosions(Explosion explosions[], int size, ALLEGRO_BITMAP *image)
{
	for (int i = 0; i < size; i++)
	{
		explosions[i].live = false;

		explosions[i].maxFrame = 4;
		explosions[i].curFrame = 0;
		explosions[i].frameCount = 0;
		explosions[i].frameDelay = 1;
		explosions[i].frameWidth = 128;
		explosions[i].frameHeight = 128;
		explosions[i].animationColumns = 8;
		explosions[i].animationDirection = 1;

		if (image != NULL)
			explosions[i].image = image;
	}
}
void DrawExplosions(Explosion explosions[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (explosions[i].live)
		{
			int fx = (explosions[i].curFrame % explosions[i].animationColumns) * explosions[i].frameWidth;
			int fy = (explosions[i].curFrame / explosions[i].animationColumns) * explosions[i].frameHeight;

			al_draw_bitmap_region(explosions[i].image, fx, fy, explosions[i].frameWidth,
				explosions[i].frameHeight, explosions[i].x - explosions[i].frameWidth / 2, explosions[i].y - explosions[i].frameHeight / 2, 0);
		}
	}
}
void StartExplosions(Explosion explosions[], int size, int x, int y)
{
	for (int i = 0; i < size; i++)
	{
		if (!explosions[i].live)
		{
			explosions[i].live = true;
			explosions[i].x = x;
			explosions[i].y = y;
			break;
		}
	}
}
void UpdateExplosions(Explosion explosions[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (explosions[i].live)
		{
			if (++explosions[i].frameCount >= explosions[i].frameDelay)
			{
				explosions[i].curFrame += explosions[i].animationDirection;
				if (explosions[i].curFrame >= explosions[i].maxFrame)
				{
					explosions[i].curFrame = 0;
					explosions[i].live = false;
				}

				explosions[i].frameCount = 0;
			}
		}
	}
}

void ChangeState(int &state, int newState)
{
	if (state == TITLE)
	{
	}
	else if (state == MENU)
	{
	}
	else if (state == HELP)
	{
	}
	else if (state == WIN)
	{
	}
	else if (state == SETTINGS)
	{
	}
	else if (state == PLAYING)
	{
	}
	else if (state == LOST)
	{
		al_rest(1);
	}

	state = newState;

	if (state == TITLE)
	{
	}
	else if (state == PLAYING)
	{
		InitCharacter(player);
		InitBullet(player, bullets, NUM_BULLETS);
		InitProjectile(comets, NUM_COMETS,1);
		InitProjectile(powerUp, NUM_POWER, 0);
		InitBoss(bossy, NUM_BOSS);
		InitExplosions(explosions, NUM_EXPLOSIONS, al_load_bitmap("./images/boom1.png"));
	}
	else if (state == MENU)
	{
	}
	else if (state == HELP)
	{
	}
	else if (state == SETTINGS)
	{
	}
	else if (state == LOST)
	{
	}
}

