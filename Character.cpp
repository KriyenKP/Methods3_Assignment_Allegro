/*	Character.cpp - Lecturer Defence Game - Computer Methods 3 - ENEL3CC - UKZN 2014
Author: R. Mawbey - 213560382
Date: 19/5/2014
Description:
Abstraction of the player into a class, used by the main game to create an instance of a player

*/


#include "Character.h"


//player
	

Character::Character()
{
}


Character::~Character()
{
}
void Character::InitCharacter()
	{
		spritex = scrn_W / 2;
		spritey = scrn_H / 2;
		ID = PLAYER;
		lives = 3;
		speed = 5;
		boundx = 65;
		boundy = 160;
	}



void Character::MoveCharacterUp()
	{
		spritey -= speed;
		if (spritey < 0) spritey = 0;
		dir = 0;
	}
void Character::MoveCharacterRight()
	{
		spritex += speed;
		if (spritex > scrn_W - 80)
			spritex = scrn_W - 80;
		dir = 1;
	}
void Character::MoveCharacterDown()
	{
		spritey += speed;
		if (spritey > scrn_H - 200)
			spritey = scrn_H - 200;
		dir = 2;

	}
void Character::MoveCharacterLeft()
	{
		spritex -= speed;
		if (spritex < 0)
			spritex = 0;
		dir = 3;
	}


