// Header? 


// What do these do? 
#include <math.h>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <lib/objects.h>
#include <cstdio>

using namespace std;

//REMEMBER TO EDIT Linker -> System -> SubSystem -> WINDOW to hide console!

//This program currently just move a Kriyen on a black background. I'm using this to learn and test aspects

const float FPS				= 60;					//Frames per second
const int scrn_W			= 1024;					// Screen Width 
const int scrn_H			= 686;					//Screen Height
const int NUM_BULLETS		= 5;					//Number of bullets  (not actual amount - more like a limit that can be on shot on the screen at any one time)
const int NUM_COMETS		= 10;					//Number of enemies  (same as above)
const int NUM_EXPLOSIONS	= 5;					//Number of explosions (same as above)					
//float shoot_x				= scrn_W / 2.0;			
//float shoot_y				= scrn_H / 2.0;
float crs_x					= scrn_W / 2.0;			//default x location for mouse position detection
float crs_y					= scrn_H / 2.0;			//default y location for mouse position detection

static ALLEGRO_COLOR red,blue,black,white,green;		//Used to create quick access to colours (versus al_map_rgb(0,0,0)
int shrinkx = 200;										//x value used to adjust player size for depth 
int shrinky = 200;										//x value used to adjust player size for depth 
enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE, ENTER };		//Key press shortcut 
enum STATE{ TITLE, PLAYING, LOST, MENU, SETTINGS};		//Game state identification
enum Direction { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};		//Direction of player identification

//asset Init
Character player;	
Bullet bullets[NUM_BULLETS];
Enemy comets[NUM_COMETS];
Explosion explosions[NUM_EXPLOSIONS];
//End asset init

bool keys[6] = { false, false, false, false, false, false }; // enum KEYS init

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
void CollideBullet(Bullet bullet[], int bSize, Enemy comets[], int cSize, Character &ship, Explosion explosions[], int eSize);
void CollideBullet(Bullet bullet[], int bSize, Enemy comets[], int cSize);

