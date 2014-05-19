#include <Graphics_and_Animations.h>
#include <allegro5\allegro.h>
#include <GameElements.h>

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

Enemy::Enemy()
{
	speed = 3 * (1+ rand() % 3);
	setActive(false);
	boundX = 110;
	boundY = 120;
}
bool Enemy::start() //to be called in a loop, where a true return breaks out of the loop
{
	bool flag = false;
	if (!checkActive())
	{
		if (rand() % 500 == 0)
		{
			setActive(true);
			setX(1024);
		retry:
			int y = 30 + rand() % (666);
			if (y < 475)
			{
				setY(y);
				startY = y;
			}
			else
				goto retry;
			flag = true;
		}
	}
	return flag;
}
void Enemy::attackPlayer()
{
	if (checkActive())
		if (!move(speed, 3)) //left = 3
			setActive(false);
}


PowerUp::PowerUp()
{
	speed = 3;
	setActive(false);
	boundX = 110;
	boundY = 120;
}
bool PowerUp::start() //to be called in a loop, where a true return breaks out of the loop
{
	bool flag = false;
	if (!checkActive())
	{
		if (rand() % 500 == 0)
		{
			setActive(true);
			setX(1024);
		retry:
			int y = 30 + rand() % (666);
			if (y < 475)
			{
				setY(y);
				startY = y;
			}
			else
				goto retry;
			flag = true;
		}
	}
	return flag;
}
void PowerUp::approach()
{
	int sign;
	if (y >= (startY + 100)) sign = 1;
	if (y < 0) y = 656;
	if (y <= (startY - 100) || y > 686) sign = -1;
	if (!move(speed*sign, 3))
		setActive(false);
}

Boss::Boss()
{
	speed = 1;
	setActive(false);
	boundX = 50;
	boundY = 50;
	health = 30;
}
void Boss::drawBoss(ALLEGRO_BITMAP *pic, int cur, int fW, int fH)
{
	draw(pic, cur, fW, fH);
	drawHealthBar(x, y, health);
}
void Boss::start()
{
	if (!checkActive())
	{
		setActive(true);
		x = 1024;
		y = 1024 / 2 - 150;
	}
}
void Boss::attackPlayer()
{
	if (checkActive())
	{
		if (x > 1024 / 2 - 50)
			move(speed, 3);//move left
		else
		{
			move(y <= 0 ? -speed : speed, 0);//move up or down
		}
	}
}

Explosions::Explosions()
{
	setActive(false);
	maxFrame = 4;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 1;
	frameWidth = 128;
	frameHeight = 128;
	animationColumns = 8;
	animationDirection = 1;
}
Explosions::Explosions(ALLEGRO_BITMAP *img)
{
	Explosions();
	if (img != NULL)
		image = img;
}
void Explosions::update()
{
	if (checkActive())
	{
		if (++frameCount >= frameDelay)
		{
			curFrame += animationDirection;
			if (curFrame >= maxFrame)
			{
				curFrame = 0;
				setActive(false);
			}
			frameCount = 0;
		}
	}
}