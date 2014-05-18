#include <allegro5\allegro.h>

/*
*	Abstract Base Class DynamicImg used as a template for images in the game which move across the screen.
*
*	Derived Classes from DynamicImg are: 
*	1) SimpleGraphic 
*	   simple graphics are single images that move across the screen
*	2) Animation
*	   animations cycle through a series of images with each refresh of the game screen, and move across the screen.
*
*	Old Version Structure	  |		New Version Class
*							  |
*	Bullet					  |		SimpleGraphic
*	Projectile				  |		SimpleGraphic
*	Boss					  |		SimpleGraphic
*	Explosion				  |		Animation
*	Character				  |		Animation
*
*	$	Big picture plan: Write a class for Bullet, Projectile, etc that inherits from 2 sources.
*			source 1 - a class derived from DynamicImg that handles the graphics-related aspects.
*			source 2 - a class derived from Gameplay that handles the aspects related to an object's role in the game.
*				       {Gameplay class structure not designed yet, but this way we're being good OOP programmers
*				        Keep graphics management and gameplay management separated and then write a class that incorporates both}
*
*	^ a $ indicates Sulaiman-style explanation talk, to be modified prior to submission
*/

class DynamicImg{
protected: 
	int x;					//x-coord relative to fixed top-left axis
	int y;					//y-coord relative to fixed top-left axis
	bool active;			//indicates whether image is destroyed or not, to prevent memory leaks
							//previously variable 'live'
							//$ may be able to make this a protected variable
	int bounds[2];			//used for determining height and width of image
							//bounds[0] = width		bounds[1] = height
public:
	virtual void draw() = 0;
	void setX(int);
	void setY(int);
	int getX();
	int getY();
	int getBound(int);
	
	static int X_BOUND_INDEX;
	static int Y_BOUND_INDEX;
};

class Animation : public DynamicImg{
public: Animation(){};			
		int maxFrame;
		int curFrame;
		int frameCount;
		int frameDelay;
		int frameWidth;
		int frameHeight;
		int animationColumns;
		int animationDirection;
		ALLEGRO_BITMAP *image;
};

class SimpleGraphic : public DynamicImg{
public: 
	SimpleGraphic();
	SimpleGraphic(bool);
	virtual void draw(ALLEGRO_BITMAP *);
	virtual void draw(ALLEGRO_BITMAP *, int, int, int);
private:
	void setActive(bool isActive);
};

//DynamicImg

void DynamicImg::setX(int xVal)
{
	x = xVal;
}

void DynamicImg::setY(int yVal)
{
	y = yVal;
}

int DynamicImg::X_BOUND_INDEX = 0;
int DynamicImg::Y_BOUND_INDEX = 1;

//SimpleGraphic Methods

SimpleGraphic::SimpleGraphic(bool isActive)
{
	setActive(isActive);
}

void SimpleGraphic::setActive(bool isActive)
{
	active = isActive;
}

void SimpleGraphic::draw(ALLEGRO_BITMAP *bitmap)
{
	if (active)
		al_draw_bitmap(bitmap, x, y, 0);
}

void SimpleGraphic::draw(ALLEGRO_BITMAP *bitmap, int cur, int fW, int fH)
{
	if (active)
		al_draw_bitmap_region(bitmap, cur * fW, 0, fW, fH, x, y, 0);
}