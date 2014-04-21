//#include <Box2D/Box2D.h>
#include <math.h>
#include <objects3_Comets.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <exception>
#include <stdio.h>
#include <cstdio>


//REMEMBER TO EDIT Linker -> System -> SubSystem -> WINDOW to hide console!

//This program currently just move a green block on a black background. I'm using this to learn and test aspects

const float FPS = 60;
const int BOUNCER_SIZE = 32;
const int scrn_W = 1024;
const int scrn_H = 700;
int bulletCount = 5;
//int pos_x = scrn_W / 2;
//int pos_y = scrn_H / 2;
float shoot_x = scrn_W / 2.0;
float shoot_y = scrn_H / 2.0;
float crs_x = scrn_W / 2.0;
float crs_y = scrn_H / 2.0;

static ALLEGRO_COLOR red,blue,black,white,green;
enum KEYS{ UP, DOWN, LEFT, RIGHT, SPACE };

//prototypes
void InitShip(SpaceShip &ship);
void DrawShip(SpaceShip &ship, ALLEGRO_BITMAP *select, int cur, int fW, int fH);

void MoveShipLeft(SpaceShip &ship);
void MoveShipUp(SpaceShip &ship);
void MoveShipDown(SpaceShip &ship);
void MoveShipRight(SpaceShip &ship);


const int NUM_BULLETS = 5;
void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size, ALLEGRO_BITMAP *bit);
void FireBullet(Bullet bullet[], int size, SpaceShip &ship);
void UpdateBullet(Bullet bullet[], int size, int dir);





int main(void)
{
	 

	//int pos_x = scrn_W / 2;
	//int pos_y = scrn_H / 2;
	float bouncer_x = scrn_W / 2.0 - BOUNCER_SIZE / 2.0;
	float bouncer_y = scrn_H / 2.0 - BOUNCER_SIZE / 2.0;
	float bouncer_dx = -4.0, bouncer_dy = 4.0;
	bool done = false, fired = false, redraw = true;
	bool keys[5] = { false, false, false, false , false };
	bool time = true;

	//Character variables
	
	SpaceShip ship;
	Bullet bullets[5];
	InitShip(ship);
	InitBullet(bullets, NUM_BULLETS);



	int curFrame = 0;
	int frameCount = 0;
	int frameDelay = 20;
	int frameW = 128;
	int frameH = 128;
	const int maxFrame = 4;

	//End character variables

	//Initialisers
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font1 = NULL;
	ALLEGRO_BITMAP *pause = NULL;
	ALLEGRO_BITMAP *bouncer = NULL;
	ALLEGRO_BITMAP *light = NULL;
	ALLEGRO_STATE *state = NULL;
	ALLEGRO_BITMAP *walkLeft, *walkRight,*standLeft, *standRight,*select;


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


	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse(); 
	
	
	//cursor = al_load_bitmap("./images/target.png");
	//custom_cursor = al_create_mouse_cursor(cursor, 0, 0);
	font1 = al_load_ttf_font("arial.ttf", 20, 0);

	//Init animated character
	al_init_image_addon();
	//Init bitmap
	light = al_load_bitmap("./images/light.png");
	walkRight = al_load_bitmap("./images/kriWalkR.png");
	walkLeft = al_load_bitmap("./images/kriWalkL.png");
	standLeft = al_load_bitmap("./images/kriL.png");
	standRight = al_load_bitmap("./images/kriR.png");
	select = standRight;
	int direction = 1;

   // al_convert_mask_to_alpha(walkLeft, al_map_rgb(106, 76, 48));
	//al_convert_mask_to_alpha(select, al_map_rgb(106, 76, 48));
	//al_convert_mask_to_alpha(walkRight, al_map_rgb(106, 76, 48));


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

	al_start_timer(timer);
	al_set_target_bitmap(bouncer);
	al_clear_to_color(black);
	al_set_target_bitmap(al_get_backbuffer(display));
	//al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, green);
	al_flip_display();
	
	
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			static bool fired = false;
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				if (direction == 1) select = walkRight;
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
				FireBullet(bullets, NUM_BULLETS, ship);
				break;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
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
				if (time == true)
				{
					al_stop_timer(timer);
					time = false;
					al_draw_filled_rectangle(100, 100, scrn_W-100, scrn_H-100, white);
					al_draw_text(font1, black, scrn_W/2, 100, ALLEGRO_ALIGN_CENTRE, "PAUSE MENU");
					al_draw_text(font1, black, scrn_W / 2, scrn_H/2, ALLEGRO_ALIGN_CENTRE, "Press P to continue");
					al_draw_text(font1, black, scrn_W / 2, scrn_H / 2 + 20, ALLEGRO_ALIGN_CENTRE, "Press ESC to close game");
					al_flip_display();

				}
				else
				{

					al_start_timer(timer);
					time = true;
					
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
			FireBullet(bullets, NUM_BULLETS, ship);
			
			//al_draw_bitmap(bullet, crs_x, crs_y, 0);
			
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
			redraw = true;
			if (keys[UP])
				MoveShipUp(ship);
			if (keys[DOWN])
				MoveShipDown(ship);
			if (keys[LEFT])
				MoveShipLeft(ship);
			if (keys[RIGHT])
				MoveShipRight(ship);
			UpdateBullet(bullets, NUM_BULLETS, direction);


		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) 
		{
			redraw = false;
			//playerQuit(font);
			DrawShip(ship,select,curFrame,frameW, frameH);
			DrawBullet(bullets, NUM_BULLETS, light);
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

			al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);

		}

	}

	//Destruction

	al_destroy_bitmap(walkLeft);
	al_destroy_bitmap(walkRight);
	//al_destroy_bitmap(select);
	//al_destroy_bitmap(stand);
	al_destroy_bitmap(bouncer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_timer(timer);
	//end Distruction
	return 0;
}



