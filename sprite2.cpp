#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

int main(void)
{
	//variables
	int width = 640;
	int height = 480;
	bool done = false;

	int x = width / 2;
	int y = height / 2;

	const int maxFrame = 8;
	int curFrame = 0;
	int frameCount = 0;
	int frameDelay = 5;

	//allegro variable
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_BITMAP *image[maxFrame];

	//program init
	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(width, height);			//create our display object

	if (!display)										//test display object
		return -1;

	//addon init
	al_install_keyboard();
	al_init_image_addon();

	image[0] = al_load_bitmap("./images/dragon/fliegt e0000.bmp");
	image[1] = al_load_bitmap("./images/dragon/fliegt e0001.bmp");
	image[2] = al_load_bitmap("./images/dragon/fliegt e0002.bmp");
	image[3] = al_load_bitmap("./images/dragon/fliegt e0003.bmp");
	image[4] = al_load_bitmap("./images/dragon/fliegt e0004.bmp");
	image[5] = al_load_bitmap("./images/dragon/fliegt e0005.bmp");
	image[6] = al_load_bitmap("./images/dragon/fliegt e0006.bmp");
	image[7] = al_load_bitmap("./images/dragon/fliegt e0007.bmp");

	for (int i = 0; i < maxFrame; i++)
		al_convert_mask_to_alpha(image[i], al_map_rgb(106, 76, 48));

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
					curFrame = 0;

				frameCount = 0;
			}

		}

		al_draw_bitmap(image[curFrame], x, y, 0);

		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}

	for (int i = 0; i < maxFrame; i++)al_destroy_bitmap(image[i]);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						//destroy our display object

	return 0;
}