#include <allegro5\allegro.h>
#pragma once

class DynamicImg{
protected:
	int x;					//x-coord relative to fixed top-left axis
	int y;					//y-coord relative to fixed top-left axis
	bool active;			//indicates whether image is destroyed or not, to prevent memory leaks
	int boundX;		//used for determining width
	int boundY;		//used for determining height
public:
	DynamicImg(bool);
	DynamicImg(int, int);
	DynamicImg();
	~DynamicImg();
	virtual void draw();
	void setX(int);
	void setY(int);
	void setActive(bool);
	int getX() const;
	int getY() const;
	int getBoundX() const;
	int getBoundY() const;
	bool checkActive() const;
	bool move(int, int);	//returns false if graphic moves off the screen
};

class Animation : public DynamicImg{
public: Animation(int, int);
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
	SimpleGraphic(int, int);
	SimpleGraphic();
	virtual void draw(ALLEGRO_BITMAP *);
	virtual void draw(ALLEGRO_BITMAP *, int, int, int);
	static void drawHealthBar(int, int, int);
private:
	void toggleActive();
};