void InitEnemy(Enemy comets[], int size);
void DrawEnemy(Enemy comets[], int size);
void DrawEnemy(Enemy comets[], int size, ALLEGRO_BITMAP *bit, int cur, int fW, int fH);
void StartEnemy(Enemy comets[], int size);
void UpdateEnemy(Enemy comets[], int size);
void CollideEnemy(Enemy comets[], int cSize, Character &player);

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


	//animated image var
	int curFrame		= 0;					//Current frame of animated image
	int frameCount		= 0;					//frame counter for animated image
	int frameDelay		= 20;					//rate at which animate image changes
	int frameW			= 128;					//frame width for animated image
	int frameH			= 128;					//frame height for animated image
	const int maxFrame	= 4;					//number of frames in animated image
	//End animated image var

	//Initialisers
	ALLEGRO_DISPLAY			*display		= NULL;					//Screen display
	ALLEGRO_EVENT_QUEUE		*event_queue	= NULL;					//Event Queue
	ALLEGRO_TIMER			*timer			= NULL;					//System timer
	ALLEGRO_FONT			*fonts[5]		= { NULL, NULL, NULL, NULL, NULL };	//fonts array
							//*font36			= NULL,
							//*font20			= NULL,
							//*font18			= NULL,
	ALLEGRO_STATE			*state1 = NULL;					//State
	ALLEGRO_BITMAP			*bgImage		= NULL,					//Title Page splash
							*walkLeft		= NULL,					//Character walking left  	
							*walkRight		= NULL,					//Character walking right
							*standLeft		= NULL,					//Character Standing left
							*standRight		= NULL,					//character standing right
							*select			= NULL,					//Current Selected position of character
							*icon			= NULL,					//Current icon -- NOT SET YET

							*numLives[3] = { NULL, NULL, NULL},		//Attack array
							//*lives			= NULL,					//Number of lives remaining

							*atk[5]			= { NULL, NULL, NULL, NULL, NULL },		//Attack array
							*atksel			= NULL,

							*lecturers[6]	= {NULL, NULL, NULL, NULL, NULL, NULL},  //Lecturer array
							//*poole		= NULL,
							//*taps			= NULL,
							//*saha			= NULL,

							*minilect[6]	= { NULL, NULL, NULL, NULL, NULL, NULL },	//Lecturer Thumbnail

							//*poole1			= NULL,
							//*taps1			= NULL,
							//*saha1			= NULL,

							*enemsel		= NULL,							//Currently selected lecturer
							*exp			= NULL,							//Explosion image

							*maps[6]		= {NULL,NULL,NULL,NULL,NULL},	//Maps array
							//*howard			= NULL,
							//*tbdavis		= NULL,

							*mapsmini[6]	= {NULL,NULL,NULL,NULL,NULL},	//Map Thumbnails
							//*howards		= NULL,
							//*tbdaviss		= NULL,

							*mapsel			= NULL,				//Currently selected maps

							*scrns[5] = { NULL, NULL, NULL, NULL, NULL },		//Box images array (pause, gameover, etc)
							//*title			= NULL,
							//*pause			= NULL,

							*btns[5] = { NULL, NULL, NULL, NULL, NULL };		//Buttons Array
							//*start			= NULL,
							//*stop				= NULL,
							//*setting			= NULL,

							

	if (!al_init())											//initialize and check Allegro
	{
		al_show_native_message_box(display, "Error!", "Warning!", "Failed to initialise Allegro! \n Closing Application!", NULL, ALLEGRO_MESSAGEBOX_WARN);
		//fprintf(stderr, "failed to initialize allegro!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}


	display = al_create_display(scrn_W, scrn_H);			//create our display object
	if (!display)											//Check display
	{
		al_show_native_message_box(display, "Error!", "Warning!", "Failed to initialise display! \n Closing Application!", NULL, ALLEGRO_MESSAGEBOX_WARN);
		//fprintf(stderr, "failed to create display!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}


	timer = al_create_timer(1.0 / FPS);							//Create Timer
	if (!timer)													//Check timer creation
	{				
		al_show_native_message_box(display, "Error!", "Warning!", "Failed to initialise timer! \n Closing Application!", NULL, ALLEGRO_MESSAGEBOX_WARN);
		//fprintf(stderr, "failed to create timer!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	event_queue = al_create_event_queue();						//Create event queue
	if (!event_queue)											//Check event queue creation
	{											
		al_show_native_message_box(display, "Error!", "Warning!", "Failed to initialise event queue! \n Closing Application!", NULL, ALLEGRO_MESSAGEBOX_WARN);
		//fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}


	//Init all Addons
	al_init_primitives_addon();								//load primitive (drawing shapes, etc)
	al_init_font_addon();									//load font addon
	al_install_audio();										// load sound addon
	al_init_ttf_addon();									//load truetype font addon	
	al_init_image_addon();									//load image processing addon
	al_install_keyboard();									//install keyboard
	al_install_mouse();										//install mouse
	//end addon innit
	
	//cursor = al_load_bitmap("./images/target.png");
	//custom_cursor = al_create_mouse_cursor(cursor, 0, 0);
	fonts[2] = al_load_ttf_font("arial.ttf", 36, 0);				//
	fonts[1] = al_load_ttf_font("arial.ttf", 20, 0);				//Load custom font
	fonts[0] = al_load_ttf_font("arial.ttf", 18, 0);				//

	//Init images

	//Character Images
	walkRight	= al_load_bitmap("./images/kriWalkR.png");
	walkLeft	= al_load_bitmap("./images/kriWalkL.png");
	standLeft	= al_load_bitmap("./images/kriL.png");
	standRight	= al_load_bitmap("./images/kriR.png");
	select		= standRight;
	
	//Attack images
	atk[0]	= al_load_bitmap("./images/calc.png");
	atk[1]	= al_load_bitmap("./images/pencil.png");
	atk[2]	= al_load_bitmap("./images/c.png");
	atksel = atk[0];

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

	enemsel = lecturers[0];											//Default selected enemy/lecturer
	
	//Lecturer Thumbnails
	minilect[0]			= al_load_bitmap("./images/poole1.png");	
	minilect[1]			= al_load_bitmap("./images/saha1.png");
	minilect[2]			= al_load_bitmap("./images/taps1.png");
	minilect[3]			= al_load_bitmap("./images/afullo1.png");
	minilect[4]			= al_load_bitmap("./images/tom1.png");
	minilect[5]			= al_load_bitmap("./images/viran1.png");
	
	//Map Images
	maps[0]		= al_load_bitmap("./images/howard.png");		//Howard Building
	maps[1]		= al_load_bitmap("./images/tbdavis.png");		//TB Davis
	maps[2]		= al_load_bitmap("./images/park.png");			//The park
	maps[3]		= al_load_bitmap("./images/science.png");	//Science
	maps[4]		= al_load_bitmap("./images/cafe.png");		//cafe
	maps[5]		= al_load_bitmap("./images/amphi.png");	//amphitheatre

	bgImage		= maps[0];								//Default selected map

	//Map thumbnail
	mapsmini[0]	= al_load_bitmap("./images/howards.png");		//Howard Building
	mapsmini[1]	= al_load_bitmap("./images/tbdaviss.png");		//TB Davis 
	mapsmini[2] = al_load_bitmap("./images/parks.png");			//The park
	mapsmini[3] = al_load_bitmap("./images/sciences.png");	//science
	mapsmini[4] = al_load_bitmap("./images/cafes.png");			//cafe
	mapsmini[5] = al_load_bitmap("./images/amphis.png");

	//Button Images
	btns[0]		= al_load_bitmap("./images/startbtn.png");		//start
	btns[1]		= al_load_bitmap("./images/sttngbtn.png");		//settings
	btns[2]		= al_load_bitmap("./images/stpbtn.png");		//stop
	btns[3]		= al_load_bitmap("./images/back.png");			//back

	//Menu Images
	scrns[0]		= al_load_bitmap("./images/BG1.png");		//Title Background
	scrns[1]		= al_load_bitmap("./images/pause.png");
	scrns[2]		= al_load_bitmap("./images/gameover.png");
	
	
	int direction = 1;						//Default direction identifier init

   // al_convert_mask_to_alpha(walkLeft, al_map_rgb(106, 76, 48));  //clear designated colour to create clear image

	//end animated character

	//Event queue - register listeners
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
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
	/*InitCharacter(player);
	InitBullet(player, bullets, NUM_BULLETS);
	InitEnemy(comets, NUM_COMETS);
	InitExplosions(explosions, NUM_EXPLOSIONS, exp);*/
	ChangeState(state, TITLE);
	srand(time(NULL));
	//end Asset variables

	//End initialisers
	//al_set_display_icon(display, icon);  
	al_set_window_title(display, "UKZN - LECTURE DEFENCE - HOWARD EDITION");   //set window title 
	al_start_timer(timer);											//Start event timer (program clock)
	al_clear_to_color(black);										//Clear and set Background black
	al_set_target_bitmap(al_get_backbuffer(display));				//
	al_flip_display();
	
	
	while (!done)
	{
		ALLEGRO_EVENT ev;										//Allegro event init
		al_wait_for_event(event_queue, &ev);					//wait for and accept events 

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)					//If Key down event
		{
			fired = false;										//Bullet Fired False 
			switch (ev.keyboard.keycode)						//Switch keyboard code returned
			{
			case ALLEGRO_KEY_UP:				
				keys[UP] = true;
				if (direction == 1) select = walkRight;			//set character sprite to ____
				else select = walkLeft;
				break;
			case ALLEGRO_KEY_W:									
				keys[UP] = true;
				if (direction == 1) select = walkRight;
				else select = walkLeft;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				if (direction == 1) select = walkRight;
				else select = walkLeft;
				break;
			case ALLEGRO_KEY_S:
				keys[DOWN] = true;
				if (direction == 1) select = walkRight;
				else select = walkLeft;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				select = walkRight;
				direction = 1;
				break;
			case ALLEGRO_KEY_D:
				keys[RIGHT] = true;
				select = walkRight;
				direction = 1;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				select = walkLeft;
				direction = 0;
				break;
			case ALLEGRO_KEY_A:
				keys[LEFT] = true;
				select = walkLeft;
				direction = 0;
				break;
			case ALLEGRO_KEY_ENTER:
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				if (state == TITLE)
					ChangeState(state, MENU);
				else if (state == MENU)
					ChangeState(state, PLAYING);
				else if (state == PLAYING)
					FireBullet(bullets, NUM_BULLETS, player);
				else if (state == LOST)
					ChangeState(state, PLAYING);
				break;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP)					//If Key down event
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				if (direction == 1) select = standRight;
				else select = standLeft;
				break;
			case ALLEGRO_KEY_W:
				keys[UP] = false;
				if (direction == 1) select = standRight;
				else select = standLeft;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				if (direction == 1) select = standRight;
				else select = standLeft;
				break;
			case ALLEGRO_KEY_S:
				keys[DOWN] = false;
				if (direction == 1) select = standRight;
				else select = standLeft;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				select = standRight;
				break;
			case ALLEGRO_KEY_D:
				keys[RIGHT] = false;
				select = standRight;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				select = standLeft;
				break;
			case ALLEGRO_KEY_A:
				select = standLeft;
				break;
			case ALLEGRO_KEY_P:
				if (state == PLAYING)					//Check state		-- THIS FUNCTION NEEDS REPAIRS	
				{
					if (timeM == true)
					{
						al_stop_timer(timer);									//Stop timer for pause menu
						timeM = false;
						al_draw_bitmap(scrns[1], scrn_W / 2 - 250, 100, 0);		//Show Pause menu
						al_flip_display();										//Bring backbuffer forward (bring all set contents to the current frame)

					}
					else
					{
						al_start_timer(timer);									//Continue timer
						timeM = true;
					}
				}
				break;
			case ALLEGRO_KEY_BACKSPACE:
				if (timeM == false)
				{
					timeM = true;
					al_start_timer(timer);
				}
				ChangeState(state, MENU);
				break;
			case ALLEGRO_KEY_ESCAPE:
				if (state==TITLE || state == MENU)
					done = true;
				else 
					ChangeState(state, MENU);
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}
		
	
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)  
		{
			crs_x = ev.mouse.x;									//get x co-ord of mouse
			crs_y = ev.mouse.y;									//get y co-ord of mouse
			//(stderr, "\nposition = x %f  y %f", crs_x,crs_y);


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
			//fprintf(stderr, "\nHERE !position = x %f  y %f", crs_x, crs_y);  //Prints mouse postion to console - used to identify position for clicks
			crs_x = ev.mouse.x;
			crs_y = ev.mouse.y;
			fired = true;
			if (state == MENU)
			{
				if (crs_x >= 400 && crs_x <=677 && crs_y >= 180 && crs_y <= 256)  ChangeState(state, PLAYING); //Start
				if (crs_x >= 400 && crs_x <= 677 && crs_y >= 310 && crs_y <= 388) ChangeState(state, SETTINGS);	//Settings
				if (crs_x >= 400 && crs_x <= 677 && crs_y >= 438 && crs_y <= 517) done = true; //Quit
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
					bgImage = maps[0]; // howard
					curMap = 0;
				}
				if (crs_x >= 240 && crs_x <= 354 && crs_y >= 250 && crs_y <= 380)
				{
					bgImage = maps[1]; // tbdavis
					curMap = 1;
				}
				if (crs_x >= 380 && crs_x <= 504 && crs_y >= 250 && crs_y <= 380)
				{
					bgImage = maps[2];		//Park
					curMap = 2;
				}
				if (crs_x >= 520 && crs_x <= 644 && crs_y >= 250 && crs_y <= 380) 
				{
					bgImage = maps[3];		//science
					curMap = 3;
				}
				if (crs_x >= 660 && crs_x <= 784 && crs_y >= 250 && crs_y <= 380)
				{
					bgImage = maps[4];		//cafe
					curMap = 4;
				}
				if (crs_x >= 800 && crs_x <= 924 && crs_y >= 250 && crs_y <= 380)
				{
					bgImage = maps[5];		//amphi
					curMap = 5;
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
					atksel = atk[1]; //Pencil
					curAtk = 1;

				}
				if (crs_x >= 670 && crs_x <= 800 && crs_y >= 470 && crs_y <= 560)
				{
					atksel = atk[2]; //C++
					curAtk = 2;
				}
				// End Position of Powers
			}
			FireBullet(bullets, NUM_BULLETS, player);   //Fire Bullets
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) 
		{		
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
					al_stop_timer(timer);
					al_draw_bitmap(scrns[1], scrn_W / 2 - 250, 100, 0);			//Show pause menu
					al_flip_display();
				}
			}
		}
		if (ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN)
		{	
			if (state == PLAYING)
			{
				al_stop_timer(timer);
				timeM = false;
				al_draw_bitmap(scrns[1], scrn_W / 2 - 250, 100, 0);
				al_flip_display();
			}
		}
		
		if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
		{
			al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
		}
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

			if (state == TITLE)
			{
			}
			else if (state == MENU)
			{
			}
			else if (state == SETTINGS)
			{
			}
			else if (state == PLAYING)
			{
				StartEnemy(comets, NUM_COMETS);
				UpdateEnemy(comets, NUM_COMETS);
				UpdateExplosions(explosions, NUM_EXPLOSIONS);
				UpdateBullet(bullets, NUM_BULLETS, direction);
				CollideBullet(bullets, NUM_BULLETS, comets, NUM_COMETS, player, explosions, NUM_EXPLOSIONS);
				CollideEnemy(comets, NUM_COMETS, player);

				if (player.lives <= 0) ChangeState(state, LOST);
			}
			else if (state == LOST)
			{
			}

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

			/*DrawCharacter(player,select,curFrame,frameW, frameH);
			DrawBullet(bullets, NUM_BULLETS, light);			
			DrawEnemy(comets, NUM_COMETS, saha, curFrame, frameW, frameH);
			DrawExplosions(explosions, NUM_EXPLOSIONS);*/

			/*
			//al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, green);
			//al_draw_scaled_bitmap(select, curFrame*frameW, 0, 128, 128, pos_x, pos_y, 350, 350, 0);    //makes shit big
			//al_draw_bitmap_region(select, curFrame * frameW, 0, frameW, frameH,pos_x,pos_y,0);
			//wal_draw_bitmap(select, pos_x, pos_y, 0);
			pos_y -= keys[UP] * 10;
			pos_y += keys[DOWN] * 10;
			pos_x -= keys[LEFT] * 10;
			pos_x += keys[RIGHT] * 10;
			*/

			if (state == TITLE)
			{
				if (scrns[0] == NULL)
					al_draw_filled_circle(0, 0, 20, green);
				else
					al_draw_bitmap(scrns[0], 0, 0, 0);  //Title Screen

				//al_draw_bitmap(scrns[0], 0, 0, 0);  //Title Screen
				al_draw_textf(fonts[0],white, scrn_W/2+20, scrn_H-60, ALLEGRO_ALIGN_CENTRE, "PRESS SPACEBAR TO START");
			}
			else if (state == MENU)
			{
				al_clear_to_color(black);
				al_draw_bitmap(btns[0], scrn_W/2 - 130, scrn_H/2 - 180 ,0);			//Start
				al_draw_bitmap(btns[1], scrn_W / 2 - 130, scrn_H / 2 -50 , 0);		//Settings
				al_draw_bitmap(btns[2], scrn_W / 2 - 130, scrn_H / 2 + 80, 0);		//Quit
			}
			else if (state == SETTINGS)
			{
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

				al_draw_bitmap(mapsmini[0], 100, 250, 0);	//Howard
				al_draw_bitmap(mapsmini[1], 240, 250, 0);	//TBDavis
				al_draw_bitmap(mapsmini[2], 380, 250, 0);	//Park
				al_draw_bitmap(mapsmini[3], 520, 250, 0);	//Science
				al_draw_bitmap(mapsmini[4], 660, 250, 0);	//Cafe
				al_draw_bitmap(mapsmini[5], 800, 250, 0);	//Amphi

				switch (curMap)
				{
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
				}

				al_draw_textf(fonts[1], white, scrn_W / 2 - 80, 425, 0, "CHOOSE YOUR POWER : ");

				al_draw_bitmap(atk[0], scrn_W / 2 - 200, 475, 0);	//Calculator
				al_draw_bitmap(atk[1], scrn_W / 2 - 20, 475, 0);	//Pencil
				al_draw_bitmap(atk[2], scrn_W / 2 + 160, 475, 0);	//C++

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
				}

				al_draw_textf(fonts[0], white, scrn_W - 300, scrn_H-50, 0, "PRESS BACKSPACE TO RETURN");
			}
			else if (state == PLAYING)
			{

				DrawCharacter(player, select, curFrame, frameW, frameH);
				DrawBullet(bullets, NUM_BULLETS, atksel);
				DrawEnemy(comets, NUM_COMETS, enemsel, curFrame, frameW, frameH);
				DrawExplosions(explosions, NUM_EXPLOSIONS);

				al_draw_textf(fonts[0], black, scrn_W/2-100, 5, 0, "Score : %i ", player.score*10);
				int x = al_get_bitmap_width(numLives[player.lives-1]);
				int y = al_get_bitmap_width(numLives[player.lives-1]);

				al_draw_scaled_bitmap(numLives[player.lives-1], 5, 5,x, y, 5, 5, 150, 150, 0);
				

			}
			else if (state == LOST)
			{
				al_draw_bitmap(scrns[2], scrn_W / 2 - 250, 100, 0);                      // Game over Screen
				al_draw_textf(fonts[2], black, scrn_W/2+70, 340, 0, "%i", player.score*10);
			}


			al_flip_display();
			al_clear_to_color(black);
			al_draw_scaled_bitmap(bgImage, 0, 0, al_get_bitmap_width(bgImage), al_get_bitmap_height(bgImage),0,0,scrn_W,scrn_H, 0);

		}

	}

	//Destruction
	al_destroy_bitmap(walkLeft);
	al_destroy_bitmap(walkRight);
	al_destroy_bitmap(standLeft);
	al_destroy_bitmap(standRight);
	al_destroy_bitmap(atksel);

	for (int i = 0; i < 5; i++)
	{
		al_destroy_font(fonts[i]);
		al_destroy_bitmap(maps[i]);
		al_destroy_bitmap(mapsmini[i]);
		al_destroy_bitmap(btns[i]);
		al_destroy_bitmap(scrns[i]);
		//al_destroy_bitmap(atk[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		al_destroy_bitmap(lecturers[i]);
		al_destroy_bitmap(minilect[i]);
	}
	
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_timer(timer);
	//al_destroy_bitmap(select);
	//end Destruction
	return 0;
}



