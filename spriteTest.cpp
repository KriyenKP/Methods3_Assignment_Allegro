#include <iostream>  

#include <string> 
#include <allegro.h> 

using namespace std;

BITMAP *buffer;
BITMAP *sprite;
int pic_positiony = 100; // x position of the sprite 
int pic_positionx = 600;// y position of the sprite 
int frame = 0; // holds each frame. 


int main(int argc, char *argv[]) {

	allegro_init();
	install_keyboard();
	install_mouse();
	set_color_depth(16);
	if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN,
		640, 480, 0, 0)<0)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Failure to init video mode!\n%s\n",
			allegro_error);
	}

	enum Direction { left, right }; // the equivalent of the next to commented lines. 
	//int left = 0; 
	//int right= 1; 

	// Width and height of the sonic sprites. 
	int width = 40;
	int height = 49;

	sprite = load_bitmap("sonic walk.bmp", NULL);
	buffer = create_bitmap(640, 480);

	while (!key[KEY_ESC])
	{
		if (key[KEY_LEFT])
		{
			Direction dir = left;// making the direction left when the key left is pressed. 

			// blit is just drawing the sprite based on the direction and frame value. 
			masked_blit(sprite, buffer, frame*width, dir*height, pic_positionx, pic_positiony, width, height);
			draw_sprite(screen, buffer, 0, 0);
			rest(100);
			clear_bitmap(buffer);
			//draw_sprite(screen,buffer,0,0);// drawing the buffer to the screen. 

			frame++; // changes the frames while it's moving. 
			pic_positionx -= 10; // moving the sprite to the left by 10 

			// if at the last frame return to the first one. 
			if (frame == 9)
			{
				frame = 0;
			}
		}

		// see if(key[Key_Left]) section. 
		if (key[KEY_RIGHT])
		{
			Direction dir = right;
			masked_blit(sprite, buffer, frame*width, dir*height, pic_positionx, pic_positiony, width, height);
			draw_sprite(screen, buffer, 0, 0);
			rest(100);
			clear_bitmap(buffer);
			//draw_sprite(screen,buffer,0,0); 
			frame++;
			pic_positionx += 10;

			if (frame == 9)
			{
				frame = 0;
			}
		}


	}
	destroy_bitmap(sprite);
	cout << "Hello world!" << endl;
	return 0;
}
END_OF_MAIN();
