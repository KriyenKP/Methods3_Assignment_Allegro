
#include <cstdlib>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

struct CBox
{
	CBox(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h)
	{
	}
	CBox(const CBox& other)
	{
		x = other.x;
		y = other.y;
		w = other.w;
		h = other.h;
	}
	bool collides(const CBox& other)
	{
		return !(other.x + other.w < x || other.y + other.h < y || other.x > x + w || other.y > y + h);
	}
	int x;
	int y;
	int w;
	int h;
};

class CPlayer
{
public:
	CPlayer(CBox p, ALLEGRO_COLOR col) : box(p), color(col)
	{
		movy = 0.0;
	}
	void setScore(int ns) { score = ns; }
	int getScore()	{ return score; }

	void setBox(const CBox& b) { box = b; }
	CBox getBox() { return box; }

	void setYMovement(double nmy){ movy = nmy; }
	double getYMovement(){ return movy; }

	void move(CBox& bounds)
	{
		//make sure the player doesn't go off-bounds
		if ((movy < 0) &&(box.y + movy < bounds.y))
		{
			box.y = bounds.y;
			movy = 0;
		}
		else if ((movy > 0) && (box.y + box.h + movy > bounds.y + bounds.h))
		{
			box.y = bounds.y + bounds.h - box.h;
			movy = 0;
		}
		box.y += movy;
		//players can't move horizontally, so no bounds checking in that matter
	}

	void draw()
	{
		al_draw_filled_rectangle(box.x, box.y, box.x + box.w, box.y + box.h, color);
	}

private:
	int score;
	CBox box;
	ALLEGRO_COLOR color;
	double movy;
};

class CBall
{
public:
	CBall(CBox p, ALLEGRO_COLOR col) : box(p), color(col), lt(3)
	{
	}
	void setXYMovement(double nmx, double nmy)
	{
		movy = nmy;
		movx = nmx;
	}

	//ghostbox is the precalculated ball's trajectory
	int move(const CBox& bounds, CPlayer* plys)
	{
		CBox ghostbox(box.x + movx, box.y + movy, box.w, box.h);
		//test collision for both players
		for (int i = 0; i<2; i++)
		{
			//a player cannot touch the ball twice in a row
			if (i != lt)
			{
				CBox other = plys[i].getBox();
				if (ghostbox.collides(other))
				{
					//set the last touch to this player
					lt = i;

					//negate the "ghost movement" in x axis
					ghostbox.x -= movx;
					//bounce horizontally
					movx = -movx;
					//bounce vertically to change the ball's trajectory
					movy = (((box.y + box.h / 2.0) - (other.y + other.h / 2.0)) / other.h) * 10;
					break;
				}
			}
		}
		if (ghostbox.y < bounds.y)
		{
			ghostbox.y = bounds.y;
			movy = -movy;
		}
		else if (ghostbox.y + ghostbox.h > bounds.y + bounds.h)
		{
			ghostbox.y = bounds.y + bounds.h - ghostbox.h;
			movy = -movy;
		}

		if (ghostbox.x + ghostbox.w < bounds.x)
		{
			box.x = bounds.x + bounds.w / 2 - box.w / 2;
			box.y = bounds.y + bounds.h / 2 - box.h / 2;
			return 2;
		}
		else if (ghostbox.x > bounds.x + bounds.w)
		{
			box.x = bounds.x + bounds.w / 2 - box.w / 2;
			box.y = bounds.y + bounds.h / 2 - box.h / 2;
			return 1;
		}
		box = ghostbox;
		return 0;
	}
	void draw()
	{
		al_draw_filled_circle(box.x + box.w / 2, box.y + box.h / 2, box.w / 2, color);
	}
private:
	CBox box;
	ALLEGRO_COLOR color;
	double movy;
	double movx;
	int lt;
};

