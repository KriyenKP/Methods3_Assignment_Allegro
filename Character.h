#pragma once
#if !defined(CONSTANTS_H)
#include "constants.h"
#endif
class Character
{
public:
	Character();
	~Character();
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
	void InitCharacter();
	void MoveCharacterUp();
	void MoveCharacterRight();
	void MoveCharacterDown();
	void MoveCharacterLeft();
};

