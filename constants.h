#if !defined(CONSTANTS_H)
#define CONSTANTS_H 1

const float FPS = 60;											//Frames per second
const int scrn_W = 1024;										//Screen Width 
const int scrn_H = 686;											//Screen Height
const int NUM_BULLETS = 3;										//Number of bullets  (not actual amount - more like a limit that can be on shot on the screen at any one time)
const int NUM_COMETS = 10;										//Number of enemies  (same as above)
const int NUM_BOSS = 1;											//Number of enemies  (same as above)
const int NUM_EXPLOSIONS = 5;									//Number of explosions (same as above)		
const int NUM_POWER = 1;

const int shrinkx = 200;												//x value used to adjust player size for depth 
const int shrinky = 200;												//x value used to adjust player size for depth 

enum IDS { PLAYER, BULLET, PROJECTILE, BOSS };



enum STATE{ TITLE, PLAYING, LOST, MENU, SETTINGS, HELP, WIN };		//Game state identification
enum Direction { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };	//Direction of player identification


#endif