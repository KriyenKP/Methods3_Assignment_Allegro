/*
This file contains structs for onscreen objects
*/

struct Projectile {
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;
	int start_x;
	int start_y;
};

struct Boss {
	int ID;
	int x;
	int y;
	bool live;
	int lives;
	int speed;
	int boundx;
	int boundy;

	/*int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;*/

	ALLEGRO_BITMAP *image;
};

struct Explosion {
	int x;
	int y;
	bool live;

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
