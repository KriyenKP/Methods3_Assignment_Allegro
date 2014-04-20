#include <Box2D\Box2D.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <exception>
#include <stdio.h>
#include <cstdio>

const float FPS = 60;
const int BOUNCER_SIZE = 32;
const int scrn_W = 640;
const int scrn_H = 480;

struct Character
{
	int x;
	int y;
	int speed;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int frameDir;

	enum Direction { LEFT, RIGHT, UP, DOWN };
};

enum KEYS { UP, DOWN, LEFT, RIGHT };
bool keys[4] = { false, false, false, false };

void InitPlayer(Character &player, ALLEGRO_BITMAP *image);

int main(void)
{
	bool done = false;

	//allegro variable
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_BITMAP *image;

	//Objects
	Character player;

	//program init
	if (!al_init())                    //initialize Allegro
		return -1;

	display = al_create_display(scrn_W, scrn_H);      //create our display object

	if (!display)                    //test display object
		return -1;

	//addon init
	al_install_keyboard();
	al_init_image_addon();

	image = al_load_bitmap("images/fliegt w.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(106, 76, 48));

	InitPlayer(player, image);

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (keys[DOWN])
			{
				player.frameDir = 0;
				if (++player.frameCount >= player.frameDelay)
				{
					if (++player.curFrame >= player.maxFrame)
						player.curFrame = 0;

					player.frameCount = 0;
				}

				player.y += player.speed;
			}
			else if (keys[UP])
			{
				player.frameDir = 3;
				if (++player.frameCount >= player.frameDelay)
				{
					if (++player.curFrame >= player.maxFrame)
						player.curFrame = 0;

					player.frameCount = 0;
				}

				player.y -= player.speed;
			}
			else if (keys[LEFT])
			{
				player.frameDir = 1;
				if (++player.frameCount >= player.frameDelay)
				{
					if (++player.curFrame >= player.maxFrame)
						player.curFrame = 0;

					player.frameCount = 0;
				}

				player.x -= player.speed;
			}
			else if (keys[RIGHT])
			{
				player.frameDir = 2;
				if (++player.frameCount >= player.frameDelay)
				{
					if (++player.curFrame >= player.maxFrame)
						player.curFrame = 0;

					player.frameCount = 0;
				}

				player.x += player.speed;

			}
		}

		al_draw_bitmap_region(image, player.curFrame * player.frameWidth, player.frameDir * player.frameHeight, player.frameWidth, player.frameHeight, player.x, player.y, 0);

		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}


	al_destroy_event_queue(event_queue);
	al_destroy_display(display);            //destroy our display object

	return 0;
}

void InitPlayer(Character &player, ALLEGRO_BITMAP *image)
{
	player.maxFrame = 4;
	player.curFrame = 0;
	player.frameCount = 0;
	player.frameDelay = 10;
	player.frameWidth = 100;
	player.frameHeight = 100;
	player.frameDir = 0;
	player.speed = 2;
	player.x = scrn_W/ 2;
	player.y = scrn_H / 2;
}