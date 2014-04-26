#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include <stdio.h>

const float FPS = 60;
const int BOUNCER_SIZE = 32;
const int scrn_W = 640;
const int scrn_H = 480;
int pos_x = scrn_W / 2;
int pos_y = scrn_H / 2;
static ALLEGRO_COLOR red, blue, black, white, green;
enum KEYS{ UP, DOWN, LEFT, RIGHT };

int main(void)
{
	bool done = false;
	int curFrame = 0;
	int frameCount = 0;
	int frameDelay = 5;
	const int maxFrame = 8;
	
	//Initialisers
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *bouncer = NULL;
	ALLEGRO_BITMAP *image[maxFrame];

	if (!al_init())											//initialize and check Allegro
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	display = al_create_display(scrn_W, scrn_H);			//create our display object
	if (!display)											//Check display
	{
		fprintf(stderr, "failed to create display!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	bouncer = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);		//create box limit
	if (!bouncer) {												//Check creation of box limit
		fprintf(stderr, "failed to create bouncer bitmap!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);							//Create Timer
	if (!timer)													//Check timer creation
	{
		fprintf(stderr, "failed to create timer!\n");
		al_destroy_bitmap(bouncer);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	event_queue = al_create_event_queue();						//Create event queue
	if (!event_queue) {											//Check event queue creation
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_bitmap(bouncer);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	
	al_init_image_addon();
	//Init bitmap
	image[0] = al_load_bitmap("./images/dragon/fliegt e0000.bmp");
	image[1] = al_load_bitmap("./images/dragon/fliegt e0001.bmp");
	image[2] = al_load_bitmap("./images/dragon/fliegt e0002.bmp");
	image[3] = al_load_bitmap("./images/dragon/fliegt e0003.bmp");
	image[4] = al_load_bitmap("./images/dragon/fliegt e0004.bmp");
	image[5] = al_load_bitmap("./images/dragon/fliegt e0005.bmp");
	image[6] = al_load_bitmap("./images/dragon/fliegt e0006.bmp");
	image[7] = al_load_bitmap("./images/dragon/fliegt e0007.bmp");
	
	for (int i = 0; i < maxFrame; i++)
	{
		al_convert_mask_to_alpha(image[i], al_map_rgb(106, 76, 48));
	}

	//end bitmap

	//Event queue - register listeners
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//end event queue

	//Colours
	black = al_map_rgb(0, 0, 0);
	white = al_map_rgb(255, 255, 255);
	red = al_map_rgb(255, 0, 0);
	green = al_map_rgb(0, 255, 0);
	blue = al_map_rgb(0, 0, 255);
	//End Colours

	al_start_timer(timer);


	//End initialisers
	
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

				break;
			case ALLEGRO_KEY_RIGHT:

				break;
			case ALLEGRO_KEY_UP:

				break;
			case ALLEGRO_KEY_DOWN:

				break;
			}
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
		}

		al_draw_bitmap(image[curFrame], 200, 100, 0);


		al_flip_display();
		al_clear_to_color(black);
	}


	//Destruction
	
	for (int i = 0; i < maxFrame; i++)
	{
		al_destroy_bitmap(image[i]);
	}
	al_destroy_bitmap(bouncer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_timer(timer);
	//end Distruction

	return 0;
}