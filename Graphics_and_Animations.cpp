#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <Graphics_and_Animations.h>

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
*	^ a $ indicates Sulaiman-style explanation talk, to be modified prior to submission << lol. xD
*/

//Animations
void Animation::draw()
{
	if (checkActive)
	{
		int fx = (curFrame%animationColumns)*frameWidth;
		int fy = (curFrame/animationColumns)*frameWidth;
		al_draw_bitmap_region(image, fx, fy,frameWidth,
			frameHeight, x - frameWidth / 2, y - frameHeight / 2, 0);
	}
}

//DynamicImg
DynamicImg::DynamicImg(int bX, int bY)
: boundX(bX),
  boundY(bY)
{

}
DynamicImg::DynamicImg():boundX(0), boundY(0){}
DynamicImg::~DynamicImg(){}
void DynamicImg::draw(){}
void DynamicImg::setX(int xVal)
{
	x = xVal;
}
void DynamicImg::setY(int yVal)
{
	y = yVal;
}
void DynamicImg::setActive(bool isActive)
{
	active = isActive;
}
bool DynamicImg::checkActive() const
{
	return active;
}
int DynamicImg::getX() const
{
	return x;
}
int DynamicImg::getY() const
{
	return y;
}
int DynamicImg::getBoundX() const
{
	return boundX;
}
int DynamicImg::getBoundY() const
{
	return boundY;
}
bool DynamicImg::move(int speed, int direction)
{
	bool onScreen = true;
	switch (direction)
	{
	case 0:
		y -= speed;			  //move upwards
		onScreen = y < 0;	  //check if bullet moved off the screen
		break;
	case 1:
		x += speed;			  //move right
		onScreen = x > 1024;//check if bullet moved off the screen
		break;
	case 3:
		x -= speed;			  //move left
		onScreen = x < 0;	  //check if bullet moved off the screen
		break;
	case 2:
		y += speed;			  //move down
		onScreen = y > 686;//check if bullet moved off the screen
		break;
	default:				  //invalid direction entered
		onScreen = false;	  //deactivate bullet
		break;
	}
	return onScreen;
}

//SimpleGraphic Methods

SimpleGraphic::SimpleGraphic(){}
SimpleGraphic::SimpleGraphic(int bX, int bY)
{
	boundX = bX;
	boundY = bY;
}
void SimpleGraphic::toggleActive()
{
	active = !active;
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
void SimpleGraphic::drawHealthBar(int x, int y, int healthScalar )
{
	al_draw_filled_rectangle(x + 30, y - 5, x + 300 + 40, y + 15, al_map_rgb(0,0,0));
	al_draw_filled_rectangle(x + 35, y, x + (healthScalar * 10) + 35, y + 10, al_map_rgb(255,0,0));
}