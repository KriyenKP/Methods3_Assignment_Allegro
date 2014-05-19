#pragma once
#include <Graphics_and_Animations.h>
#include <Gameplay.h>
class Bullet : public SimpleGraphic, public Gameplay
{
private: const int speed = 10;
		 int direction;
public:	 Bullet();
		 void changeDir(int);
		 void update();
};

class Enemy : public SimpleGraphic, public Gameplay
{
private: int speed;
		 int startX;
		 int startY;
public: Enemy();
		bool start();
		void attackPlayer(); //formerly update in prev versions
};

class Boss : public SimpleGraphic, public Gameplay
{
private: int speed;
		 int health;
		 int startX;
		 int startY;
public: Boss();
		void start();
		void attackPlayer(); //formerly update in prev versions
		void drawBoss(ALLEGRO_BITMAP *, int, int, int);
};

class PowerUp : public SimpleGraphic, public Gameplay
{
private: int speed;
		 int startX;
		 int startY;
public: PowerUp();
		bool start();
		void approach(); //formerly update in prev versions
};

class Explosions: public Animation, public Gameplay
{
public: Explosions();
		Explosions(ALLEGRO_BITMAP *);
		void update();

};