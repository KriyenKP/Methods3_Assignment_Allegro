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
	int frameWidth = 128;
	int frameHeight = 128;

	//allegro variable
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_BITMAP *image,*imager;
	ALLEGRO_BITMAP *select;

	//program init
	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(width, height);			//create our display object

	if (!display)										//test display object
		return -1;

	//addon init
	al_install_keyboard();
	al_init_image_addon();
	
	image = al_load_bitmap("./images/fliegt w.bmp");
	select = image;
	imager = al_load_bitmap("./images/fliegt e.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(106, 76, 48));
	al_convert_mask_to_alpha(imager, al_map_rgb(106, 76, 48));

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
				select = image;
				break;
			case ALLEGRO_KEY_RIGHT:
				select = imager;
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

			x -= 5;

			if (x <= 0 - frameWidth)
				x = width;
		}

		al_draw_bitmap_region(select, curFrame * frameWidth, 0, frameWidth, frameHeight, x, y, 0);

		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}


	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						//destroy our display object

	return 0;
}