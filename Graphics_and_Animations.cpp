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
	case 0:
		y -= speed;			  //move upwards
		onScreen = y > 0;	  //check if bullet moved off the screen
		break;
	case 1:
		x += speed;			  //move right
		onScreen = x < 1024;//check if bullet moved off the screen
		break;
	case 3:
		x -= speed;			  //move left
		onScreen = x > 0;	  //check if bullet moved off the screen
		break;
	case 2:
		y += speed;			  //move down
		onScreen = y < 686;//check if bullet moved off the screen
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


/*
*	Game comprises of 5 general elements:
*		1) Avatar (prev Character). User-controlled.
*		2) Enemy (prev Projectile). Standard opponent.
*		3) Boss. Opponent to be destroyed at end of level.
*		4) Bullet. Thrown by Avatar when attacking Enemy/Boss
*		5) Explosion. Occurs when Bullet and Enemy/Boss collide
*		6) PowerUp
*/


Bullet::Bullet()
{
	setActive(false);
}
Bullet::Bullet(int newDir)
{
	setActive(false);
	direction = newDir;
}
void Bullet::changeDir(int newDir)
{
	direction = newDir;
}
void Bullet::update()
{
	bool remainActive = move(speed, direction);
	if (!remainActive)
		setActive(false);
}

//NB for implementation
//initBullet incorporated into constructor
//DrawBullet() in old code should be largely the same, calling .draw()
//findDeadBullet() would be unchanged
//updateBullet() to be implemented with loop, changeDir() and update()
//collideBullet() to be implemented with the collisionDetection stuff from Gameplay


Enemy::Enemy(int bX = 110, int bY = 120)
{
	speed = 3 * (1 + rand() % 3);
	setActive(false);
	boundX = bX;
	boundY = bY;
}