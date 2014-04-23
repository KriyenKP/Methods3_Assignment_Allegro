//#include <Box2D/Box2D.h>
#include <math.h>
#include <objects.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
//#include <exception>
#include <stdio.h>
#include <cstdio>


//REMEMBER TO EDIT Linker -> System -> SubSystem -> WINDOW to hide console!

//This program currently just move a Kriyen on a black background. I'm using this to learn and test aspects

const float FPS			= 60;
const int BOUNCER_SIZE	= 32;
const int scrn_W		= 1024;
const int scrn_H		= 700;
int bulletCount			= 5;
float shoot_x			= scrn_W / 2.0;
float shoot_y			= scrn_H / 2.0;
float crs_x				= scrn_W / 2.0;
float crs_y				= scrn_H / 2.0;
int enem				= rand() % 3 + 1;

static ALLEGRO_COLOR red,blue,black,white,green;
int shrinkx = 200;
int shrinky = 200;
enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE, ENTER };
enum Direction {
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3
};
bool keys[6] = { false, false, false, false, false, false };

//prototypes
void InitCharacter(Character &player);
void DrawCharacter(Character &player, ALLEGRO_BITMAP *select, int cur, int fW, int fH);

void MoveCharacterLeft(Character &player);
void MoveCharacterUp(Character &player);
void MoveCharacterDown(Character &player);
void MoveCharacterRight(Character &player);

const int NUM_BULLETS = 5;
void InitBullet(Character &player, Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size, ALLEGRO_BITMAP *bit);
void FireBullet(Bullet bullet[], int size, Character &player);
void UpdateBullet(Character &player, Bullet bullet[], int size, int dir);
void UpdateBullet(Bullet bullet[], int size, int dir);
void CollideBullet(Bullet bullet[], int bSize, Enemy comets[], int cSize);

const int NUM_COMETS = 10;
void InitEnemy(Enemy comets[], int size);
void DrawEnemy(Enemy comets[], int size);
void DrawEnemy(Enemy comets[], int size, ALLEGRO_BITMAP *bit, int cur, int fW, int fH);
void StartEnemy(Enemy comets[], int size);
void UpdateEnemy(Enemy comets[], int size);
void CollideEnemy(Enemy comets[], int cSize, Character &player);


