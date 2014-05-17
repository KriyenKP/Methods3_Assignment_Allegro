const float FPS = 60;											//Frames per second
const int scrn_W = 1024;										//Screen Width 
const int scrn_H = 686;											//Screen Height
const int NUM_BULLETS = 5;										//Number of bullets  (not actual amount - more like a limit that can be on shot on the screen at any one time)
const int NUM_COMETS = 10;										//Number of enemies  (same as above)
const int NUM_BOSS = 1;											//Number of enemies  (same as above)
const int NUM_EXPLOSIONS = 5;									//Number of explosions (same as above)		
const int NUM_POWER = 1;

float crs_x = scrn_W / 2.0;										//default x location for mouse position detection
float crs_y = scrn_H / 2.0;										//default y location for mouse position detection

static ALLEGRO_COLOR red, blue, black, white, green;			//Used to create quick access to colours (versus al_map_rgb(0,0,0)

int shrinkx = 200;												//x value used to adjust player size for depth 
int shrinky = 200;												//x value used to adjust player size for depth 

enum KEYS { UP, DOWN, LEFT, RIGHT, S_UP, S_DOWN, S_LEFT, S_RIGHT, SPACE, ENTER };				//Key press shortcut, defines key constants 
enum STATE{ TITLE, PLAYING, LOST, MENU, SETTINGS, HELP, WIN };		//Game state identification
enum Direction { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };	//Direction of player identification

int playone = 1;												//restrict sound to play only once
int sign = 1;													//direction for boss movement
bool bosslevel = false;											//only let boss come after certain score
bool win = 0;
bool SecLife = false;
int poweredNum = 0;
int level = 0;

int bossCheck = 0;

int egg = 0;

//animated image var
int curFrame = 0;					//Current frame of animated image
int frameCount = 0;					//frame counter for animated image
int frameDelay = 20;				//rate at which animate image changes
int frameW = 128;					//frame width for animated image
int frameH = 128;					//frame height for animated image
const int maxFrame = 4;				//number of frames in animated image
//End animated image var
