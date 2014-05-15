//objects

enum IDS { PLAYER, BULLET, PROJECTILE, BOSS };

//player

struct Character {
	int ID;
	int spritex;
	int spritey;
	int lives;
	int dir;
	int speed;
	int boundx;
	int boundy;
	int score = 0;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;

	int animationRow;

	ALLEGRO_BITMAP *image;
};

struct Bullet {
	int ID;
	int x;
	int y;
	int dir;
	bool live;
	int speed;

};

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