void InitShip(SpaceShip &ship)
{
	ship.x = scrn_W / 2;
	ship.y = scrn_H / 2;
	ship.ID = PLAYER;
	ship.lives = 3;
	ship.speed = 7;
	ship.boundx = 6;
	ship.boundy = 7;
	ship.score = 0;
}

void DrawShip(SpaceShip &ship, ALLEGRO_BITMAP *select, int cur, int fW, int fH)
{
	al_draw_bitmap_region(select, cur * fW, 0, fW, fH, ship.x, ship.y, 0);
	/*al_draw_filled_rectangle(ship.x, ship.y - 9, ship.x + 10, ship.y - 7, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(ship.x, ship.y + 9, ship.x + 10, ship.y + 7, al_map_rgb(255, 0, 0));

	al_draw_filled_triangle(ship.x - 12, ship.y - 17, ship.x + 12, ship.y, ship.x - 12, ship.y + 17, al_map_rgb(0, 255, 0));
	al_draw_filled_rectangle(ship.x - 12, ship.y - 2, ship.x + 15, ship.y + 2, al_map_rgb(0, 0, 255));
	*/


}

void MoveShipLeft(SpaceShip &ship)
{
	ship.x -= ship.speed;
	if (ship.x < 0)
		ship.x = 0;

}
void MoveShipUp(SpaceShip &ship)
{
	ship.y -= ship.speed;
	if (ship.y < 0)
		ship.y = 0;
}
void MoveShipDown(SpaceShip &ship)
{
	ship.y += ship.speed;
	if (ship.y > scrn_H-80)
		ship.y = scrn_H-80;

}
void MoveShipRight(SpaceShip &ship)
{
	ship.x += ship.speed;
	if (ship.x > scrn_W-80)
		ship.x = scrn_W-80;
}

void InitBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		bullet[i].ID = BULLET;
		bullet[i].speed = 10;
		bullet[i].live = false;
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
void FireBullet(Bullet bullet[], int size, SpaceShip &ship)
{
	for (int i = 0; i < size; i++)
	{
		if (!bullet[i].live)
		{
			bullet[i].x = ship.x + 17;
			bullet[i].y = ship.y + 50;
			bullet[i].live = true;
			break;
		}
	}
}


void UpdateBullet(Bullet bullet[], int size, int dir)
{
	if (dir == 1)
	{
		for (int i = 0; i < size; i++)
		{
			if (bullet[i].live)
			{
			    bullet[i].x += bullet[i].speed;
				bullet[i].y += bullet[i].speed;
				if (bullet[i].x > scrn_W) bullet[i].live = false;
			}
		}
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			if (bullet[i].live)
			{
				bullet[i].x -= bullet[i].speed;
				if (bullet[i].x < 0) bullet[i].live = false;
			}
		}
	}
}

		
	