int main(int argc, char** argv)
{
	int defw = 640;
	int defh = 480;
	if (argc == 3)
	{
		/*the program was run using 3 arguments
		argv[0] = name of the executable file
		argv[1] = screen width (as a string)
		argv[2] = screen height (as a string)
		*/
		defw = atoi(argv[1]);
		defh = atoi(argv[2]);
	}
	if (!al_init()) return 1; //exit if can't initialize allegro
	//initialize most stuff
	al_install_keyboard();
	al_init_primitives_addon();
	//run in a window
	al_set_new_display_flags(ALLEGRO_WINDOWED);

	//initialize our display and event queue
	ALLEGRO_DISPLAY* display = al_create_display(defw, defh);
	ALLEGRO_EVENT_QUEUE* eventq = al_create_event_queue();

	//this timer will tick once per frame
	ALLEGRO_TIMER* framet = al_create_timer(1.0 / 30.0); //30 frames per second
	al_start_timer(framet);

	//register the event sources so they send events to our queue
	al_register_event_source(eventq, al_get_display_event_source(display)); //display/window
	al_register_event_source(eventq, al_get_keyboard_event_source()); //keyboard
	al_register_event_source(eventq, al_get_timer_event_source(framet)); //fps timer

	//this box is our play field (covers the whole screen)
	CBox fieldbox(0, 0, defw, defh);

	//we setup the ball at the center of the screen with white color
	CBall ball(CBox(defw / 2 - 10, defh / 2 - 10, 20, 20), al_map_rgb(255, 255, 255));
	//we tell it to move to the left
	ball.setXYMovement(-5.0, 0.0);

	//we initialize our both players in an array
	CPlayer players[2] = {
		//red player on the left
		CPlayer(CBox(10, defh / 2 - 80 / 2, 20, 80), al_map_rgb(255, 0, 0)),
		//blue player on the right
		CPlayer(CBox(defw - 10 - 20, defh / 2 - 80 / 2, 20, 80), al_map_rgb(0, 0, 255)),
	};

	/*when this variable is set to true
	the program will quit the main loop
	and free the allocated resources
	before quitting */
	bool exit = false;
	while (!exit)
	{
		al_wait_for_event(eventq, NULL);
		ALLEGRO_EVENT ev;
		while (al_get_next_event(eventq, &ev))
		{
			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				if (ev.timer.source == framet)
				{
					//fill the screen with black
					al_clear_to_color(al_map_rgb(0, 0, 0));
					//move and draw our two players
					for (int i = 0; i<2; i++)
					{
						players[i].move(fieldbox);
						players[i].draw();
					}
					//move, collide and draw the ball
					switch (ball.move(fieldbox, players))
					{
					case 0:
						break;
					case 1:
						players[0].setScore(players[0].getScore() + 1);
						ball.setXYMovement(5.0, 0.0);
						break;
					case 2:
						players[1].setScore(players[1].getScore() + 1);
						ball.setXYMovement(-5.0, 0.0);
						break;
					}
					ball.draw();
					//show what we've drawn
					al_flip_display();
				}
			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				//quit if the user tries to close the window
				if (ev.display.source == display) exit = true;
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				//handle key presses
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_W:
					players[0].setYMovement(-3.0);
					break;
				case ALLEGRO_KEY_S:
					players[0].setYMovement(3.0);
					break;
				case ALLEGRO_KEY_UP:
					players[1].setYMovement(-3.0);
					break;
				case ALLEGRO_KEY_DOWN:
					players[1].setYMovement(3.0);
					break;
				case ALLEGRO_KEY_ESCAPE:
					exit = true;
					break;
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					exit = true;
					break;
				}
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_UP)
			{
				int code = ev.keyboard.keycode;
				/*avoid clumsy movement making sure the released key corresponds with
				the moving direction*/
				if (code == ALLEGRO_KEY_W && players[0].getYMovement() < 0)
					players[0].setYMovement(0.0);
				else if (code == ALLEGRO_KEY_S && players[0].getYMovement() > 0)
					players[0].setYMovement(0.0);
				else if (code == ALLEGRO_KEY_UP && players[1].getYMovement() < 0)
					players[1].setYMovement(0.0);
				else if (code == ALLEGRO_KEY_DOWN && players[1].getYMovement() > 0)
					players[1].setYMovement(0.0);
			}
		}
	}
	al_destroy_event_queue(eventq);
	al_destroy_timer(framet);
	al_destroy_display(display);
}