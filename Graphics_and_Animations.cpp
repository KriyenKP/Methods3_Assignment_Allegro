#include <allegro5\allegro.h>
#include <init.h> //change to constants.h on merge
// !!!!
// you can't include init here and in learning. you can only include it once.. thats why shit is getting messy, not enough structure and stuff all over the place, I mean tbh, what is the point of init? I don't know.
// constants.h makes some sort of sense, easy place to configure game constants, but again this shouldnt be used all over the place! only included by the 'game' class and derived classes ( or whatever class is doing game stuff) 
// over and above that constants particular to a certain class should be in that classes header or .cpp (I'm not sure which is best practice tbh. ) 
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
	case NORTH:
		y -= speed;			  //move upwards
		onScreen = y < 0;	  //check if bullet moved off the screen
		break;
	case EAST:
		x += speed;			  //move right
		onScreen = x > scrn_W;//check if bullet moved off the screen
		break;
	case WEST:
		x -= speed;			  //move left
		onScreen = x < 0;	  //check if bullet moved off the screen
		break;
	case SOUTH:
		y += speed;			  //move down
		onScreen = y > scrn_H;//check if bullet moved off the screen
		break;
	default:				  //invalid direction entered
		onScreen = false;	  //deactivate bullet
		break;
	}
	return onScreen;
}

//SimpleGraphic Methods

SimpleGraphic::SimpleGraphic(){}
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