int main(void)
{
	//int pos_x = scrn_W / 2;
	//int pos_y = scrn_H / 2;
	float bouncer_x		= scrn_W / 2.0 - BOUNCER_SIZE / 2.0;
	float bouncer_y		= scrn_H / 2.0 - BOUNCER_SIZE / 2.0;
	float bouncer_dx	= -4.0, 
		  bouncer_dy	= 4.0;

	bool done	= false, 
		 fired	= false, 
		 redraw = true,
	     timeM	= true;

	//Asset variables
	
	Character player;InitCharacter(player);
	Bullet bullets[5];InitBullet(player,bullets, NUM_BULLETS);
	Enemy comets[NUM_COMETS];InitEnemy(comets, NUM_COMETS);
	srand(time(NULL));

	//end Asset variables

	//animated image var
	int curFrame		= 0;					//Current frame of animated image
	int frameCount		= 0;					//frame counter for animated image
	int frameDelay		= 20;					//rate at which animate image changes
	int frameW			= 128;					//frame width for animated image
	int frameH			= 128;					//frame height for animated image
	const int maxFrame	= 4;					//number of frames in animated image

	//End animated image var

	//Initialisers
	ALLEGRO_DISPLAY			*display		= NULL;
	ALLEGRO_EVENT_QUEUE		*event_queue	= NULL;
	ALLEGRO_TIMER			*timer			= NULL;
	ALLEGRO_FONT			*font1			= NULL;
	ALLEGRO_BITMAP			*pause			= NULL;
	ALLEGRO_BITMAP			*bouncer		= NULL;
	ALLEGRO_BITMAP			*light			= NULL;
	ALLEGRO_BITMAP			*bgImage		= NULL;
	ALLEGRO_STATE			*state			= NULL;
	ALLEGRO_BITMAP			*walkLeft		= NULL,
							*walkRight		= NULL,
							*standLeft		= NULL,
							*standRight		= NULL,
							*select			= NULL,
							*poole			= NULL,
							*taps			= NULL,
							*saha			= NULL;


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

	bouncer = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);		//create box 
	if (!bouncer) {												//Check creation of box 
		fprintf(stderr, "failed to create bouncer bitmap!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);							//Create Timer
	if (!timer)													//Check timer creation
	{				
		al_show_native_message_box(display, "Error!", "Warning!", "Failed to initialise timer! \n Closing Application!", NULL, ALLEGRO_MESSAGEBOX_WARN);
		//fprintf(stderr, "failed to create timer!\n");
		al_destroy_bitmap(bouncer);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	event_queue = al_create_event_queue();						//Create event queue
	if (!event_queue)											//Check event queue creation
	{											
		al_show_native_message_box(display, "Error!", "Warning!", "Failed to initialise event queue! \n Closing Application!", NULL, ALLEGRO_MESSAGEBOX_WARN);
		//fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_bitmap(bouncer);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}


	//Init all Addons
	al_init_primitives_addon();								//load primitive (drawing shapes, etc)
	al_init_font_addon();									//load font addon
	al_init_ttf_addon();									//load truetype font addon	
	al_init_image_addon();									//load image processing addon
	al_install_keyboard();									//install keyboard
	al_install_mouse();										//install mouse
	//end addon innit
	
	//cursor = al_load_bitmap("./images/target.png");
	//custom_cursor = al_create_mouse_cursor(cursor, 0, 0);
	font1 = al_load_ttf_font("arial.ttf", 20, 0);				//Load custom font

	//Init images
	light		= al_load_bitmap("./images/c.png");
	walkRight	= al_load_bitmap("./images/kriWalkR.png");
	walkLeft	= al_load_bitmap("./images/kriWalkL.png");
	standLeft	= al_load_bitmap("./images/kriL.png");
	standRight	= al_load_bitmap("./images/kriR.png");
	select		= standRight;
	bgImage		= al_load_bitmap("./images/tbdavis.png");
	poole		= al_load_bitmap("./images/poole.png");
	saha		= al_load_bitmap("./images/saha.png");
	taps		= al_load_bitmap("./images/taps.png");
	
	int direction = 1;

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

	//End initialisers

	al_start_timer(timer);											//Start event timer (program clock)
	al_set_target_bitmap(bouncer);							
	al_clear_to_color(black);
	al_set_target_bitmap(al_get_backbuffer(display));
	al_flip_display();
	
	
	while (!done)
	{
		ALLEGRO_EVENT ev;										//Allegro event init
		al_wait_for_event(event_queue, &ev);					//wait for and accept events 

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)					//If Key down event
		{
			fired = false;
			switch (ev.keyboard.keycode)
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
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				FireBullet(bullets, NUM_BULLETS, player);			//Registers projectile character
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
				keys[LEFT] = false;
				select = standLeft;
				break;
			case ALLEGRO_KEY_P:
				if (timeM == true)
				{
					al_stop_timer(timer);
					timeM = false;
					al_draw_filled_rectangle(100, 100, scrn_W-100, scrn_H-100, white);
					al_draw_text(font1, black, scrn_W/2, 100, ALLEGRO_ALIGN_CENTRE, "PAUSE MENU");
					al_draw_text(font1, black, scrn_W / 2, scrn_H/2, ALLEGRO_ALIGN_CENTRE, "Press P to continue");
					al_draw_text(font1, black, scrn_W / 2, scrn_H / 2 + 20, ALLEGRO_ALIGN_CENTRE, "Press ESC to close game");
					al_flip_display();

				}
				else
				{
					al_start_timer(timer);
					timeM = true;	
				}
				
				
				break;
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}
		
	
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) 
		{
			crs_x = ev.mouse.x;
			crs_y = ev.mouse.y;
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

			fprintf(stderr, "\nHERE !position = x %f  y %f", crs_x, crs_y);
			crs_x = ev.mouse.x;
			crs_y = ev.mouse.y;
			fired = true;
			FireBullet(bullets, NUM_BULLETS, player);
			
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) 
		{
			
			
		}
		
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (++frameCount >= frameDelay)
			{
				if (++curFrame >= maxFrame)
				{
					curFrame = 0;
				}
				frameCount = 0;
			}
			StartEnemy(comets, NUM_COMETS);
			UpdateEnemy(comets, NUM_COMETS);
			CollideBullet(bullets, NUM_BULLETS, comets, NUM_COMETS);
			CollideEnemy(comets, NUM_COMETS, player);

			redraw = true;
			if (keys[UP])
				MoveCharacterUp(player);
			if (keys[DOWN])
				MoveCharacterDown(player);
			if (keys[LEFT])
				MoveCharacterLeft(player);
			if (keys[RIGHT])
				MoveCharacterRight(player);
			UpdateBullet(player,bullets, NUM_BULLETS, direction);


		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) 
		{
			redraw = false;

			DrawCharacter(player,select,curFrame,frameW, frameH);
			DrawBullet(bullets, NUM_BULLETS, light);

			bool test = false;
			

			switch (enem)
			{
			case 1:
				DrawEnemy(comets, NUM_COMETS, poole, curFrame, frameW, frameH);
				test = true;
				break;
			case 2:
				DrawEnemy(comets, NUM_COMETS, taps, curFrame, frameW, frameH);
				test = true;
				break;
			case 3:
				DrawEnemy(comets, NUM_COMETS, saha, curFrame, frameW, frameH);
				test = true;
				break;
			
			}

			

			//al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, green);
			//al_draw_scaled_bitmap(select, curFrame*frameW, 0, 128, 128, pos_x, pos_y, 350, 350, 0);    //makes shit big
			//al_draw_bitmap_region(select, curFrame * frameW, 0, frameW, frameH,pos_x,pos_y,0);
			//wal_draw_bitmap(select, pos_x, pos_y, 0);

			/*pos_y -= keys[UP] * 10;
			pos_y += keys[DOWN] * 10;
			pos_x -= keys[LEFT] * 10;
			pos_x += keys[RIGHT] * 10;*/

			al_flip_display();
			al_clear_to_color(black);
			al_draw_scaled_bitmap(bgImage, 0, 0, al_get_bitmap_width(bgImage), al_get_bitmap_height(bgImage),0,0,scrn_W,scrn_H, 0);
			//al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);

		}

	}

	//Destruction
	al_destroy_font(font1);
	al_destroy_bitmap(walkLeft);
	al_destroy_bitmap(walkRight);
	al_destroy_bitmap(standLeft);
	al_destroy_bitmap(standRight);
	al_destroy_bitmap(light);
	al_destroy_bitmap(bouncer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_timer(timer);
	//al_destroy_bitmap(select);
	//end Distruction
	return 0;
}



void InitCharacter(Character &player)
{
	player.x = scrn_W / 2;
	player.y = scrn_H / 2;
	player.ID = PLAYER;
	player.lives = 3;
	player.speed = 7;
	player.boundx = 6;
	player.boundy = 7;
	player.score = 0;
}

void DrawCharacter(Character &player, ALLEGRO_BITMAP *select, int cur, int fW, int fH)
{
	al_draw_scaled_bitmap(select, cur * fW, 0, fW, fH, player.x, player.y, shrinkx, shrinky, 0);
	//al_draw_bitmap_region(select, cur * fW, 0, fW, fH, player.x, player.y, 0);
	/*al_draw_filled_rectangle(player.x, player.y - 9, player.x + 10, player.y - 7, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(player.x, player.y + 9, player.x + 10, player.y + 7, al_map_rgb(255, 0, 0));

	al_draw_filled_triangle(player.x - 12, player.y - 17, player.x + 12, player.y, player.x - 12, player.y + 17, al_map_rgb(0, 255, 0));
	al_draw_filled_rectangle(player.x - 12, player.y - 2, player.x + 15, player.y + 2, al_map_rgb(0, 0, 255));
	*/
}

void MoveCharacterUp(Character &player)
{
	player.y -= player.speed;
	if (player.y < 0) player.y = 0;
	player.dir = 0;
	if (shrinkx >= 80 && shrinky >= 80)
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
	if (player.y > scrn_H-80)
		player.y = scrn_H-80;
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
void UpdateBullet(Character &player, Bullet bullet[], int size, int dir)
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
void CollideBullet(Bullet bullet[], int bSize, Enemy comets[], int cSize)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int j = 0; j < cSize; j++)
			{
				if (comets[j].live)
				{
					if (bullet[i].x >(comets[j].x - comets[j].boundx) && bullet[i].x < (comets[j].x + comets[j].boundx) &&
						bullet[i].y >(comets[j].y - comets[j].boundy) && bullet[i].y < (comets[j].y + comets[j].boundy))
					{
						bullet[i].live = false;
						comets[j].live = false;
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
	}
}
void DrawEnemy(Enemy comets[], int size, ALLEGRO_BITMAP *bit, int cur, int fW, int fH)
{
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live)
		{
			//al_draw_bitmap(bit, comets[i].x, comets[i].y, 0);
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
	
