#include <Graphics_and_Animations.h>
#include <allegro5\allegro.h>
#include <Gameplay.h>

/*
*	Game comprises of 5 general elements:
*		1) Avatar (prev Character). User-controlled.
*		2) Enemy (prev Projectile). Standard opponent.
*		3) Boss. Opponent to be destroyed at end of level.
*		4) Bullet. Thrown by Avatar when attacking Enemy/Boss
*		5) Explosion. Occurs when Bullet and Enemy/Boss collide
*/

class Bullet : public SimpleGraphic, public Gameplay
{
private: const int speed = 10;
		 int direction;
public:	 Bullet();
		 void changeDir(int);
		 void update();
};

Bullet::Bullet()
{
	setActive(true);
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
