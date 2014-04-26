#include <allegro5/allegro.h>
#include <ctime>
#define maxbullets 30  //define maximum number of bullets on screen.

int screen_h = 480; //screen height
int screen_w = 640; //screen width
int canshoot = 1; //to aid in stopping all of the bullets to be unleashed in one go
volatile long speed_counter = 0;  // timer stuff

BITMAP *buffer;

void increment_speed_counter() //timer stuff
{
	speed_counter++;
}

class Bullet
{
public:
	int bx;           //x value for bullet
	int by;           //y value for bullet
	int bs;           //speed bullet travels across screen
	int be;           //does the bullet exist on screen (1 yes, 0 no)
	Bullet();
	void shoot();
	void movebullets();
private:
};

Bullet::Bullet()
{
	bx = 0;
	by = screen_h / 2;
	bs = 3;
	be = 0;
}

void Bullet::shoot()
{
	circlefill(buffer, bx, by, 1, makecol(255, 0, 0)); //draws bullet on screen
	be = 1;            //indicates bullet is on screen
}

void Bullet::movebullets()
{
	circlefill(buffer, bx, by, 1, makecol(0, 0, 0));  //masks previous bullet location
	bx += bs;          // moves bullet to new location
	circlefill(buffer, bx, by, 1, makecol(255, 0, 0));  // draws bullet in new location
	if (bx > screen_w)
	{
		be = 0;  //if the bullet goes off screen bullet doesn't need to be drawn
	}
}

int main()   //all the usual allegro initialisations here
{
	allegro_init();
	install_timer();
	LOCK_VARIABLE(speed_counter);
	LOCK_FUNCTION(increment_speed_counter);
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));
	install_keyboard();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, screen_w, screen_h, 0, 0);
	buffer = create_bitmap(screen_w, screen_h);
	Bullet bullet[maxbullets]; // create array of bullets

	while (!key[KEY_ESC])  //define esc key to quit
	{
		while (speed_counter > 0)  //timer stuff
		{
			if (key[KEY_SPACE]) //press space to shoot
			{
				for (int i = 0; i < maxbullets; i++) //cycle through aray
				if (bullet[i].be == 0 && canshoot == 1) //if bullet doesn't already exist and we can shoot...shoot
				{
					bullet[i].bx = 0; //define where the x position of the bullet is going to originate from
					bullet[i].by = screen_h / 2;    //define y position of bullet
					bullet[i].shoot(); //initial bullet creation
					canshoot = 0;      //prevent space bar bing held down.
				}
			}
			if (!key[KEY_SPACE])  //check to see if space bar has been released
			{
				if (canshoot == 0)
				{
					canshoot = 1; //change varialbe so that you can shoot again
				}
			}

			for (int i = 0; i < maxbullets; i++) // for all bullets on screen, update their position.
			if (bullet[i].be == 1)
			{
				bullet[i].movebullets();
			}
			for (int i = 0; i < maxbullets; i++) //this bit is a check to write on screen whether......
			{
				int status = bullet[i].be; //does bullet exist
				textprintf(buffer, font, 0, (i * 10), makecol(255, 255, 255), "%i", status);

			}
			textprintf(buffer, font, 20, 0, makecol(255, 255, 255), "%i", canshoot); //are you allowed to shoot                                             
			speed_counter--;
		}
		draw_sprite(screen, buffer, 0, 0); // write buffer to screen.    

	}
	return 0;
}
END_OF_MAIN();