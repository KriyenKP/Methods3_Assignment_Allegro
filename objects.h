//Object IDS
enum IDS{PLAYER, BULLET, ENEMY};

//Our Player
struct Character
{
	int ID;
	int x;
	int y;
	int lives;
	int speed;
	int dir;
	int boundx;
	int boundy;
	int score;
};

struct Bullet
{
	int ID;
	int x;
	int y;
	int dir;
	bool live;
	int speed;
};

struct Enemy
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;
};