void InitCharacter(Character &player)
{
	player.x = scrn_W / 2;
	player.y = scrn_H / 2;
	player.ID = PLAYER;
	player.lives = 3;
	player.speed = 7;
	player.boundx = 5;
	player.boundy = 7;
	player.score = 0;
}
void DrawCharacter(Character &player, ALLEGRO_BITMAP *select, int cur, int fW, int fH)
{
	al_draw_scaled_bitmap(select, cur * fW, 0, fW, fH, player.x, player.y, shrinkx, shrinky, 0);
	//al_draw_bitmap_region(select, cur * fW, 0, fW, fH, player.x, player.y, 0);
}

void MoveCharacterUp(Character &player)
{
	player.y -= player.speed;
	if (player.y < 0) player.y = 0;
	player.dir = 0;
	if (shrinkx >= 200 && shrinky >= 200)
	{
		shrinky -= 5;
		shrinkx -= 5;
	}
}
void MoveCharacterRight(Character &player)
{
	player.x += player.speed;
	if (player.x > scrn_W - 80)
		player.x = scrn_W - 80;
	player.dir = 1;
}
void MoveCharacterDown(Character &player)
{
	player.y += player.speed;
	if (player.y > scrn_H-200)
		player.y = scrn_H-200;
	player.dir = 2;
	if (shrinkx <= 350 && shrinky <= 350)
	{
		shrinky += 5;
		shrinkx += 5;
	}
}
void MoveCharacterLeft(Character &player)
{
	player.x -= player.speed;
	if (player.x < 0)
		player.x = 0;
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
		if (bullet[i].live)

			//al_draw_filled_circle(bullet[i].x, bullet[i].y, 2, al_map_rgb(255, 255, 255));
			//al_draw_scaled_bitmap(select, curFrame*frameW, 0, 128, 128, pos_x, pos_y, 350, 350, 0);    //makes shit big
			al_draw_bitmap(bit, bullet[i].x, bullet[i].y,0 );

			//al_draw_scaled_bitmap(bit, bullet[i].x, bullet[i].y, 117, 117, (float)bullet[i].x, (float)bullet[i].y, 100.0, 100.0, 0);
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
	bullet[index].x = player.x;
	bullet[index].y = player.y+30;

	// ... or adjust position based on direction, if you want:
	if (player.dir == NORTH) 
	{
		
		bullet[index].x = player.x;
		bullet[index].y = player.y + 17;
	}
	else if (player.dir == EAST)
	{
		
		bullet[index].x = player.x + 17;
		bullet[index].y = player.y;
	}
	else if (player.dir == SOUTH)
	{
		
		bullet[index].x = player.x;
		bullet[index].y = player.y - 17;
	}
	else if (player.dir == WEST)
	{
		
		bullet[index].x = player.x -17;
		bullet[index].y = player.y;
	}

	//fprintf(stderr, "\nBullet [%d] direction :  %d", index, bullet[index].dir);

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
void CollideBullet(Bullet bullet[], int bSize, Enemy comets[], int cSize, Character &player , Explosion explosions[], int eSize)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int j = 0; j < cSize; j++)
			{
				if (comets[j].live)
				{
					if (bullet[i].x >(comets[j].x - comets[j].boundx) 
						&& bullet[i].x < (comets[j].x + comets[j].boundx) 
						&& bullet[i].y > (comets[j].y - comets[j].boundy) 
						&& bullet[i].y < (comets[j].y + comets[j].boundy))
					{
						bullet[i].live = false;
						comets[j].live = false;
						player.score++;
						
						StartExplosions(explosions, eSize, bullet[i].x + 70, bullet[i].y+30);
					}
				}
			}
		}
	}
}

void InitEnemy(Enemy comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		comets[i].ID = ENEMY;
		comets[i].live = false;
		comets[i].speed = 5;
		comets[i].boundx = 50;
		comets[i].boundy = 100;
		//comets[i].image = al_load_bitmap("./images/boom.png");
	}
}
void DrawEnemy(Enemy comets[], int size, ALLEGRO_BITMAP *bit, int cur, int fW, int fH)
{
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live)
		{
			//al_draw_bitmap(bit, comets[i].x, comets[i].y, 0);
			//comets[i].image = bit;
			al_draw_bitmap_region(bit, cur * fW, 0, fW, fH, comets[i].x, comets[i].y, 0);

			//al_draw_filled_circle(comets[i].x, comets[i].y, 20, al_map_rgb(255, 0, 0));
		}
	}
	
	
}
void StartEnemy(Enemy comets[], int size)
{
	
	for (int i = 0; i < size; i++)
	{
		if (!comets[i].live)
		{
			if (rand() % 500 == 0)
			{
				comets[i].live = true;
				comets[i].x = scrn_W;
				
				retry:
				int y = 30 + rand() % (scrn_H - 20);
				if (y < 475)
				{ 
					comets[i].y = y;
				}
				else
				{
					goto retry;
				}

				break;
			}
		}
	}
}
void UpdateEnemy(Enemy comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live)
		{
			comets[i].x -= comets[i].speed;

			if (comets[i].x < 0)
				comets[i].live = false;
		}
	}
}
void CollideEnemy(Enemy comets[], int cSize, Character &player)
{
	for (int i = 0; i < cSize; i++)
	{
		if (comets[i].live)
		{
			if (comets[i].x - comets[i].boundx < player.x + player.boundx &&
				comets[i].x + comets[i].boundx > player.x - player.boundx &&
				comets[i].y - comets[i].boundy < player.y + player.boundy &&
				comets[i].y + comets[i].boundy > player.y - player.boundy)
			{
				player.lives--;
				comets[i].live = false;
			}
			else if (comets[i].x < 0)
			{
				comets[i].live = false;
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
	else if (state == SETTINGS)
	{
	}
	else if (state == PLAYING)
	{
	}
	else if (state == LOST)
	{
	}

	state = newState;

	if (state == TITLE)
	{
	}
	else if (state == PLAYING)
	{
		InitCharacter(player);
		InitBullet(player, bullets, NUM_BULLETS);
		InitEnemy(comets, NUM_COMETS);
		InitExplosions(explosions, NUM_EXPLOSIONS, al_load_bitmap("./images/boom1.png"));
	}
	else if (state == MENU)
	{
	}
	else if (state == SETTINGS)
	{
	}
	else if (state == LOST)
	{
	}
}
